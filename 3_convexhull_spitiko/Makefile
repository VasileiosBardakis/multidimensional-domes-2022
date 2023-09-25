CC=g++
CFLAGS=-fdiagnostics-color=always -O3
LIBS=

all: convex benchmark

convex: convexhull.cpp
	$(CC) $(CFLAGS) -g convexhull.cpp -o convexhull.out $(LIBS)

benchmark: convexhull_benchmark.cpp
	$(CC) $(CFLAGS) -g convexhull_benchmark.cpp -o benchmark.out $(LIBS)

cleanConvex:
	rm -f convexhull.out

cleanBenchmark:
	rm -f benchmark.out

cleanCSV:
	rm -f convex.csv points.csv

clean: cleanConvex cleanBenchmark