CC 		 	= cc -std=c11
CFLAGS 	= -Wall -Werror
OBJFILES = trove.o build.o remove.o update.o find.o
HEADERS = trove.h
TARGET = trove


$(TARGET): $(HEADERS) $(OBJFILES) 
			$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) 

trove: $(HEADERS) $(OBJFILES)
			$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

trove.o: trove.c trove.h
			$(CC) $(CFLAGS) -c trove.c

build.o: build.c build.h
			$(CC) $(CFLAGS) -c build.c

update.o: update.c update.h
			$(CC) $(CFLAGS) -c update.c

remove.o: remove.c remove.h
			$(CC) $(CFLAGS) -c remove.c

find.o: find.c find.h
			$(CC) $(CFLAGS) -c find.c

clean:
	rm -f $(OBJFILES) $(TARGET)