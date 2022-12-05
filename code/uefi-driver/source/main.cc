#include <becca/efi.hh>

namespace becca
{
    extern "C"
    efi::status UefiMain(efi::handle imageHandle, efi::SystemTable* table)
    {
        return 0;
    }
}

