scheduler: main.c process.c process.h
	gcc main.c process.c -o scheduler
clean: rm -f scheduler
