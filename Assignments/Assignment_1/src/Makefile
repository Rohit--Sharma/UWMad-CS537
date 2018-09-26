CC = gcc
WFLAGS = -Wall -Wextra
EXE = 537ps

$(EXE): options_processing.o stat_parser.o process_ids.o
	$(CC) $(WFLAGS) -o $(EXE) options_processing.o stat_parser.o process_ids.o

options_processing.o: options_processing.c
	$(CC) $(WFLAGS) -c options_processing.c

stat_parser.o: stat_parser.c
	$(CC) $(WFLAGS) -c stat_parser.c

process_ids.o: process_ids.c
	$(CC) $(WFLAGS) -c process_ids.c

clean:
	rm -f *.o $(EXE)

recompile: clean $(EXE)

