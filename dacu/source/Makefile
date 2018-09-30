# Source, Executable, Includes, Library Defines
INCL   = 
SRC    = main.c com.c led.c debug.c ptime.c loud.c chip_pca.c net_event.c net.c event.c
OBJ    = $(SRC:.c=.o)
LIBS   = 
EXE    = a.out

# Compiler, Linker Defines
CC      = /usr/bin/gcc
#CFLAGS  = -ansi -pedantic -Wall -O2
# -w for close warning
CFLAGS  =  -w	
LIBPATH = -L.
LDFLAGS = -o $(EXE) $(LIBPATH) $(LIBS)
CFDEBUG = -ansi -pedantic -Wall -g -DDEBUG $(LDFLAGS)
RM      = /bin/rm -f

# Compile and Assemble C Source Files into Object Files
%.o: %.c
	$(CC) -c $(CFLAGS) $*.c

# Link all Object Files with external Libraries into Binaries
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ)

# Objects depend on these Libraries
$(OBJ): $(INCL)

# Create a gdb/dbx Capable Executable with DEBUG flags turned on
debug:
	$(CC) $(CFDEBUG) $(SRC)

# Clean Up Objects, Exectuables, Dumps out of source directory
clean:
	$(RM) $(OBJ) $(EXE)
