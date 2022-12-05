#include <becca/efi.hh>
#include <becca/efi/terminal.hh>
#include <becca/efi/alloc.hh>

#include <becca/hooks.hh>

namespace becca::efi
{
    status (*Orig)(void* thiz, wchar_t* str) = nullptr;

    extern "C"
    status Hook(void* thiz)
    {
        return 0;
    }

    // Main entrypoint, initializes efilib
    extern "C"
    status UefiMain(handle imageHandle, SystemTable* systemTable)
    {
        terminal::Initialize(systemTable);
        alloc::Initialize(systemTable);

        terminal::WriteLine("Hello World!");

        void* test;
        auto hookCtx = hooks::CreateTrampolineHook(systemTable->ConsoleOut->ClearScreen, Hook, reinterpret_cast<void**>(&Orig));
        systemTable->ConsoleOut->ClearScreen(systemTable->ConsoleOut);

        terminal::WaitForAnyKey();
        terminal::Exit();
        return 0;
    }
}

