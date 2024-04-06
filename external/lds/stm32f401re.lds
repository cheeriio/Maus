/* Skrypt linkera dla STM32F401RE, 512 KiB FLASH, 96 KiB RAM */

MEMORY
{
    FLASH  (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM    (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
    DMAMEM (rw)  : ORIGIN = 0x20040000, LENGTH = 0K
}

/* Linker zgłosi błąd, gdy w RAM zostanie za mało miejsca
   na stos i stertę. */
_minimum_stack_and_heap_size = 8192;

INCLUDE cortex-m.lds
