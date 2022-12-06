#pragma once

#include <becca/types.hh>

namespace becca::efi
{
    enum class DevicePathType : u8
    {
        Hardware = 1,
        Acpi,
        Message,
        Media,
        Bios,
        End = 0x7f
    };

    enum class DevicePathSubType : u8
    {
        HardwarePci = 1,
        HardwarePccard,
        HardwareMemoryMap,
        HardwareVendor,
        HardwareController,
        HardwareBmc,
        EndAll = 0xff
    };

    namespace protocol
    {
        struct DevicePathProtocol
        {
            DevicePathType Type;
            DevicePathSubType SubType;
            u8 Length[2];
        };
    }
}
