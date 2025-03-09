#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <algorithm>
#include <iomanip> // For hex formatting

using namespace std;

// Opcode mappings
unordered_map<string, string> opcode_map = {
    {"add", "0110011"}, {"andi", "0010011"}, {"or", "0110011"}, {"sll", "0110011"},
    {"slt", "0110011"}, {"sra", "0110011"}, {"srl", "0110011"}, {"sub", "0110011"},
    {"xor", "0110011"}, {"mul", "0110011"}, {"div", "0110011"}, {"rem", "0110011"},
    {"addi", "0010011"}, {"andi", "0010011"}, {"ori", "0010011"}, {"lb", "0000011"},
    {"ld", "0000011"}, {"lh", "0000011"}, {"lw", "0000011"}, {"jalr", "1100111"},
    {"sb", "0100011"}, {"sw", "0100011"}, {"sd", "0100011"}, {"sh", "0100011"},
    {"beq", "1100011"}, {"bne", "1100011"}, {"bge", "1100011"}, {"blt", "1100011"},
    {"auipc", "0010111"}, {"lui", "0110111"}, {"jal", "1101111"}
};

// Funct3 mappings
unordered_map<string, string> funct3_map = {
    {"add", "000"}, {"andi", "111"}, {"or", "110"}, {"sll", "001"},
    {"slt", "010"}, {"sra", "101"}, {"srl", "101"}, {"sub", "000"},
    {"xor", "100"}, {"mul", "000"}, {"div", "100"}, {"rem", "110"},
    {"addi", "000"}, {"andi", "111"}, {"ori", "110"}, {"lb", "000"},
    {"ld", "011"}, {"lh", "001"}, {"lw", "010"}, {"jalr", "000"},
    {"sb", "000"}, {"sw", "010"}, {"sd", "011"}, {"sh", "001"},
    {"beq", "000"}, {"bne", "001"}, {"bge", "101"}, {"blt", "100"}
};

// Funct7 mappings for R-type
unordered_map<string, string> funct7_map = {
    {"add", "0000000"}, {"and", "0000000"}, {"or", "0000000"}, {"sll", "0000000"},
    {"slt", "0000000"}, {"sra", "0100000"}, {"srl", "0000000"}, {"sub", "0100000"},
    {"xor", "0000000"}, {"mul", "0000001"}, {"div", "0000001"}, {"rem", "0000001"}
};

// Register mappings
unordered_map<string, string> register_map = {
    {"x0", "00000"}, {"x1", "00001"}, {"x2", "00010"}, {"x3", "00011"},
    {"x4", "00100"}, {"x5", "00101"}, {"x6", "00110"}, {"x7", "00111"},
    {"x8", "01000"}, {"x9", "01001"}, {"x10", "01010"}, {"x11", "01011"},
    {"x12", "01100"}, {"x13", "01101"}, {"x14", "01110"}, {"x15", "01111"},
    {"x16", "10000"}, {"x17", "10001"}, {"x18", "10010"}, {"x19", "10011"},
    {"x20", "10100"}, {"x21", "10101"}, {"x22", "10110"}, {"x23", "10111"},
    {"x24", "11000"}, {"x25", "11001"}, {"x26", "11010"}, {"x27", "11011"},
    {"x28", "11100"}, {"x29", "11101"}, {"x30", "11110"}, {"x31", "11111"}
};

// Helper function to clean register names (remove commas)
string cleanRegister(string reg) {
    reg.erase(remove(reg.begin(), reg.end(), ','), reg.end());
    return reg;
}

// Convert assembly to machine code
string assemble(string line, string &bitfield_format) {
    stringstream ss(line);
    string opcode, rd, rs1, rs2, imm;
    ss >> opcode;

    if (opcode_map.find(opcode) == opcode_map.end()) 
        return "ERROR: Invalid instruction";

    string machine_code;

    if (opcode_map[opcode] == "0110011") {  // R-type instruction
        ss >> rd >> rs1 >> rs2;
        rd = cleanRegister(rd);
        rs1 = cleanRegister(rs1);
        rs2 = cleanRegister(rs2);

        if (register_map.find(rd) == register_map.end() ||
            register_map.find(rs1) == register_map.end() ||
            register_map.find(rs2) == register_map.end()) {
            return "ERROR: Invalid register";
        }

        string funct7 = funct7_map[opcode];
        string funct3 = funct3_map[opcode];
        string opcode_bin = opcode_map[opcode];

        machine_code = funct7 + register_map[rs2] + register_map[rs1] +
                       funct3 + register_map[rd] + opcode_bin;

        bitfield_format = opcode_bin + "-" + funct3 + "-" + funct7 + "-" + 
                          register_map[rd] + "-" + register_map[rs1] + "-" + register_map[rs2] + "-NULL";
    } 
    else if (opcode_map[opcode] == "0010011") {  // I-type instruction
        ss >> rd >> rs1 >> imm;
        rd = cleanRegister(rd);
        rs1 = cleanRegister(rs1);

        if (register_map.find(rd) == register_map.end() ||
            register_map.find(rs1) == register_map.end()) {
            return "ERROR: Invalid register";
        }

        bitset<12> imm_bin(stoi(imm));
        string funct3 = funct3_map[opcode];
        string opcode_bin = opcode_map[opcode];

        machine_code = imm_bin.to_string() + register_map[rs1] + funct3 +
                       register_map[rd] + opcode_bin;

        bitfield_format = opcode_bin + "-" + funct3 + "-NULL-" +
                          register_map[rd] + "-" + register_map[rs1] + "-" + imm_bin.to_string();
    }

    // Convert binary string to proper 8-digit hex (FORCE UPPERCASE)
    unsigned int hex_code = stoi(machine_code, nullptr, 2);
    stringstream hex_stream;
    hex_stream << "0x" << uppercase << setfill('0') << setw(8) << hex << hex_code;

    return hex_stream.str();
}

int main() {
    ifstream input("input.asm");
    ofstream output("output.mc");

    if (!input.is_open() || !output.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    string line;
    int address = 0;

    while (getline(input, line)) {
        string bitfield_format;
        string machine_code = assemble(line, bitfield_format);

        if (machine_code.substr(0, 5) == "ERROR") {
            cerr << "Invalid instruction: " << line << endl;
            continue;
        }

        stringstream address_stream;
        address_stream << "0x" << uppercase << hex << address; // Force uppercase address

        output << address_stream.str() << " " << machine_code 
               << " , " << line << " # " << bitfield_format << endl;

        address += 4;
    }

    input.close();
    output.close();
    return 0;
}