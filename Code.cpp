#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <algorithm>
#include <iomanip>

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

// Function to check if a register is valid
bool isValidRegister(const string& reg) {
    return register_map.find(reg) != register_map.end();
}

// Function to parse input file and generate machine code
void processInstructions(const string& inputFile, const string& outputFile) {
    ifstream input(inputFile);
    ofstream output(outputFile);
    if (!input.is_open() || !output.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string line;
    int address = 0;
    while (getline(input, line)) {
        stringstream ss(line);
        string opcode, rd, rs1, rs2;
        ss >> opcode;

        if (opcode_map.find(opcode) == opcode_map.end()) {
            cerr << "Invalid instruction: " << line << endl;
            continue;
        }

        if (opcode_map[opcode] == "0110011") { // R-type
            ss >> rd >> rs1 >> rs2;
            if (!isValidRegister(rd) || !isValidRegister(rs1) || !isValidRegister(rs2)) {
                cerr << "Invalid register in instruction: " << line << endl;
                continue;
            }
        }
        // Additional checks for other instruction types can be added here
        output << "0x" << hex << address << " " << line << endl;
        address += 4;
    }

    input.close();
    output.close();
}

int main() {
    processInstructions("input.asm", "output.mc");
    return 0;
}
