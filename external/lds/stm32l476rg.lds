/* Skrypt linkera dla STM32F476RG 1024 KiB FLASH, 128 KiB RAM */

MEMORY
{
    FLASH  (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
    SRAM1  (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
    SRAM2  (rw)  : ORIGIN = 0x10000000, LENGTH = 16K
    RAM    (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
    DMAMEM (rw)  : ORIGIN = 0x20000000, LENGTH = 0K
}

/* Linker zgłosi błąd, gdy w RAM zostanie za mało miejsca
   na stos i stertę. */
_minimum_stack_and_heap_size = 8192;

INCLUDE cortex-m.lds
