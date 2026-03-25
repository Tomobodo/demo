.globl sys_exit
sys_exit:
  mov $60, %rax
  syscall

.globl sys_clock_gettime
sys_clock_gettime:
  mov $228, %rax
  syscall
  ret
