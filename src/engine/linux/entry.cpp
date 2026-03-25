extern "C" [[noreturn]] void sys_exit(int code);

extern "C" void entry() { sys_exit(0); }
