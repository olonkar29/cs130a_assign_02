#ifndef TOPK_H
#define TOPK_H

#include "node.h"
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

class topK {
	private:
		//Fields relating to the minHeap
		node* root;
		int capacity;
		int currentSize;
		std::vector<std::string> order; //stores the highest frequency in the minHeap

		//Fields relating to the hashTable
		hashNode* table;

	public:
		topK(int c){
			capacity = c;
			//one extra spot, we don't use 0 here.
			root = new node[c + 1];
			currentSize = 0;
			//creating a hash table of 4 times k.
			table = new hashNode[4*c];
		}

		void insert(std::string in) {
			//search if exists.
			if(this->searchtable(in)){
				//already in the table, increment count.
				node* location = this->searchtable(in);
				location->incFreq();
				//percUp and adjust
				this->percUp();
			} else if(currentSize < capacity) {
				currentSize++;
				root[currentSize].setContent(in);
				root[currentSize].setFreq(1);
				order.push_back(in);
				std::size_t index = (std::hash<std::string>{}(in))%(4*capacity);
				int quad = 0;
				
				while(table[index].getState() == 1){
					quad++;
					index = (index + (quad*quad))%(4*capacity);
				}
				
				table[index].setLoc(&root[currentSize]);
				table[index].setState(1);
				//percolateUp and adjust 
				this->percUp();
			} else {
				//deleteMin
			}
		}

		//returns the address in minHeap relating to the string
		node* searchtable(std::string in) {
			std::size_t index = (std::hash<std::string>{}(in))%(4*capacity);
			bool exists = false;
			int quad = 0;
			while(table[index].getState() != 0){
				if(table[index].getLoc()){
					if(table[index].getLoc()->getContent().compare(in) == 0){
						exists = true;
						break;
					} else {
						quad++;
						index = (index + (quad*quad))%(4*capacity);	
					}
				}
			}
			if(exists) {
				return table[index].getLoc();
			} else {
				return NULL;
			}
		}

		//returns index in hashTable relating to the string
		int getTableInd(std::string in) {
			std::size_t index = (std::hash<std::string>{}(in))%(4*capacity);
			bool exists = false;
			int quad = 0;
			while(table[index].getState() != 0){
				if(table[index].getLoc()) {
					if(table[index].getLoc()->getContent().compare(in) == 0){
						exists = true;
						break;
					} else {
						quad++;
						index = (index + (quad*quad))%(4*capacity);	
					}
				}
			}

			if(exists) {
				return index;
			} else {
				return -1;
			}
		}

		void percUp() {
			int childInd = currentSize;
			int parentInd;
			int childHash;
			int parentHash;
			while(childInd > 1){
				parentInd = childInd/2;
				childHash = this->getTableInd(root[childInd].getContent());
				parentHash = this->getTableInd(root[parentInd].getContent());
				if(root[childInd].getFreq() < root[parentInd].getFreq()) {
					//swap in the minHeap
					node temp(root[childInd].getContent(), root[childInd].getFreq());
					root[childInd].setContent(root[parentInd].getContent());
					root[childInd].setFreq(root[parentInd].getFreq());
					root[parentInd].setContent(temp.getContent());
					root[parentInd].setFreq(temp.getFreq());

					//adjust address in the hashTable
					table[childHash].setLoc(&root[parentInd]);
					table[parentHash].setLoc(&root[childInd]);

				} else if(root[childInd].getFreq() == root[parentInd].getFreq()) {
					//check if child is older than the parent
					if (this->getOrder(root[childInd].getContent()) < this->getOrder(root[parentInd].getContent())) {
						//swap
						node temp(root[childInd].getContent(), root[childInd].getFreq());
						root[childInd].setContent(root[parentInd].getContent());
						root[childInd].setFreq(root[parentInd].getFreq());
						root[parentInd].setContent(temp.getContent());
						root[parentInd].setFreq(temp.getFreq());

						//adjust address in the hashTable
						table[childHash].setLoc(&root[parentInd]);
						table[parentHash].setLoc(&root[childInd]);

					} else {
						break;
					}
				} else {
					break;
				}
				childInd = childInd/2;
			}
		}

		void percDown() {

		}

		void printHeap() {
			cout << "Heap (min to max):" << endl;
			for(int i = 1; i <= currentSize; i++) {
				cout << i << ": " << root[i].getContent() << ", " << root[i].getFreq() << endl;
			}
		}

		void printTable() {
			for(int i = 0; i < 4*capacity; i++) {
				if(table[i].getLoc()) {
					cout << table[i].getLoc()->getContent() << ": " << table[i].getLoc() << endl;
				}
			}
		}

		int getOrder(std::string in) {
			//int index = 0;
			for(int i = 0; i < order.size(); i++) {
				if(order.at(i).compare(in)==0) {
					return i;
				}
			}
			//if the string is not in the order vector... should never happen if correctly instantiated...
			return -1;
		}
};

#endif
