LABEL:          .data  1
LABEL1:          .data  -1       , +2  ,    -3,   -4,
LABEL2:          .data  ,6 6 ,,7,
        .string "abcde"
LABEL:    .string "  a  b      c               d       e       "
bad123:    .string   a  b      c               d       e       "
LABEL123: .data 1 ,3
CommaAfterString:    .string "  a  b      c               d       e       " ,
    LABEL132:          add  1 lab1
ade r1 r2
bne &XYZ
END: stop
sub r1 r4
cmp
dec K
jmp &LOOP

