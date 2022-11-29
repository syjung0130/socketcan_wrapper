INC_DIR=./
CFLAGS=-O4 -I=${INC_DIR}
OBJECTS=main.o socketcan_wrapper.o

all: main

# if you did not use '\', each command runs in seperated shell.
# if you use command in same shell, must use '\'.
# to separate each command, add ';' to the end of command.
main: $(OBJECTS)
	@gcc -o $@ ${CFLAGS} main.o socketcan_wrapper.o; \
	echo "build out complete !"

%.o: %.c
	@gcc -c ${CFLAGS} $<; \
	echo "build c source.."

clean:
	rm -rf *.o; \
	rm -rf main
