CC = cc -std=c11
CFLAGS = -Wall -Werror

OBJFILES = trove.o build.o remove.o find.o utils.o
TARGET = trove
HEADERS = trove.h

$(TARGET): $(OBJFILES) 
			$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) 

%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJFILES) $(TARGET)

run: $(TARGET)
