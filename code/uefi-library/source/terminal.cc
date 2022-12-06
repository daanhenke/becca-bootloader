#include <becca/efi/terminal.hh>
#include <becca/string.hh>

namespace becca::efi::terminal
{
    usize gSavedMode;
    protocol::SimpleTextOutputProtocol* gOut;
    protocol::SimpleTextInputProtocol* gIn;
    BootServices* gBootServices;

    // Setup SimpleOutputProtocol, switches to highest resolution and clears the screen
    status Initialize(SystemTable* systemTable)
    {
        gBootServices = systemTable->BootServices;
        gOut = systemTable->ConsoleOut;
        gIn = systemTable->ConsoleIn;
        gSavedMode = static_cast<usize>(gOut->Mode->Mode);

        usize maxWidth = 0, maxIndex = 0;
        for (auto i = 0; i < gOut->Mode->MaxMode; i++)
        {
            usize currWidth, currHeight;
            gOut->QueryMode(gOut, i, &currWidth, &currHeight);

            if (currWidth > maxWidth)
            {
                maxWidth = currWidth;
                maxIndex = i;
            }
        }

        gOut->SetMode(gOut, maxIndex);

        return 0;
    }

    // Resets StandardOutputProtocol to pre-becca state
    void Exit()
    {
        gOut->SetMode(gOut, gSavedMode);
    }

    // TODO: Move out of terminal, also used in fs
    // Writes an ascii string into a widestring buffer
    const char* WriteTo(const char* string, wchar_t* buffer, usize bufferSize)
    {
        auto currIn = string;
        auto currOut = buffer;

        while (((currOut - buffer) < bufferSize - 1) && *currIn != '\0')
        {
            auto currChar = *currIn++;
            if (currChar == '\n')
            {
                // Linebreak conversion
                *currOut++ = L'\r';
            }

            *currOut++ = wchar_t(currChar);
        }

        *currOut = L'\0';

        // We want to return nullptr if we're finished, and the next character if we're not
        if (*currIn == '\0')
        {
            return nullptr;
        }

        return currIn;
    }

    void Write(const char* string)
    {
        if (string == nullptr) string = "(null)";

        wchar_t buffer[512];
        auto curr = string;

        while (curr != nullptr)
        {
            curr = WriteTo(curr, buffer, sizeof(buffer) / sizeof(wchar_t));
            gOut->OutputString(gOut, buffer);
        }
    }

    void WriteLine(const char* string)
    {
        Write(string);
        Write("\n");
    }

    void WriteUnsigned(usize value, bool withEndline, usize radix)
    {
        char buffer[17];
        string::Append(value, buffer, radix);
        Write(buffer);
        if (withEndline) Write("\n");
    }

    void WriteHexDump(void* buffer, usize size)
    {
        auto curr = reinterpret_cast<u8*>(buffer);

        WriteLine("\n------------------------");

        auto bytesOnCurrentLine = 0;
        for (auto i = 0; i < size; i++)
        {
            if (bytesOnCurrentLine++ >= 16)
            {
                Write("\n");
                bytesOnCurrentLine = 0;
            }
            WriteUnsigned(*curr++, false, 16);
            Write(" ");
        }

        WriteLine("\n------------------------");
    }


    void WaitForKey(wchar_t key)
    {
        while (WaitForAnyKey() != key);
    }

    wchar_t WaitForAnyKey()
    {
        WriteLine("Press any key to continue...");
        Write("> ");
        gOut->EnableCursor(gOut, true);

        gBootServices->WaitForEvent(1, &gIn->WaitForKey, nullptr);
        InputKey key;
        gIn->ReadKeyStroke(gIn, &key);

        gOut->EnableCursor(gOut, false);

        return key.UnicodeChar;
    }
}
