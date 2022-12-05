#pragma once

#include <becca/efi/types.hh>
#include <becca/efi/protocol/SimpleTextInput.hh>
#include <becca/efi/protocol/SimpleTextOutput.hh>

namespace becca::efi
{
    enum class MemoryType
    {
        ReservedMemory,
        LoaderCode,
        LoaderData,
        BootServicesCode,
        BootServicesData,
        RuntimeServicesCode,
        RuntimeServicesData,
        ConventionalMemory,
        UnusableMemory,
        AcpiReclaimMemory,
        AcpiMemoryNvs,
        MemoryMappedIO,
        MemoryMappedIOPortSpace,
        PalCode,
        PersistentMemory
    };

    struct TableHeader
    {
        u64 Signature;
        u32 Revision;
        u32 HeaderSize;
        u32 Crc32;
        u32 Reserved;
    };

    struct RuntimeServices
    {
        TableHeader Header;

    };

    struct BootServices
    {
        TableHeader Header;

        u64 (*RaiseTpl)(u64 newTpl);
        void (*RestoreTpl)(u64 oldTpl);

        status (*AllocatePages)(); // Dummy
        status (*FreePages)(); // Dummy
        status (*GetMemoryMap)(); // Dummy
        status (*AllocatePool)(MemoryType poolType, usize size, void** buffer);
        status (*FreePool)(void* buffer);
        status (*CreateEvent)(); // Dummy
        status (*SetTimer)(); // Dummy
        status (*WaitForEvent)(usize NumberOfEvents, event* events, usize* index);
    };

    struct SystemTable
    {
        TableHeader Header;
        const wchar_t* FirmwareVendor;
        u32 FirmwareRevision;

        handle ConsoleInHandle;
        protocol::SimpleTextInputProtocol* ConsoleIn;
        handle ConsoleOutHandle;
        protocol::SimpleTextOutputProtocol* ConsoleOut;
        handle StdErrHandle;
        protocol::SimpleTextOutputProtocol* StdErr;

        RuntimeServices* RuntimeServices;
        BootServices* BootServices;

        usize ConfigurationTableEntryCount;
        void* ConfigurationTable;
    };
}
