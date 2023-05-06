CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: emulate

emulate: emulate.o pipeline.o print_state.o check_condition_field.o data_processing.o multiply.o single_data_transfer.o branch.o
      $(CC) emulate.o pipeline.o print_state.o check_condition_field.o data_processing.o multiply.o single_data_transfer.o branch.o -o emulate

emulate.o: emulate.c 
      $(CC) $(CFLAGS) emulate.c -c -o emulate.o

pipeline.o: pipeline.h pipeline.c
      $(CC) $(CFLAGS) pipeline.c -c -o pipeline.o

print_state.o: print_state.h print_state.c
      $(CC) $(CFLAGS) print.c -c -o print_state.o

check_condition_field.o: check_condition_h check_condition_field.c
      $(CC) $(CFLAGS) check_condition_field.c -c -o check_condition_field.o

data_processing.o: data_processing.h data_processing.c
      $(CC) $(CFLAGS) data_processing.c -c -o data_processing.o

multiply.o: multiply.h multiply.c
      $(CC) $(CFLAGS) multiply.c -c -o multiply.o

single_data_transfer.o: single_data_transfer.h single_data_transfer.c
      $(CC) $(CFLAGS) singleDataTransfer.c -c -o single_data_transfer.o

branch.o: branch.h branch.c
      $(CC) $(CFLAGS) branch.c -c -o branch.o

clean:
   rm -f $(wildcard *.o)
   rm -f emulate