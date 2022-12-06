#pragma once

#include <becca/efi/system.hh>

namespace becca::efi::terminal
{
    status Initialize(SystemTable* SystemTable);
    void Exit();

    const char* WriteTo(const char* string, wchar_t* buffer, usize bufferSize);
    void Write(const char* value);
    void WriteLine(const char* value);
    void WriteUnsigned(usize value, bool withEndline = true, usize radix = 16);
    void WriteHexDump(void* buffer, usize size = 64);

    void WaitForKey(wchar_t key);
    wchar_t WaitForAnyKey();
}
