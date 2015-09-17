How to run & debug vybrid using JLinkGDBServer

1)  Run "JLinkGDBServer.exe -select USB -device VF6xx_A5 -if JTAG -speed auto"
2)  Run gdb client: "arm-none-eabi-gdb.exe –x <script_file>.gdbinit"
3)  Run one of LOAD_XXXXX functions defined in <script_file>.gdbinit,
    using application path as argument.
        f.i "LOAD_INTRAM app/path/intram_debug/hello.elf"
4)  Type "continue" to run application

