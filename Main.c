#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>

#include "Compression_Decompression.h"

int main(int argc, char *argv[]) {
	char d = ' ';
	if(argc < 4) {
		errno = EINVAL;
		printf("Please run in any one of the following formats:\n./project -c1 <file> <compressed-file-name>\n./project -c2 <file> <compressed-file-name>\n./project -uc1 <compressed-file> <uncompressed-file>\n./project -uc2 <compressed-file> <uncompressed-file>\n");
		return errno;
	}

	if(!strcmp(argv[1], "-c1")) {
		char arr[128], code[128], c, swap, count[500];
		int fd1, fd2, i, j, freq[128], f[128], k = 0, size, temp, l, s = 0; 
		table codes;
		MinHeapNode *tree;
		init(&codes);
		fd1 = open(argv[2], O_RDONLY);
		if(fd1 == -1) {
			perror("File not found\n");
			return errno;
		}	
		
		fd2 = open(argv[3], O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
		if(fd2 == -1) {
			perror("File not found\n");
			return errno;
		}

		for(i = 0; i < 127; i++) {
			arr[i] = i;
			f[i] = 0;
		}

		

		while(read(fd1, &c, sizeof(char))) {
			++f[c - 0];
		}

		for(j = 0; j < 127; j++) {
			if(f[j] != 0) {
				freq[k] = f[j];
				code[k] = arr[j];
				k++;
			}
		}


		size = k;
		printf("%d\n", size);
	
		for(i = 0; i < size; i++) {
			j = i;
			while(j > 0 && (freq[j - 1] > freq[j])) {
				temp = freq[j];
				swap = code[j];
				freq[j] = freq[j - 1];
				code[j] = code[j - 1];
				freq[j - 1] = temp;
				code[j - 1] = swap;
				j--;
			}
		}
	
		
		for(k = 0; k < size; k++) {
			printf("%c : %d\n", code[k], freq[k]);
			s++;
			sprintf(count, "%d", freq[k]);
		
			for (l = 0; *(count + l); l++) { 
				s++; 
			} 
			s++;
		} 
		printf("%d\n", s);
		write(fd2, &s, sizeof(int));
		
		write(fd2, &d, 1);
	
		for(k = 0; k < size; k++) {
			write(fd2, &code[k], 1);
			sprintf(count, "%d", freq[k]);
			for (l = 0; *(count + l); l++) { 
				write(fd2, &count[l], 1); 
			} 
			write(fd2, &d, 1);
		}	

		tree = HuffmanCodes(code, freq, size, &codes);
		
		lseek(fd1, 0, SEEK_SET);
		compressfileHuffman(fd1, fd2, &codes);
		close(fd1);
		close(fd2);
	}
	
	
	

	if(!strcmp(argv[1], "-uc1")) {
		int fd2, fd3, k = 0, freq[128], size;
		table codes;
		list l;
		initlist(&l);
		MinHeapNode *tree;
		init(&codes);
		char code[128], c;
		fd2 = open(argv[2], O_RDONLY, S_IWUSR | S_IRUSR);
		if(fd2 == -1) {
			perror("File not found\n");
			return errno;
		}

		read(fd2, &size, sizeof(int));
		printf("%d\n", size);
		read(fd2, &c, 1);
		 
		while(k < (size - 1)) {
			read(fd2, &c, 1);
			insert(&l, c);		
			k++;
		}
		//traverse(&l);
		read(fd2, &c, 1);
		
		
		list ptr, tmp;
		ptr = l;
		int count = 0;
		k = 0;
		if (ptr == NULL) {
			printf("List is empty\n");
		}
	
		else {
			while (ptr) {
				count = 0;
				if(ptr && ptr->data == ' ') {
					ptr = ptr->next;
				}
				c = ptr->data;
				code[k] = c;
				printf("%c : ", code[k]);
				if(ptr->next) {
					tmp = ptr->next;
					if(tmp && tmp->data >= '0' && tmp->data <= '9') {
						while(tmp && tmp->data >= '0' && tmp->data <= '9') {
							count = count * 10 + (tmp -> data - '0');
							tmp = tmp -> next;
						}
						ptr = tmp;
					}
				}
			freq[k] = count;
			printf("%d\n", freq[k]);
			k++;
			}
		}
		printf("%d\n", k);
	
		tree = HuffmanCodes(code, freq, k, &codes);
			
		fd3 = open(argv[3], O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR);
		if(fd3 == -1) {
			perror("File not found\n");
			return errno;
		}
		decompressfileHuffman(fd2, fd3, tree);
		close(fd2);
		close(fd3);
	}

	if(!strcmp(argv[1], "-c2")) {
		int fd1, fd2, i = 0;
		char c, *str;
		fd1 = open(argv[2], O_RDONLY);
		if(fd1 == -1) {
			perror("File not found\n");
			return errno;
		}	
		
		fd2 = open(argv[3], O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
		if(fd2 == -1) {
			perror("File not found\n");
			return errno;
		}

		while(read(fd1, &c, 1)) {
			i++;
		}

		str = (char*)malloc(sizeof(char) * i + 1);
		lseek(fd1, 0, SEEK_SET);
		i = 0;
		while(read(fd1, &c, 1)) {
			str[i] = c;
			i++;
		}
		str[i] = '\0';
		compressfileRLE(str, fd2); 
		close(fd1);
		close(fd2);
	}


	if(!strcmp(argv[1], "-uc2")) {
		int fd2, fd3, i = 0;
		char c;
		list a;
		initlist(&a);
		fd2 = open(argv[2], O_RDONLY);
		if(fd2 == -1) {
			perror("File not found\n");
			return errno;
		}	
		
		fd3 = open(argv[3], O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR);
		if(fd3 == -1) {
			perror("File not found\n");
			return errno;
		}

		while(read(fd2, &c, 1)) {
			insert(&a, c);	
			i++;
		}
		traverse(&a);
		decompressfileRLE(&a, fd3);
		close(fd2);
		close(fd3);
	}
	
    	return 0;
}

