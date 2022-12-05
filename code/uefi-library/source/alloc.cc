#include <becca/efi/alloc.hh>

namespace becca::alloc
{
    efi::BootServices* gBootServices2 = nullptr;

    void Initialize(efi::SystemTable* systemTable)
    {
        gBootServices2 = systemTable->BootServices;
    }

    void* Allocate(usize size)
    {
        void* result = nullptr;
        gBootServices2->AllocatePool(efi::MemoryType::LoaderData, size, &result);
        return result;
    }

    void Free(void* address)
    {
        gBootServices2->FreePool(address);
    }
}
