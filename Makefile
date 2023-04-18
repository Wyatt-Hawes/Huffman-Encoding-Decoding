CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic #$(shell pkg-config --cflags)

all: encode decode

test: test.o node.o stack.o pq.o code.o io.o huffman.o
	$(CC) -o $@ $^

encode: encode.o node.o stack.o pq.o code.o io.o huffman.o
	$(CC) -o $@ $^

decode: decode.o node.o stack.o pq.o code.o io.o huffman.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	
spotless: clean
	rm -f test encode decode

format:
	clang-format -i -style=file *.[c]
