CC = g++
CFLAGS = -std=c++17 -Wall 
DEPS = header.h 
OBJ = print_dir_ent.o cmd_mode.o print_dir.o main.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *o main