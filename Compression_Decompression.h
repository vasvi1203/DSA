#include<stdio.h>

//A Huffman tree node
typedef struct MinHeapNode {
    char data;  
    unsigned freq; 
    struct MinHeapNode *left, *right;
} MinHeapNode;
  
//A Min Heap : Collection of min-heap (or Huffman tree) nodes
typedef struct MinHeap {
    	unsigned size;
	unsigned capacity;
    	MinHeapNode **array;
} MinHeap;


typedef struct node {
	char data;
	char *code;
	struct node *next;
} node;


typedef node* table;

typedef struct lnode {
	char data;
	struct lnode *next;
} lnode;

typedef lnode* list;

void initlist(list *l);
void insert(list *l, char c);
void traverse(list *l);

void init(table *a);
MinHeapNode* HuffmanCodes(char *data, int *freq, int size, table *a);
MinHeapNode* buildHuffmanTree(char *data, int *freq, int size);
MinHeap* createAndBuildMinHeap(char *data, int *freq, int size);
MinHeap* createMinHeap(unsigned capacity);
void buildMinHeap(MinHeap *minHeap);
void minHeapify(MinHeap *minHeap, int idx);
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b);
MinHeapNode* newNode(char data, unsigned freq);
int isSizeOne(MinHeap *minHeap);
MinHeapNode* extractMin(MinHeap *minHeap);
void insertMinHeap(MinHeap *minHeap, MinHeapNode *minHeapNode);
void printCodes(MinHeapNode* root, int *arr, int top, table *a);
void codetable(table *a, char c, char *arr);
void printArr(int *arr, int n);
int isLeaf(MinHeapNode *root);
void compressfileHuffman(int fd1, int fd2, table *a);
void decompressfileHuffman(int fd2, int fd3, MinHeapNode *tree);

void compressfileRLE(char* src, int fd2);
void decompressfileRLE(list *l, int fd3);







