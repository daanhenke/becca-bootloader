#pragma once

#include <becca/efi/types.hh>
#include <becca/efi/protocol/File.hh>

namespace becca::efi
{
    namespace guids
    {
        const guid SimpleFileSystemProtocol = { 0x0964e5b22, 0x6459, 0x11d2, { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } };
    }

    namespace protocol
    {
        struct SimpleFileSystemProtocol
        {
            u64 Revision;
            status (*OpenVolume)(SimpleFileSystemProtocol* thiz, FileProtocol** volume);
        };
    }
}
