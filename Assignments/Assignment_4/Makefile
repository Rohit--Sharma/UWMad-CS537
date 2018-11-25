CC=gcc
WARNING_FLAGS=-Wall -Wextra -pedantic
EXE=537malloc.o
# LIBS is placed at the end of gcc's linking stage (after all .o files) so it links the necessary library functions (like pthread) to your code
LIBS=-lpthread # if needed, add more libraries here
SCAN_BUILD_DIR = scan-build-out

# TODO: Recheck make file
# the -g flag at all gcc compilation stages ensures that you can use gdb to debug your code
$(EXE): 537malloc.c 537malloc.h redblack_tree.o
	$(CC) -g $(WARNING_FLAGS) -O0 -c 537malloc.c redblack_tree.o -o $(EXE)
redblack_tree.o: redblack_tree.c redblack_tree.h
	$(CC) -g $(WARNING_FLAGS) -c redblack_tree.c

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
