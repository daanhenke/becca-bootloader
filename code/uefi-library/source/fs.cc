#include <becca/efi/fs.hh>
#include <becca/efi/protocol/LoadedImage.hh>
#include <becca/efi/protocol/SimpleFileSystem.hh>

#include <becca/efi/terminal.hh>
#include <becca/efi/alloc.hh>

namespace becca::efi::fs
{
    protocol::FileProtocol* gBootVolume = nullptr;

    status Initialize(handle imageHandle, efi::SystemTable* systemTable)
    {
        status result = 0;
        auto& bootServices = systemTable->BootServices;

        protocol::LoadedImageProtocol* myImage = nullptr;
        if ((result = bootServices->HandleProtocol(imageHandle, &guids::LoadedImageProtocol, reinterpret_cast<void**>(&myImage))))
        {
            return result;
        }

        protocol::SimpleFileSystemProtocol* fs = nullptr;
        if ((result = bootServices->HandleProtocol(myImage->DeviceHandle, &guids::SimpleFileSystemProtocol, reinterpret_cast<void**>(&fs))))
        {
            return result;
        }

        return fs->OpenVolume(fs, &gBootVolume);
    }

    void Exit()
    {
        gBootVolume->Close(gBootVolume);
    }

    char* ReadFile(const char* path)
    {
        // Translate path to widechars
        wchar_t wideBuffer[sizeof(wchar_t) * 512]; // No idea what max path length is, 512 is a pretty safe bet
        terminal::WriteTo(path, wideBuffer, sizeof(wideBuffer));

        // Open the file
        protocol::FileProtocol* file = nullptr;
        if (gBootVolume->Open(gBootVolume, &file, wideBuffer, 1, 0))
        {
            return nullptr;
        }

        // Read file info to determine size
        usize infoSize = 0;
        file->GetInfo(file, &guids::FileInfoId, &infoSize, nullptr);

        u8 infoBuffer[infoSize];
        auto info = reinterpret_cast<FileInfo*>(infoBuffer);
        file->GetInfo(file, &guids::FileInfoId, &infoSize, infoBuffer);

        // Allocate and read file
        auto fileSize = info->FileSize;
        auto fileBuffer = alloc::AllocateArray<u8>(fileSize + 1);

        file->Read(file, &fileSize, fileBuffer);
        fileBuffer[info->FileSize] = '\0';

        return reinterpret_cast<char*>(fileBuffer);
    }
}
