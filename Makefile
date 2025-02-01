EXECUTABLE = cpuminer
FLAGS = -lcrypto -O3 -static -D COLORS 
OPENSSL = libs/openssl-1.1.1k/ 

CC = gcc

all: main 

main:
	$(CC) main.c $(FLAGS) -L $(OPENSSL) -I $(OPENSSL)/include/ -o $(EXECUTABLE)

clean:
	rm -f *.o $(EXECUTABLE) $(EXECUTABLE)-arm
