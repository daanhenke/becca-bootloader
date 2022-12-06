#pragma once

#include <becca/efi/system.hh>

namespace becca::efi::fs
{
    status Initialize(handle imageHandle, SystemTable* SystemTable);
    void Exit();

    char* ReadFile(const char* path);
}
