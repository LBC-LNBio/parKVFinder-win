parKVFinder-win64: dictionaryprocessing.o matrixprocessing.o pdbprocessing.o argparser.o tomlprocessing.o resultsprocessing.o move src/parKVFinder.c
	gcc -fopenmp -Isrc -g -o parKVFinder-win64.exe lib/dictionaryprocessing.o lib/matrixprocessing.o lib/pdbprocessing.o lib/argparser.o lib/tomlprocessing.o lib/resultsprocessing.o src/parKVFinder.c -lm -static

matrixprocessing.o: src/matrixprocessing.c src/matrixprocessing.h
	gcc -fopenmp -O3 -Isrc -c src/matrixprocessing.c -lm -static

dictionaryprocessing.o: src/dictionaryprocessing.c src/dictionaryprocessing.h
	gcc -Isrc -c src/dictionaryprocessing.c

pdbprocessing.o: src/pdbprocessing.c src/pdbprocessing.h
	gcc -Isrc -c src/pdbprocessing.c

argparser.o: src/argparser.c src/argparser.h
	gcc -Isrc -c src/argparser.c

tomlprocessing.o: src/tomlprocessing.c src/tomlprocessing.h
	gcc -Isrc -c src/tomlprocessing.c

resultsprocessing.o: src/resultsprocessing.c src/resultsprocessing.h
	gcc -Isrc -c src/resultsprocessing.c

move: dictionaryprocessing.o matrixprocessing.o pdbprocessing.o argparser.o tomlprocessing.o resultsprocessing.o
	mv dictionaryprocessing.o matrixprocessing.o pdbprocessing.o argparser.o tomlprocessing.o resultsprocessing.o lib/

clean:
	rm -r lib/matrixprocessing.o lib/dictionaryprocessing.o lib/pdbprocessing.o lib/argparser.o lib/tomlprocessing.o lib/resultsprocessing.o parKVFinder-win64.exe
