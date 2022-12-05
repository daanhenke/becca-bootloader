#pragma once

#include <becca/types.hh>

namespace becca::efi
{
    struct InputKey
    {
        u16 ScanCode;
        wchar_t UnicodeChar;
    };

    namespace protocol
    {
        struct SimpleTextInputProtocol
        {
            status (*Reset)(SimpleTextInputProtocol* thiz, bool extendedVerification);
            status (*ReadKeyStroke)(SimpleTextInputProtocol* thiz, InputKey* key);
            event WaitForKey;
        };
    }
}
