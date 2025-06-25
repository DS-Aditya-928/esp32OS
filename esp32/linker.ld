ENTRY( call_start_cpu0 );

/* Specify main memory areas */
MEMORY
{
  /* Use values from the ESP-IDF 'bootloader' component.
  /* TODO: Use human-readable lengths */
  /* TODO: Use the full memory map - this is just a test */
  iram_seg ( RWX )       : ORIGIN = 0x40080400, len = 0x20000
  dram_seg ( RW )       : ORIGIN = 0x3FFF0000, len = 0x10000
}

/* Define output sections */
SECTIONS 
{
  /DISCARD/ : {
    *(.eh_frame)
    *(.eh_frame_hdr)
  }
  .iram.text : ALIGN(4)
  {
    . = ALIGN(16);
    KEEP(*(.entry.text))
    *(.text)
    *(.text*)
    KEEP (*(.init))
    KEEP (*(.fini))
    

    . = ALIGN(4);
    _etext = .;
  } >iram_seg

  _sidata = .;
  .data : AT(_sidata)
  {
    . = ALIGN(4);
    _sdata = .;
    PROVIDE(_sdata = .);
    *(.data)
    *(.data*)

    *(.rodata)
    *(.rodata*)
    
    . = ALIGN(4);
    _edata = .;
    PROVIDE(_edata = .);
  } >dram_seg

  /* Uninitialized data also goes into Data RAM */
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;
    PROVIDE(_ebss = .);
  } >dram_seg

  . = ALIGN(4);
  PROVIDE ( end = . );
  PROVIDE ( _end = . );
}