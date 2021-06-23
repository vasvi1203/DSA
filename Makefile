bin_PROGRAMS = project
project_SOURCES = Main.c Compression_Decompression.c Compression_Decompression.h

project: Main.o Compression_Decompression.o
	gcc Main.o Compression_Decompression.o -o project

Main.o: Main.c
	gcc -c Main.c

Compression_Decompression.o: Compression_Decompression.c Compression_Decompression.h
	gcc -c Compression_Decompression.c

clean:
	rm *.o project
