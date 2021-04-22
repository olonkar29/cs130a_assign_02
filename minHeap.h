#ifndef MINHEAP_H
#define MINHEAP_H
#include "node.h"
class minHeap{
	private:
		int size;
		node* root;

	public:
		minHeap(int s) {root = new node[s]; size = s;}
};
#endif
