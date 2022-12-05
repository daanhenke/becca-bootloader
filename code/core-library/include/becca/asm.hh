#pragma once

#include <becca/types.hh>

namespace becca::assembly
{
    namespace instructions
    {
        struct modrm
        {
            u8 RegisterOrMemory: 3;
            u8 Register: 3;
            u8 Mode: 2;
        };

        struct __attribute__((packed)) jmp_reg
        {
            u8 OpCode;
            modrm ModRm;
        };

        struct __attribute__((packed)) mov_reg_abs
        {
            u8 OpCode;
            modrm ModRm;
            u64 Address;
        };
    }

    usize GetInstructionLength(const void* instructionPointer, bool x86_64 = true);
}
