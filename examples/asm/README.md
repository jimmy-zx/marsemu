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
+----------------+ <- 0($fp)
| dynamic area   |
+----------------+ <- 0($sp)
```

The stack size is at least 24.

- Example:
```asm
    .globl  func
    .ent    func
func:
    .frame  $fp,24,$ra  # this is necessary to allow backtracing
    addi   $sp,$sp,-24
    sw      $ra,20($sp)
    sw      $fp,16($sp)
    move    $fp,$sp

    move    $sp,$fp
    lw  $ra,20($sp)
    lw  $fp,16($sp)
    addi   $sp,$sp,24
    .end    func
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
