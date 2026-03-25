typedef unsigned long size_t;

#define MAP_PRIVATE 0x02
#define MAP_ANONYMOUS 0x20
#define PROT_READ 0x01
#define PROT_WRITE 0x02
#define SYS_mmap 9
#define SYS_munmap 11

extern "C" long syscall6(long n, long a1, long a2, long a3, long a4, long a5,
                         long a6) {
  long ret;
  asm volatile("syscall"
               : "=a"(ret)
               : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(a4), "r"(a5), "r"(a6)
               : "rcx", "r11", "memory");
  return ret;
}

void *operator new(size_t size) {
  void *p = (void *)syscall6(SYS_mmap, 0, size, PROT_READ | PROT_WRITE,
                             MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  if ((long)p < 0 && (long)p > -4096)
    return nullptr;
  return p;
}

void operator delete(void *p, size_t size) noexcept {
  if (!p)
    return;
  syscall6(SYS_munmap, (long)p, size, 0, 0, 0, 0);
}

void *operator new[](size_t size) { return operator new(size); }

void operator delete[](void *p) noexcept {}

extern "C" void *memset(void *dest, int c, size_t count) {
  auto p = static_cast<unsigned char *>(dest);
  while (count--)
    *p++ = static_cast<unsigned char>(c);
  return dest;
}
