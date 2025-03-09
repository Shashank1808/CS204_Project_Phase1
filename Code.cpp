#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>

using namespace std;

// Define a set of valid RISC-V instructions
unordered_set<string> validInstructions = {
    "add", "sub", "and", "or", "sll", "slt", "sra", "srl", "xor", "mul", "div", "rem",
    "addi", "ori", "lb", "ld", "lh", "lw", "jalr", "sb", "sw", "sd", "sh", "andi",
    "beq", "bne", "bge", "blt", "auipc", "lui", "jal"
};

// Function to validate instruction
bool isValidInstruction(const string& instruction) {
    // Extract the instruction name (first word)
    istringstream iss(instruction);
    string instName;
    iss >> instName;
    return validInstructions.find(instName) != validInstructions.end();
}

// Function to convert assembly instruction to machine code
string assembleInstruction(const string& instruction, int address) {
    // This is a placeholder for actual instruction conversion logic
    // You will need to implement the conversion for each instruction type
    // For now, we will return a dummy machine code
    string machineCode = "0x00000000"; // Dummy machine code
    return "0x" + to_string(address) + " " + machineCode + " , " + instruction + " # Dummy encoding";
}

int main() {
    ifstream inputFile("input.asm");
    ofstream outputFile("output.mc");
    string line;
    int address = 0x00000000; // Starting address for code segment
    bool inDataSegment = false; // Flag to track if we are in the data segment

    if (!inputFile.is_open()) {
        cerr << "Error opening input file!" << endl;
        return 1;
    }

    // Read each line from the input assembly file
    while (getline(inputFile, line)) {
        // Trim whitespace (optional)
        line.erase(0, line.find_first_not_of(" \n\r\t")); // Left trim
        line.erase(line.find_last_not_of(" \n\r\t") + 1); // Right trim

        if (!line.empty()) {
            // Check for segment directives
            if (line == ".text") {
                inDataSegment = false; // Switch to text segment
                address = 0x00000000; // Reset address for text segment
                continue;
            } else if (line == ".data") {
                inDataSegment = true; // Switch to data segment
                address = 0x10000000; // Set address for data segment
                continue;
            }

            // Validate the instruction
            if (!isValidInstruction(line) && !inDataSegment) {
                cerr << "Error: Invalid instruction '" << line << "' at address " << hex << address << endl;
                continue; // Skip to the next instruction
            }

            // Convert the instruction to machine code
            string machineCode = assembleInstruction(line, address);
            outputFile << machineCode << endl;

            // Increment address based on segment
            if (!inDataSegment) {
                address += 0x4; // Increment address for next instruction in text segment
            } else {
                address += 0x4; // Increment address for next data entry (if applicable)
            }
        }
    }

    inputFile.close();
    outputFile.close();
    cout << "Assembly conversion completed. Output written to output.mc" << endl;

    return 0;
}