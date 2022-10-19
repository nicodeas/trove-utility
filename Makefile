CC 		 	= cc -std=c11
CFLAGS 	= -Wall -Werror
OBJFILES = trove.o globals.o build.o remove.o update.o find.o
HEADER = trove.h 
TARGET = trove


$(TARGET): $(HEADER) $(OBJFILES) 
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) 

%.o:	%.c %.h
		$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJFILES) $(TARGET)