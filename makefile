assembler:	mmn14.o preasembler.o trans.o unversalfunction.o order.o instructions.o asembler.o operandes.o
	gcc -ansi -Wall -pedantic mmn14.o preasembler.o trans.o unversalfunction.o order.o instructions.o asembler.o operandes.o -o assembler
mmn14.o:	mmn14.c  mmn14.h
	gcc -c -ansi -Wall -pedantic mmn14.c -o mmn14.o
preasembler.o:	preasembler.c mmn14.h
	gcc -c -ansi -Wall -pedantic preasembler.c -o preasembler.o
trans.o:	trans.c mmn14.h
	gcc -c -ansi -Wall -pedantic trans.c -o trans.o
unversalfunction.o:	unversalfunction.c mmn14.h
	gcc -c -ansi -Wall -pedantic unversalfunction.c -o unversalfunction.o
order.o:	order.c mmn14.h
	gcc -c -ansi -Wall -pedantic order.c -o order.o
instructions.o:	instructions.c mmn14.h
	gcc -c -ansi -Wall -pedantic instructions.c -o instructions.o
asembler.o:	asembler.c mmn14.h
	gcc -c -ansi -Wall -pedantic asembler.c -o asembler.o
operandes.o:	operandes.c mmn14.h
	gcc -c -ansi -Wall -pedantic operandes.c -o operandes.o
clean:
	rm *.o assembler *.am
