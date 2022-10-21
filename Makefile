CC 		 	= cc -std=c11
CFLAGS 	= -Wall -Werror
OBJFILES = trove.o globals.o build.o remove.o find.o
HEADERS = trovelib.h globals.h
TARGET = trove


$(TARGET): $(HEADERS) $(OBJFILES) 
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) 

%.o:	%.c
		$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJFILES) $(TARGET)