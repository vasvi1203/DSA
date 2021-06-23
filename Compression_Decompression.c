#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>

#include "Compression_Decompression.h"
#define MAX_TREE_HT 130

void init(table *a) {
	*a = NULL;
}


void initlist(list *l) {
	*l = NULL;
}

void insert(list *l, char c) {
	list ptr;
	lnode *newnode = (lnode*)malloc(sizeof(lnode));
	newnode->data = c;
	newnode->next = NULL;
	if(*l == NULL) {
		*l = newnode;
		return;
	}
	ptr = *l;
	while(ptr->next != NULL) {
		ptr = ptr->next;
	}
	ptr->next = newnode;
}

void traverse(list *l) {
	list ptr;
	ptr = *l;
	if(ptr == NULL) {
		return;
	}
	while(ptr) {
		printf("%c\n", ptr->data);
		ptr = ptr->next;
	}
}


// The main function that builds a Huffman Tree and print codes by traversing the built Huffman Tree
MinHeapNode* HuffmanCodes(char *data, int *freq, int size, table *a) {
	int arr[MAX_TREE_HT], top;
	init(a);
	/* Construct Huffman Tree */
	MinHeapNode *root = buildHuffmanTree(data, freq, size);
  
	/* Print Huffman codes using the Huffman tree built above */
	top = 0;
	printCodes(root, arr, top, a);
	return root;
}


// The main function that builds Huffman tree
MinHeapNode* buildHuffmanTree(char *data, int *freq, int size) {
	MinHeapNode *left, *right, *top;
  
	/* Step 1: Create a min heap of capacity equal to size. Initially, there are nodes equal to size */
	MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);
  
	/* Iterate while size of heap doesn't become 1 */
	while(!isSizeOne(minHeap)) {
  
        	/* Step 2: Extract the two minimum freq items from min heap */
      		left = extractMin(minHeap);
        	right = extractMin(minHeap);
  
        	/* Step 3:  Create a new internal node with frequency equal to the sum of the two nodes frequencies. Make the two extracted node as left and right children of this new node. Add this node to the min heap '$' is a special value for internal nodes, not used */
        	top = newNode('$', left->freq + right->freq);
		top->left = left;
        	top->right = right;
  
        	insertMinHeap(minHeap, top);
    	}
  
    	/* Step 4: The remaining node is the root node and the tree is complete */
    	return extractMin(minHeap);
}


// Creates a min heap of capacity equal to size and inserts all character of data[] in min heap. Initially size of min heap is equal to capacity
MinHeap* createAndBuildMinHeap(char *data, int *freq, int size) {
	int i;
	MinHeap *minHeap = createMinHeap(size);
  	for(i = 0; i < size; ++i) {
		minHeap->array[i] = newNode(data[i], freq[i]);
  	}
    	minHeap->size = size;
    	buildMinHeap(minHeap);
  	return minHeap;
}


// A utility function to create a min heap of given capacity
MinHeap* createMinHeap(unsigned capacity) {
	MinHeap *minHeap = (MinHeap*)malloc(sizeof(MinHeap));
	/* current size is 0 */
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = (MinHeapNode**)malloc(minHeap->capacity * sizeof(MinHeapNode*));
	return minHeap;
}


// A standard function to build min heap
void buildMinHeap(MinHeap* minHeap) {
	int n = (minHeap->size) - 1, i;  
	for(i = ((n - 1) / 2); i >= 0; i--) {
        	minHeapify(minHeap, i);
	}
}


// The standard minHeapify function.
void minHeapify(MinHeap* minHeap, int idx) {
	int smallest, left, right;
  	smallest = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;
	if ((left < minHeap->size) && (minHeap->array[left]->freq < minHeap->array[smallest]->freq)) {
		smallest = left;
	}
  
	if ((right < minHeap->size) && (minHeap->array[right]->freq < minHeap->array[smallest]->freq)) {
		smallest = right;
	}
  
	if (smallest != idx) {
        	swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        	minHeapify(minHeap, smallest);
	}
}


