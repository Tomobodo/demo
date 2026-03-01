void* operator new(unsigned long size)
{
    static unsigned char heap[1024 * 1024]; // 1 Mo de tas statique
    static unsigned long offset = 0;

    void* ptr = &heap[offset];
    offset += size;
    return ptr;
}

void operator delete(void* p, unsigned long size) noexcept
{
}
