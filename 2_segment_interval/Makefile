CC=g++
CFLAGS=-fdiagnostics-color=always -O3
LIBS=

all: segment interval

segment: segment.cpp
	$(CC) $(CFLAGS) -g segment.cpp -o segment.out $(LIBS)

interval: interval.cpp
	$(CC) $(CFLAGS) -g interval.cpp -o interval.out $(LIBS)

cleanSegment:
	rm -f segment.out

cleanInterval:
	rm -f interval.out

clean: cleanSegment cleanInterval