// A utility function to swap two min heap nodes
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
	MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}
  

// A utility function to check if size of heap is 1 or not
int isSizeOne(MinHeap* minHeap) {
	return (minHeap->size == 1);
}


// A standard function to extract minimum value node from heap
MinHeapNode* extractMin(MinHeap* minHeap) {
	MinHeapNode* temp = minHeap->array[0];
	minHeap->array[0] = minHeap->array[(minHeap->size) - 1];
	--minHeap->size;
	minHeapify(minHeap, 0);  
	return temp;
}


// A utility function allocate a new min heap node with given character and frequency of the character
MinHeapNode* newNode(char data, unsigned freq) {
	MinHeapNode *temp = (MinHeapNode*)malloc(sizeof(MinHeapNode));
	temp->left = temp->right = NULL;
	temp->data = data;
	temp->freq = freq;
	return temp;
}


// A utility function to insert a new node to Min Heap
void insertMinHeap(MinHeap* minHeap, MinHeapNode* minHeapNode) {
	++minHeap->size;
	int i = minHeap->size - 1;
	while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
		minHeap->array[i] = minHeap->array[(i - 1) / 2];
        	i = (i - 1) / 2;
	}
	minHeap->array[i] = minHeapNode;
}
  

// Prints huffman codes from the root of Huffman Tree. It uses arr[] to store codes
void printCodes(MinHeapNode* root, int *arr, int top, table *a) {
	char *arr1;
	int i;
	
	/* Assign 0 to left edge and recur */
	if (root->left) {
		arr[top] = 0;
        	printCodes(root->left, arr, top + 1, a);
    	}
  
    	/* Assign 1 to right edge and recur */
    	if (root->right) {
  		arr[top] = 1;
        	printCodes(root->right, arr, top + 1, a);
    	}
  
    	/* If this is a leaf node, then it contains one of the input characters, print the character and its code from arr[] */
    	if (isLeaf(root)) {
		printf("%c : ", root->data);
        	printArr(arr, top);
		arr1 = (char*)malloc(sizeof(char) * top);
		for(i = 0; i < top; i++) {
			arr1[i] = arr[i] + '0';
		}
		codetable(a, root->data, arr1);
    	}
}


