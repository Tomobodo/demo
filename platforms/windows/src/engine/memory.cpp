#include <windows.h>

void* operator new(size_t size)
{
    return HeapAlloc(GetProcessHeap(), 0, size);
}

void operator delete(void* p, unsigned int) noexcept
{
    if (p) HeapFree(GetProcessHeap(), 0, p);
}

void* operator new[](size_t size)
{
    return HeapAlloc(GetProcessHeap(), 0, size);
}

void operator delete[](void* p) noexcept
{
    if (p) HeapFree(GetProcessHeap(), 0, p);
}
