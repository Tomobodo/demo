typedef unsigned long size_t;

extern "C" void *memset(void *dest, int c, size_t count) {
  auto p = static_cast<unsigned char *>(dest);
  while (count--)
    *p++ = static_cast<unsigned char>(c);
  return dest;
}

extern "C" void *memcpy(void *dst, const void *src, unsigned long n) {
  unsigned char *d = (unsigned char *)dst;
  const unsigned char *s = (const unsigned char *)src;
  while (n--)
    *d++ = *s++;
  return dst;
}
