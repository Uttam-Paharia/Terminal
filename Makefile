all: terminal.o ls.o cp.o  grep.o ps.o
	gcc  terminal.o ls.o cp.o grep.o ps.o -lreadline -o myshell && rm *.o
terminal.o: terminal.c 
	gcc -c  terminal.c
ls.o: ls.c
	gcc -c ls.c
cp.o: cp.c
	gcc -c cp.c
grep.o: grep.c
	gcc -c grep.c
ps.o: ps.c
	gcc -c ps.c
