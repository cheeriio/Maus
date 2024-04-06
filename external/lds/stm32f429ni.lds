/* Skrypt linkera dla STM32F429NI, 2 MiB FLASH, 64+112+16+64 KiB RAM */

MEMORY
{
    FLASH  (rx)  : ORIGIN = 0x08000000, LENGTH = 2048K
    CCRAM  (rw)  : ORIGIN = 0x10000000, LENGTH = 64K
    SRAM1  (rwx) : ORIGIN = 0x20000000, LENGTH = 112K
    SRAM2  (rw)  : ORIGIN = 0x2001C000, LENGTH = 16K
    SRAM3  (rw)  : ORIGIN = 0x20020000, LENGTH = 64K
    RAM    (rw)  : ORIGIN = 0x10000000, LENGTH = 64K
    DMAMEM (rw)  : ORIGIN = 0x20000000, LENGTH = 192K
}

/* Linker zgłosi błąd, gdy w RAM zostanie za mało miejsca
   na stos i stertę. */
_minimum_stack_and_heap_size = 8192;

INCLUDE cortex-m.lds
