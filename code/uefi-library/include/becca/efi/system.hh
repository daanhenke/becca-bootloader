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

    namespace OpenProtocolMode
    {
        const auto ByHandleProtocol = 1;
        const auto GetProtocol = 2;
        const auto TestProtocol = 4;
        const auto ByChildController = 8;
        const auto ByDriver = 16;
        const auto Exclusive = 32;
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
        status (*SignalEvent)(); // Dummy
        status (*CloseEvent)(); // Dummy
        status (*CheckEvent)(); // Dummy
        status (*InstallProtocolInterface)(); // Dummy
        status (*ReinstallProtocolInterface)(); // Dummy
        status (*UninstallProtocolInterface)(); // Dummy
        status (*HandleProtocol)(handle handle, const guid* protocol, void** interface); // Seems to not work most of the time, very cool!
        void* Reserved;
        status (*RegisterProtocolNotify)(); // Dummy
        status (*LocateHandle)(); // Dummy
        status (*LocateDevicePath)(); // Dummy
        status (*InstallConfigurationTable)(); // Dummy
        status (*LoadImage)(); // Dummy
        status (*StartImage)(); // Dummy
        status (*Exit)(); // Dummy
        status (*UnloadImage)(); // Dummy
        status (*ExitBootServices)(); // Dummy
        status (*GetNextMonotonicCount)(); // Dummy
        status (*Stall)(); // Dummy
        status (*SetWatchdogTimer)(); // Dummy

        // 1.1+
        status (*ConnectController)(); // Dummy
        status (*DisconnectController)(); // Dummy
        status (*OpenProtocol)(handle tgtHandle, const guid* protocol, void** interface, handle agentHandle, handle controllerHandle, u32 mode);
        status (*CloseProtocol)(); // Dummy
        status (*OpenProtocolInformation)(); // Dummy
        status (*ProtocolsPerHandle)(); // Dummy
        status (*LocateHandleBuffer)(); // Dummy
        status (*LocateProtocol)(const guid* protocol, void* registration, void** interface);
        status (*InstallMultipleProtocolInterfaces)(); // Dummy
        status (*UninstallMultipleProtocolInterfaces)(); // Dummy
        status (*CalculateCrc32)(); // Dummy
        status (*CopyMemory)(); // Dummy
        status (*SetMemory)(); // Dummy
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
