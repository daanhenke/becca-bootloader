#pragma once

#include <becca/alloc.hh>
#include <becca/efi/system.hh>

namespace becca::alloc
{
    void Initialize(efi::SystemTable* systemTable);
}