void codetable(table *a, char c, char* arr) {
	table ptr;
	node *newnode = (node*)malloc(sizeof(node));
	newnode->data = c;
	newnode->code = (char*)malloc(sizeof(arr));
	strcpy(newnode->code, arr);
	newnode->next = NULL;
	ptr = *a;
	if(ptr == NULL) {
		*a = newnode;
	}

	else {
		while(ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = newnode;
	}
}

 

// A utility function to print an array of size n
void printArr(int *arr, int n) {
	int i;
	for (i = 0; i < n; i++) {
		printf("%d", arr[i]);
	}
    	printf("\n");
}



// Utility function to check if this node is leaf
int isLeaf(MinHeapNode* root) {
	return (!(root->left) && !(root->right));
}


void compressfileHuffman(int fd1, int fd2, table *a) {
	char bit, c, d;
	int bitsleft = 8, i = 0, compressedbits = 0, originalbits = 0;
	float ratio, percent;

	bit = '\0';
	table ptr;
	ptr = *a;
	if(!ptr) {
		return;
	}
	while(read(fd1, &c, 1)) {
		originalbits++;
		while(ptr) {
			
			/* compare character with table */
			if(ptr->data == c) {
				
				/* add character by left shifting bits */
				while(i != strlen(ptr->code)) {
					while(i < strlen(ptr->code) && bitsleft != 0) {
						d = ptr->code[i];
						d = d & 1;
						bit = bit | d;
						i++;
						bitsleft--;
						if(bitsleft != 0) {
							bit = bit << 1;
						}	
					}
				
					/* 1 byte */
					if (bitsleft == 0) {
                				write(fd2, &bit, 1);
                				bit = '\0';
                				bitsleft = 8;
            				}
				}	

				i = 0;
			}
		ptr = ptr->next;
		}
	ptr = *a;
	}

	if (bitsleft != 8) {
        	bit = bit << (bitsleft-1);
        	write(fd2, &bit, 1);
    	}

	lseek(fd2, 0, SEEK_SET);
	while(read(fd2, &c, 1)) {
		compressedbits++;
	}

	ratio = (float)originalbits / compressedbits;
	percent = ((float)(originalbits - compressedbits) / originalbits) * 100;
	printf("Original bits = %d\n", originalbits * 8);
	printf("Compressed bits = %d\n", compressedbits * 8);
	printf("Compression ratio = %f\n", ratio);
	printf("Memory saved = %f%%\n", percent);
}


void decompressfileHuffman(int fd2, int fd3, MinHeapNode *tree) {
	char bit, d, a, c;
	int i = 0; 
	MinHeapNode *ptr;
	ptr = tree;

	/* bit = 1000000 */
	bit = 1 << 7;
	while(read(fd2, &c, 1)) {
		while(i < 8) {

			/* find first bit of c and left shift bits of c by 1 */
			d = bit & c;
			c = c << 1;

			/* left traversal */
			if(d == 0) {
				ptr = ptr->left;//printf("left %c\t", ptr->data);
			}

			/* right traversal */
			else {
				ptr = ptr->right;//printf("right %c\t", ptr->data);
			}

			/* character found */
			if(isLeaf(ptr)) {
				a = ptr->data;
				write(fd3, &a, 1);
				ptr = tree;
			}	
			i++;
		}
		i = 0;
	}
}




void compressfileRLE(char* src, int fd2) { 
	int rLen, originalbits, compressedbits;
	float ratio, percent; 
	char count[500], c; 
	int len = strlen(src);

	char* dest = (char*)malloc(sizeof(char) * (len * 2 + 1)); 

	int i, j = 0, k, l = 0; 

	/* traverse the input string one by one */
	for (i = 0; i < len; i++) { 

		/* Copy the first occurrence of the new character */
		dest[j++] = src[i];

		/* Count the number of occurrences of the new character */
		rLen = 1; 
		while (i < len - 1 && src[i] == src[i + 1]) { 
			rLen++; 
			i++; 
		} 
		
		/* Store rLen in a character array count[] */
		sprintf(count, "%d", rLen);
		
		/* Copy the count[] to destination */
		for (k = 0; *(count + k); k++, j++) { 
			dest[j] = count[k]; 
		} 
	} 

	/*terminate the destination string */
	dest[j] = '\0';	
	while(l < strlen(dest)) {
		c = dest[l];
		write(fd2, &c, 1);
		l++;
	}

	lseek(fd2, 0, SEEK_SET);
	while(read(fd2, &c, 1)) {
		compressedbits++;
	}

	originalbits = len;
	ratio = (float)originalbits / compressedbits;
	percent = ((float)(originalbits - compressedbits) / originalbits) * 100;
	printf("Original bits = %d\n", originalbits * 8);
	printf("Compressed bits = %d\n", compressedbits * 8);
	printf("Compression ratio = %f\n", ratio);
	printf("Memory saved = %f%%\n", percent);
} 


void decompressfileRLE(list *l, int fd3) {
	list ptr, tmp;
	ptr = *l;
	char c;
	int count = 0, i;
	if (ptr == NULL) {
		return;
	}

	while (ptr) {
		count = 0;
		c = ptr->data;
		if(ptr->next) {
			tmp = ptr->next;

			/* find the occurence of each character */
			if(tmp && tmp->data >= '0' && tmp->data <= '9') {
				while(tmp && tmp->data >= '0' && tmp->data <= '9') {
					count = count * 10 + (tmp -> data - '0');
					tmp = tmp -> next;
				}
				ptr = tmp;
			}
		}

		/* write the character count times */

		for(i = 0; i < count; i++) {
			write(fd3, &c, 1);
		}
	}
}
				

