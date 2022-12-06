#include <becca/hooks.hh>
#include <becca/asm.hh>
#include <becca/alloc.hh>
#include <becca/efi/terminal.hh> // TODO: REMOVE

namespace becca::hooks
{
    const u8 gFarJmpTemplate[] =
    {
        // endbr64
        0xf3, 0x0f, 0x1e, 0xfa,
        // mov rax, addr64
        0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        // jmp [rax]
        0xff, 0xe0
    };

    const auto FarJmpTemplateSize = sizeof(gFarJmpTemplate);
    const auto FarJmpTemplateAddressOffset = FarJmpTemplateSize - (sizeof(void*) + 2);

    TrampolineHook* CreateTrampolineHook(void* target, void* hook, void** trampoline)
    {
        auto targetArr = reinterpret_cast<u8*>(target);

        // Gets the length of x instructions where length >= the length of a jmp instruction
        usize overwrittenCodeSize = 0;
        while (overwrittenCodeSize < FarJmpTemplateSize)
        {
            auto currSize = assembly::GetInstructionLength(reinterpret_cast<void*>(targetArr + overwrittenCodeSize));
            if (currSize == 0) break;

            overwrittenCodeSize += currSize;
        }

        // Allocate space for trampoline
        auto trampolineSize = overwrittenCodeSize + FarJmpTemplateSize;
        auto trampolineBuffer = alloc::AllocateArray<u8>(trampolineSize);

        // Copy to be overwritten instructions to trampoline
        for (auto i = 0; i < overwrittenCodeSize; i++) trampolineBuffer[i] = targetArr[i];

        // Copy jmp instruction to trampoline
        for (auto i = 0; i < FarJmpTemplateSize; i++) trampolineBuffer[overwrittenCodeSize + i] = gFarJmpTemplate[i];
        *reinterpret_cast<void**>(trampolineBuffer + overwrittenCodeSize + FarJmpTemplateAddressOffset) = targetArr + overwrittenCodeSize;

        // Overwrite first instructions with our jmp instruction
        for (auto i = 0; i < FarJmpTemplateSize; i++) targetArr[i] = gFarJmpTemplate[i];
        *reinterpret_cast<void**>(targetArr + FarJmpTemplateAddressOffset) = hook;

        *trampoline = trampolineBuffer;
        return nullptr;
    }
}
