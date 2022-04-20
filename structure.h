#pragma once

namespace pkodev
{
#pragma pack(push, 1)

    // Assembler call instruction
    struct asm_call
    {
        unsigned char opcode;
        unsigned int offset;

        // Constructor
        asm_call() :
            opcode(0xE8),
            offset(0x00000000)
        {

        }

        // Constructor
        asm_call(unsigned int offset_) :
            opcode(0xE8),
            offset(offset_)
        {

        }
    };

    // Assembler push instruction
    struct asm_push
    {
        unsigned char opcode;
        unsigned char value;

        // Constructor
        asm_push() :
            opcode(0x6A),
            value(0)
        {

        }

        // Constructor
        asm_push(char value_) :
            opcode(0x6A),
            value(value_)
        {

        }
    };

#pragma pack(pop)
}