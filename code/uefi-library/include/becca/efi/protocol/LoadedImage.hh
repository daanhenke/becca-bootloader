#pragma once

#include <becca/efi/system.hh>
#include <becca/efi/protocol/DevicePath.hh>

namespace becca::efi
{
    namespace guids
    {
        const static guid LoadedImageProtocol = { 0x5B1B31A1, 0x9562, 0x11D2, { 0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B } };
    }

    namespace protocol
    {
        struct LoadedImageProtocol
        {
            u32 Revision;
            handle ParentHandle;
            SystemTable* SystemTable;
            handle DeviceHandle;
            DevicePathProtocol* FilePath;
            void* Reserved;
            u32 LoadOptionsSize;
            void* LoadOptions;
            void* ImageBase;
            u64 ImageSize;
            MemoryType ImageCodeType;
            MemoryType ImageDataType;

            status (*Unload)(handle ImageHandle);
        };
    }
}
