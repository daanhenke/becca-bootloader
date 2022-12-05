#pragma once

namespace becca::hooks
{
    struct TrampolineHook
    {

    };

    TrampolineHook* CreateTrampolineHook(void* target, void* hook, void** trampoline);
}
