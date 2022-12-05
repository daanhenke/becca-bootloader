#include <becca/asm.hh>

#define HighNibble(val) (val >> 4)
#define LowNibble(val) (val & 0xF)

// Based on https://github.com/Nomade040/length-disassembler/blob/master/ldisasm.cpp
// TODO: Much cleanup
namespace becca::assembly
{
    static const u8 prefixes[]  = { 0xF0, 0xF2, 0xF3, 0x2E, 0x36, 0x3E, 0x26, 0x64, 0x65, 0x66, 0x67 };
    static const u8 op1modrm[]  = { 0x62, 0x63, 0x69, 0x6B, 0xC0, 0xC1, 0xC4, 0xC5, 0xC6, 0xC7, 0xD0, 0xD1, 0xD2, 0xD3, 0xF6, 0xF7, 0xFE, 0xFF };
    static const u8 op1imm8[]   = { 0x6A, 0x6B, 0x80, 0x82, 0x83, 0xA8, 0xC0, 0xC1, 0xC6, 0xCD, 0xD4, 0xD5, 0xEB };
    static const u8 op1imm32[]  = { 0x68, 0x69, 0x81, 0xA9, 0xC7, 0xE8, 0xE9 };
    static const u8 op2modrm[]  = { 0x0D, 0xA3, 0xA4, 0xA5, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF };

    inline bool IsByteInArray(const u8* array, usize size, u8 value)
    {
        for (auto i = 0; i < size; i++)
        {
            if (array[i] == value) return true;
        }

        return false;
    }

    const u8* ParseModRM(const u8* currByte, bool addressPrefix)
    {
        auto rawModRm = *++currByte;

        if (! addressPrefix || (addressPrefix && *currByte >= 0x40))
        {
            bool hasSib = false;
            if (*currByte < 0xC0 && (*currByte & 0b111) == 0b100 && !addressPrefix)
            {
                hasSib = true;
                currByte++;
            }

            if (rawModRm >= 0x40 && rawModRm <= 0x7F) currByte++;
            else if ((rawModRm <= 0x3F && (rawModRm & 0b111) == 0b101) || (rawModRm >= 0x80 && rawModRm <= 0xBF)) currByte += (addressPrefix ? 2 : 4);
            else if (hasSib && (*currByte & 0b111) == 0b101) currByte += (rawModRm & 0b01000000) ? 1 : 4;
        }
        else if (addressPrefix && rawModRm == 0x26) currByte += 2;

        return currByte;
    }

    usize GetInstructionLength(const void* instructionPointer, bool x86_64)
    {
        auto currByte = reinterpret_cast<const u8*>(instructionPointer);

        // Parse prefixes
        bool operandPrefix, addressPrefix, rexW;
        for (auto i = 0; i < 14 && IsByteInArray(prefixes, sizeof(prefixes), *currByte) || (x86_64 ? (HighNibble(*currByte) == 4) : false); i++, currByte++)
        {
            if (*currByte == 0x66)
            {
                operandPrefix = true;
            }
            else if (*currByte == 0x67)
            {
                addressPrefix = true;
            }
            else if ((HighNibble(*currByte) == 4) && (LowNibble(*currByte) >= 8))
            {
                rexW = true;
            }
        }

        // Parse opcode(s)
        usize offset = 0;
        if (*currByte == 0x0F)
        {
            currByte++;

            if (*currByte == 0x38 || *currByte == 0x3A) // 3 bytes long
            {
                if (*currByte == 0x3A) offset++;
                currByte = ParseModRM(currByte, addressPrefix);
            }
            else // 2 bytes long
            {
                if (HighNibble(*currByte) == 8) offset += 4;
                else if (
                    (HighNibble(*currByte) == 7 && LowNibble(*currByte) < 4)
                    || *currByte == 0xA4
                    || *currByte == 0xC2
                    || (*currByte > 0xC3 && *currByte <= 0xC6)
                    || *currByte == 0xBA
                    || *currByte == 0xAC
                ) offset++;

                if (
                    IsByteInArray(op2modrm, sizeof(op2modrm), *currByte)
                    || (HighNibble(*currByte) != 3 && HighNibble(*currByte) > 0 && HighNibble(*currByte) < 7)
                    || *currByte >= 0xD0
                    || (HighNibble(*currByte) == 7 && LowNibble(*currByte) != 7)
                    || HighNibble(*currByte) == 9
                    || HighNibble(*currByte) == 0xB
                    || (HighNibble(*currByte) == 0xC && LowNibble(*currByte) < 8)
                    || (HighNibble(*currByte) == 0 && LowNibble(*currByte) < 4)
                ) currByte = ParseModRM(currByte, addressPrefix);
            }
        }
        else // 1 byte long
        {
            if (
                (HighNibble(*currByte) == 0xE && LowNibble(*currByte) < 8)
                || (HighNibble(*currByte) == 0xB && LowNibble(*currByte) < 8)
                || HighNibble(*currByte) == 7
                || (HighNibble(*currByte) < 4 && (LowNibble(*currByte) == 4 || LowNibble(*currByte) == 0xC))
                || (*currByte == 0xF6 && !(*(currByte + 1) & 48))
                || IsByteInArray(op1imm8, sizeof(op1imm8), *currByte)
            )
            {
                offset++;
            }
            else if (*currByte == 0xC2 || *currByte == 0xCA)
            {
                offset += 2;
            }
            else if (*currByte == 0xC8)
            {
                offset += 3;
            }
            else if (
                (HighNibble(*currByte) < 4 && (LowNibble(*currByte) == 5 || LowNibble(*currByte) == 0xD))
                || (HighNibble(*currByte) == 0xB && LowNibble(*currByte) >= 8)
                || (*currByte == 0xF7 && !(*(currByte + 1) & 48))
                || IsByteInArray(op1imm32, sizeof(op1imm32), *currByte)
            )
            {
                offset += rexW ? 8 : (operandPrefix ? 2 : 4);
            }
            else if (HighNibble(*currByte) == 0xA && LowNibble(*currByte) < 4)
            {
                offset += rexW ? 8 : (addressPrefix ? 2 : 4);
            }
            else if (*currByte == 0xEA || *currByte == 0x9A)
            {
                offset += operandPrefix ? 4 : 6;
            }

            if (
                IsByteInArray(op1modrm, sizeof(op1modrm), *currByte)
                || (HighNibble(*currByte) < 4 && (LowNibble(*currByte) < 4 || (LowNibble(*currByte) >= 8 && LowNibble(*currByte) < 0xC)))
                || HighNibble(*currByte) == 8
                || (HighNibble(*currByte) == 0xD && LowNibble(*currByte) >= 8)
            )
            {
                currByte = ParseModRM(currByte, addressPrefix);
            }
        }

        return (++currByte + offset) - reinterpret_cast<const u8*>(instructionPointer);
    }
}
