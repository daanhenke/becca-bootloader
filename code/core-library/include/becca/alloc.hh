#pragma once

#include <becca/types.hh>

namespace becca::alloc
{
    void* Allocate(usize size);
    void Free(void* address);

    template <typename T>
    T* Allocate()
    {
        return reinterpret_cast<T*>(Allocate(sizeof(T)));
    }

    template <typename T>
    T* AllocateArray(usize size)
    {
        return reinterpret_cast<T*>(Allocate(sizeof(T) * size));
    }

}
