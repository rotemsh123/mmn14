assembler:	mmn14.o preasembler.o trans.o unversalfunction.o
	gcc -ansi -Wall -pedantic mmn14.o preasembler.o trans.o unversalfunction.o -o assembler
mmn14.o:	mmn14.c  mmn14.h
	gcc -c -ansi -Wall -pedantic mmn14.c -o mmn14.o
preasembler.o:	preasembler.c mmn14.h
	gcc -c -ansi -Wall -pedantic preasembler.c -o preasembler.o
trans.o:	trans.c mmn14.h
	gcc -c -ansi -Wall -pedantic trans.c -o trans.o
unversalfunction.o:	unversalfunction.c mmn14.h
	gcc -c -ansi -Wall -pedantic unversalfunction.c -o unversalfunction.o
clean:
	rm *.o assembler *.am
