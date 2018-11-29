CC=gcc
WARNING_FLAGS=-Wall -Wextra -pedantic
EXE=output
# LIBS is placed at the end of gcc's linking stage (after all .o files) so it links the necessary library functions (like pthread) to your code
LIBS=-lpthread # if needed, add more libraries here
SCAN_BUILD_DIR=scan-build-out

all: main.o 537malloc.o redblack_tree.o
	$(CC) -o $(EXE) main.o 537malloc.o redblack_tree.o

# main.c is your testcase file name
main.o: main.c
	$(CC) $(WARNING_FLAGS) -c main.c

# Include all your .o files in the below rule
obj: 537malloc.o range_tree.o

537malloc.o: 537malloc.c 537malloc.h redblack_tree.o
	$(CC) $(WARNING_FLAGS) -g -O0 -c 537malloc.c redblack_tree.o

redblack_tree.o: redblack_tree.c redblack_tree.h
	$(CC) $(WARNING_FLAGS) -g -O0 -c redblack_tree.c

# the -f flag for rm ensures that clean doesn't fail if file to be deleted doesn't exist
clean:
	rm -f *.o $(EXE)
	rm -f $(SCAN_BUILD_DIR)

#Run the Clang Static Analyzer
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

#View scan availble using firefox
scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html

# recompile runs clean and then makes everything again to generate executable
# this is equivalent to running "make clean" followed by "make"
recompile: clean $(EXE)
