#ifndef TOPK_H
#define TOPK_H

#include "node.h"
#include <string>
using namespace std;

class topK {
	private:
		//Fields relating to the minHeap
		node* root;
		int capacity;
		int currentSize;

		//Fields relating to the hashTable
		hashNode table[];
	public:
		topK(int c){
			capacity = c;
			//one extra spot, we don't use 0 here.
			node* root = new node[c + 1];
			currentSize = 0;
			table[4*c];
			
		}

};

#endif
