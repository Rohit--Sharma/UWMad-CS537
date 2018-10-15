CC = gcc
WFLAGS = -Wall -Wextra
EXE = prod_consumer
SCAN_BUILD_DIR = scan-build-out

$(EXE): queue.o thread.o producer_consumer_driver.o
	$(CC) $(WFLAGS) -o $(EXE) queue.o thread.o producer_consumer_driver.o -lpthread

queue.o: queue.c producer_consumer_header.h
	$(CC) $(WFLAGS) -c queue.c

thread.o: thread.c producer_consumer_header.h
	$(CC) $(WFLAGS) -c thread.c

proc_id_processor.o: proc_id_processor.c producer_consumer_header.h
	$(CC) $(WFLAGS) -c proc_id_processor.c

producer_consumer_driver.o: producer_consumer_driver.c producer_consumer_header.h
	$(CC) $(WFLAGS) -c producer_consumer_driver.c

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
