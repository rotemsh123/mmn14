assembler:	mmn14.o preasembler.c trans.c
	gcc -ansi -Wall -pedantic mmn14.o preasembler.c trans.c -o assembler
mmn14.o:	mmn14.c mmn14.h
	gcc -c -ansi -Wall -pedantic mmn14.c -o mmn14.o
preasembler.o:	preasembler.c mmn14.h
	gcc -c -ansi -Wall -pedantic preasembler.c -o preasembler.o
trans.o:	trans.c mmn14.h
	gcc -c -ansi -Wall -pedantic trans.c -o trans.o
