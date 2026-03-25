typedef unsigned long size_t;

extern "C" void *memset(void *dest, int c, size_t count) {
  auto p = static_cast<unsigned char *>(dest);
  while (count--)
    *p++ = static_cast<unsigned char>(c);
  return dest;
}
