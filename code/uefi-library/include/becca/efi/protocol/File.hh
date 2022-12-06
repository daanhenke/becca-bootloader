#pragma once

#include <becca/efi/types.hh>

namespace becca::efi
{
    namespace guids
    {
        const guid FileInfoId = { 0x09576e92, 0x6d3f, 0x11d2, { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } };
        const guid FileSystemInfoId = { 0x09576e93, 0x6d3f, 0x11d2, { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } };
        const guid FileSystemVolumeInfoId = { 0xdb47d7d3, 0xfe81, 0x11d3, { 0x9a, 0x35, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d } };
    }

    struct FileInfo
    {
        u64 Size;
        u64 FileSize;
        u64 PhysicalSize;
        time CreateTime;
        time LastAccessTime;
        time ModificationTime;
        u64 Attribute;
        wchar_t FileName[];
    };

    namespace protocol
    {
        struct FileProtocol
        {
            u64 Revision;
            status (*Open)(FileProtocol* thiz, FileProtocol** newHandle, const wchar_t* fileName, u64 openMode, u64 attributes);
            status (*Close)(FileProtocol* thiz);
            status (*Delete)(FileProtocol* thiz);
            status (*Read)(FileProtocol* thiz, usize* bufferSize, void* buffer);
            status (*Write)(FileProtocol* thiz, usize* bufferSize, void* buffer);
            status (*GetPosition)(FileProtocol* thiz, u64* position);
            status (*SetPosition)(FileProtocol* thiz, u64 position);
            status (*GetInfo)(FileProtocol* thiz, const guid* informationType, usize* bufferSize, void* buffer);
            status (*SetInfo)(FileProtocol* thiz, const guid* informationType, usize bufferSize, void* buffer);
            status (*Flush)(FileProtocol* thiz);
        };
    }
}
