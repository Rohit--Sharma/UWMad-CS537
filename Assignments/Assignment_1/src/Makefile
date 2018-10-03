CC = gcc
WFLAGS = -Wall -Wextra
EXE = 537ps
SCAN_BUILD_DIR = scan-build-out

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
        rm -f $(SCAN_BUILD_DIR)

#Run the Clang Static Analyzer
scan-build: clean
        scan-build -o $(SCAN_BUILD_DIR) make

#View scan availble using firefox

scan-view: scan-build
        firefox -new-window $(SCAN_BUILD_DIR)/*/index.html

recompile: clean $(EXE)
