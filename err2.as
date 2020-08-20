;errors in second move
MAIN: add LIST, r3
K: add r2, r3
.entry LIST
.entry K MAIN
bne &KL
.extern KL
