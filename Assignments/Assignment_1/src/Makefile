CC = gcc
WFLAGS = -Wall -Wextra
EXE = 537ps

$(EXE): options_processor.o proc_files_parser.o proc_id_processor.o 537ps_driver.o
	$(CC) $(WFLAGS) -o $(EXE) options_processor.o proc_files_parser.o proc_id_processor.o 537ps_driver.o

options_processor.o: options_processor.c 537ps_header.h
	$(CC) $(WFLAGS) -c options_processor.c

proc_files_parser.o: proc_files_parser.c 537ps_header.h
	$(CC) $(WFLAGS) -c proc_files_parser.c

proc_id_processor.o: proc_id_processor.c 537ps_header.h
	$(CC) $(WFLAGS) -c proc_id_processor.c

537ps_driver.o: 537ps_driver.c 537ps_header.h
	$(CC) $(WFLAGS) -c 537ps_driver.c

clean:
	rm -f *.o $(EXE)

recompile: clean $(EXE)

