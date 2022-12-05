#include <becca/efi.hh>
#include <becca/efi/terminal.hh>
#include <becca/efi/alloc.hh>

#include <becca/hooks.hh>

namespace becca::efi
{
    status (*Orig)(void* thiz) = nullptr;

    extern "C"
    status Hook(void* thiz)
    {
        terminal::WriteLine("Hooked ClearScreen, not clearing screen >:D\n");
        return 0; // Orig(thiz);
    }

    // Main entrypoint, initializes efilib
    extern "C"
    status UefiMain(handle imageHandle, SystemTable* systemTable)
    {
        terminal::Initialize(systemTable);
        alloc::Initialize(systemTable);

        terminal::WriteLine("Hello World!");

        auto hookCtx = hooks::CreateTrampolineHook(
            systemTable->ConsoleOut->ClearScreen,
            Hook,
            reinterpret_cast<void**>(&Orig)
        );
        systemTable->ConsoleOut->ClearScreen(systemTable->ConsoleOut);

        terminal::WaitForAnyKey();
        terminal::Exit();
        return 0;
    }
}

