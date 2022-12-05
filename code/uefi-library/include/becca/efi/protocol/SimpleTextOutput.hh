#pragma once

#include <becca/types.hh>

namespace becca::efi
{
    struct SimpleTextOutputMode
    {
        s32 MaxMode;
        s32 Mode;
        s32 Attribute;
        s32 CursorColumn;
        s32 CursorRow;
        bool CursorVisible;
    };

    namespace protocol
    {
        struct SimpleTextOutputProtocol
        {
            status (*Reset)(SimpleTextOutputProtocol* thiz, bool extendedVerification);
            status (*OutputString)(SimpleTextOutputProtocol* thiz, const wchar_t* string);
            status (*TestString)(SimpleTextOutputProtocol* thiz, const wchar_t* string);
            status (*QueryMode)(SimpleTextOutputProtocol* thiz, usize modeNumber, usize* columns, usize* rows);
            status (*SetMode)(SimpleTextOutputProtocol* thiz, usize modeNumber);
            status (*SetAttribute)(SimpleTextOutputProtocol* thiz, usize attribute);
            status (*ClearScreen)(SimpleTextOutputProtocol* thiz);
            status (*SetCursorPosition)(SimpleTextOutputProtocol* thiz, usize column, usize row);
            status (*EnableCursor)(SimpleTextOutputProtocol* thiz, bool visible);
            SimpleTextOutputMode* Mode;
        };
    }
}
