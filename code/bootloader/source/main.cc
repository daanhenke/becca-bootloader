#include <becca/efi.hh>
#include <becca/efi/terminal.hh>
#include <becca/efi/alloc.hh>
#include <becca/efi/fs.hh>

#include <becca/efi/protocol/LoadedImage.hh>
#include <becca/hooks.hh>

namespace becca::efi
{
    status (*Orig)(void* thiz, const wchar_t* thing) = nullptr;

    extern "C"
    status Hook(void* thiz, const wchar_t* thing)
    {
        return Orig(thiz, L"No logging for you!\r\n");
        return 0; // Orig(thiz);
    }

    // Main entrypoint, initializes efilib
    extern "C"
    status UefiMain(handle imageHandle, SystemTable* systemTable)
    {
        status result = 0;

        if ((result = terminal::Initialize(systemTable)))
        {
            return result;
        }

        alloc::Initialize(systemTable);

        if ((result = fs::Initialize(imageHandle, systemTable)))
        {
            return result;
        }

        terminal::WriteLine("Hello World!");

        auto text = fs::ReadFile("\\becca\\motd.txt");
        terminal::WriteLine(text);

        terminal::WaitForAnyKey();

        fs::Exit();
        terminal::Exit();
        return 0;
    }
}

