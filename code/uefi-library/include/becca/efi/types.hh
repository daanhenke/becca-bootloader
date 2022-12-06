#pragma once

#include <becca/types.hh>

namespace becca::efi
{
    typedef u64 status;
    typedef void* handle;
    typedef void* event;

    struct guid
    {
        u32 A;
        u16 B;
        u16 C;
        u8 D[8];
    };

    struct time
    {
        u16 Year;
        u8 Month;
        u8 Day;
        u8 Hour;
        u8 Minute;
        u8 Second;
        u8 Pad1;
        u32 NanoSecond;
        s16 TimeZone;
        u8 Daylight;
        u8 Pad2;
    };
}
