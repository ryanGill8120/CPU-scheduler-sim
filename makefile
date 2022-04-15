CC = gcc
COPS = -D_GNU_SOURCE -g

all: schedulerSim

schedulerSim: SchedulerSim.c pslibrary.h pslibrary.c
	$(CC) $(COPS) -o schedulerSim SchedulerSim.c pslibrary.c
clean:
	rm runall assign2
