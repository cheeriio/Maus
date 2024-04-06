/* Skrypt linkera dla STM32F207IG, 1 MiB FLASH, 112+16 KiB RAM */

MEMORY
{
    FLASH  (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
    SRAM1  (rwx) : ORIGIN = 0x20000000, LENGTH = 112K
    SRAM2  (rw)  : ORIGIN = 0x2001C000, LENGTH = 16K
    RAM    (rw)  : ORIGIN = 0x20000000, LENGTH = 128K
    DMAMEM (rw)  : ORIGIN = 0x20040000, LENGTH = 0K
}

/* Linker zgłosi błąd, gdy w RAM zostanie za mało miejsca
   na stos i stertę. */
_minimum_stack_and_heap_size = 16384;

INCLUDE cortex-m3.lds
