mymake.o:mymake.c
	gcc -Wall -g -c mymake.c -o mymake.o
depend.o:depend.c
	gcc -Wall -g -c depend.c -o depend.o
mymake2:mymake.o depend.o
	gcc depend.o mymake.o -o mymake2
.PHONY:clean
clean:
	rm -f *.o
