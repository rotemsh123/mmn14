MAIN: mov S1.1 ,LENGTH
add r2,STR
MAIN: jmp END
macro m1
inc K
mov S1.2 ,r3
endmacro
jsr #-5
sub #5, r4
m1
bne LOOP
END: hlt
STR: .string "abcdef"
LENGTH: .data 16,-9,-16
K: .data 22
S1: .struct3 8, "ab"

  