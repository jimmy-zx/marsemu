# MIPS ASM

## Calling convention

```
Top of stack
+----------------+ <- old $sp
| $ra            |
| old $fp        |
| ...            |
+----------------+ <- $16($fp)
| reserved       |
+----------------+ <- 0($fp), 0($sp)
```

The stack size is at least 24.

- Example:
```asm
func:
    # function prologue
    addi    $sp,$sp,-24   # allocate a stack of 24 bytes
    sw      $fp,16($sp)   # save old $fp
    move    $fp,$sp       # get the new $fp
    sw      $ra,20($fp)   # save $ra

    # function epilogue
    lw      $ra,20($fp)   # restore $ra
    lw      $fp,16($fp)   # restore old $fp
    addi    $sp,$sp,24    # restore $sp
    ja      $ra           # return
```

See [inc.s](/examples/asm/inc.s)

## Macros

GNU AS:
```
.macro (name)
...
.endm
```

MARS:
```
.macro (name)
...
.end_macro
```

## References
- [1](https://www.cs.purdue.edu/homes/hosking/502/spim/node23.html)
- [2](https://refspecs.linuxfoundation.org/elf/mipsabi.pdf)
