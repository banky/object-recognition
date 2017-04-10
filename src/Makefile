OBJS = CSVRow.o ImageUtils.o train.o matrix.o main.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LIBS = -lcurl `pkg-config --cflags --libs opencv`
LFLAGS = -Wall $(DEBUG) $(LIBS)

main: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o main

main.o: train.h CSVRow.h ImageUtils.h main.cpp
	$(CC) $(CFLAGS) main.cpp

CSVRow.o: CSVRow.h CSVRow.cpp
	$(CC) $(CFLAGS) CSVRow.cpp

ImageUtils.o: ImageUtils.h ImageUtils.cpp
	$(CC) $(CFLAGS) ImageUtils.cpp

train.o: train.h matrix.h train.cpp
	$(CC) $(CFLAGS) train.cpp

matrix.o: matrix.h matrix.cpp
	$(CC) $(CFLAGS) matrix.cpp

clean:
	\rm *.o main
