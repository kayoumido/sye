 
-include $(USR_DIR)/Makefile.common

LIB = write read getpid close pipe dup2 dup sleep usleep

all: $(patsubst %,%.o,$(LIB))


clean:
	rm -rf *.o
