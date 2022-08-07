MAIN: .entry LOOP
.entry LENGTH
.extern L3
.extern W
MAIN: mov S1.1, W
adr r2,STR
LOOP: jmp W,,,,
prn #-5
sub r1    
inc #r
mov S1.2, #3
mov
bne L3
tyu 66
LOOP: hlt
.extern L3
;

;sdsgdsf

STR: .string "abcdef" "ggg"
STR2: .string "abcdef
LENGTH: .data ttt,-9,15
K: .data 22
S1: .struct 8, 7
S2: .struct