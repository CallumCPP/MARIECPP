#include <iostream>
#include <fstream>
#include <string.h>

typedef unsigned short ushort; // 2 bytes
typedef unsigned short uchar; // 1 byte

// MARIE instruction set (this is a MARIE simulator now)
enum INSTRUCTION {
    JNS = 0x0,
    LOAD = 0x1,
    STORE = 0x2,
    ADD = 0x3,
    SUBT = 0x4,
    INPUT = 0x5,
    OUTPUT = 0x6,
    HALT = 0x7,
    SKIPCOND = 0x8,
    JUMP = 0x9,
    CLEAR = 0xA,
    ADDI = 0xB,
    JUMPI = 0xC,
    LOADI = 0xD,
    STOREI = 0xE
};

ushort memory[256];
ushort programCounter = 0;
ushort accumulator = 0;

void parseInstruction(INSTRUCTION instruction, uchar operand) {
    switch (instruction) {
        case (JNS): {
            // std::cout << "JNS called\n";
            memcpy(&memory[operand], &programCounter, sizeof(programCounter));
            operand++;
            memcpy(&programCounter, &operand, sizeof(operand));
            break;
        }

        case (LOAD): {
            // std::cout << "LOAD called\n";
            memcpy(&accumulator, &operand, sizeof(operand));
            programCounter++;
            break;
        }
        
        case (STORE): {
            // std::cout << "STORE called\n";
            memcpy(&memory[operand], &accumulator, sizeof(accumulator));
            programCounter++;
            break;
        }

        case (ADD): {
            // std::cout << "ADD called\n";
            accumulator += operand;
            
            programCounter++;
            break;
        }

        case (SUBT): {
            // std::cout << "SUBT called\n";
            accumulator -= *(ushort*)&memory[operand];
            programCounter++;
            break;
        }

        case (INPUT): {
            // std::cout << "INPUT called\n";
            std::cout << "Enter an integer: ";
            std::cin >> accumulator;
            programCounter++;
            break;
        }

        case (OUTPUT): {
            // std::cout << "OUTPUT called\n";
            std::cout << accumulator << '\n';
            programCounter++;
            break;
        }

        case (HALT): {
            // std::cout << "HALT called\n";
            exit(0);
            break;
        }

        case (SKIPCOND): {
            // std::cout << "SKIPCOND called\n";
            if (operand == 000)
                programCounter = (accumulator < 0) ? programCounter+1 : programCounter;
                
            else if (operand == 400)
                programCounter = (accumulator == 0) ? programCounter+1 : programCounter;

            else if (operand == 800)
                programCounter = (accumulator > 0) ? programCounter+1 : programCounter;

            programCounter++;
            break;
        }

        case (JUMP): {
            // std::cout << "JUMP called\n";
            memcpy(&programCounter, &operand, sizeof(operand));
            break;
        }

        case (CLEAR): {
            // std::cout << "CLEAR called\n";
            accumulator = 0;
            programCounter++;
            break;
        }

        case (ADDI): {
            // std::cout << "ADDI called\n";
            ushort realAddress = *(ushort*)&memory[operand];
            accumulator += *(ushort*)&memory[realAddress];
            programCounter++;
            break;
        }

        case (JUMPI): {
            // std::cout << "JUMPI called\n";
            ushort realAddress = *(ushort*)&memory[operand];
            memcpy(&programCounter, &realAddress, sizeof(realAddress));
            programCounter++;
            break;
        }

        case (LOADI): {
            // std::cout << "LOADI called\n";
            ushort realAddress = *(ushort*)&memory[operand];
            memcpy(&accumulator, &realAddress, sizeof(realAddress));
            programCounter++;
            break;
        }

        case (STOREI): {
            // std::cout << "STOREI called\n";
            ushort realAddress = *(ushort*)&memory[operand];
            memcpy(&memory[operand], &accumulator, sizeof(accumulator));
            programCounter++;
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: ./ImaginaryCPU <path-to-executable>\n";
        return 1;
    }

    std::ifstream executableFile(argv[1]);
    if (executableFile.fail()) {
        std::cout << "Invalid file path. Try again.\n";
        return 0;
    }

    executableFile.seekg(0, std::ios::end);
    size_t executableSize = executableFile.tellg();
    executableFile.seekg(0, std::ios::beg);

    if (executableSize > sizeof(memory)) {
        std::cout << "Executable too big. 4kB maximum.\n";
        return 0;
    }

    executableFile.read((char*)memory, executableSize);

    // for (int i = 0; i < executableSize; i++) {
    //     std::cout << std::hex << (int)((char*)memory)[i];
    // }

    while (true) {
        parseInstruction((INSTRUCTION)*(char*)&memory[programCounter], *(char*)(&memory[programCounter]+sizeof(uchar)));
    }

    return 0;
}
