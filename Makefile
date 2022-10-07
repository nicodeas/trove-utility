CC 		 	= cc -std=c11
CFLAGS 	= -Wall -Werror
OBJFILES = trove.o build.o
HEADERS = trove.h
TARGET = trove

all: $(TARGET)

$(TARGET): $(HEADERS) $(OBJFILES) 
			$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) 
 
clean:
	rm -f $(OBJFILES) $(TARGET)