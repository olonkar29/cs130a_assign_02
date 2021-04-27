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
				//currentSize == capacity
				//
				//deleteMin, save frequency for min
				int insertFreq = this->deleteMin();
				//insert new item with frequency saved from prev min
				currentSize++;
				root[currentSize].setContent(in);
				root[currentSize].setFreq(insertFreq);
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
				
				//percDown and adjust, make sure newer string stays at the bottom.
			}
		}

		int deleteMin() {
			if(currentSize > 0) {
				//deletes first value in heap, returns the value of frequency of the minimum..
				int prevFreq = root[1].getFreq();
				//int order = this->getOrder(root[1].getContent());
				//order.erase(order.begin() + order - 1);
				//adjust locations in hashTable: nullify initial root, set last val's location to root
				table[getTableInd(root[1].getContent())].setState(2);
				table[getTableInd(root[1].getContent())].setLoc(NULL);
				table[getTableInd(root[currentSize].getContent())].setLoc(&root[1]);

				//set root to the last value in the heap
				root[1].setContent(root[currentSize].getContent());
				root[1].setFreq(root[currentSize].getFreq());
				//reset last value in heap to default
				root[currentSize].setContent("");
				root[currentSize].setFreq(0);
				//reduce currentSize by one
				currentSize--;
				//percDown and adjust... 
				this->percDown();
				return prevFreq;
			}
			return -1;
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
				} else {
					quad++;
					index = (index + (quad*quad))%(4*capacity);	

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
			//cout << "percDowned" << endl;
			int parent = 1;
			int minChild;
			int parentHash;
			int childHash;
			while(parent < currentSize && this->getMinChild(parent) > 0) {
				minChild = this->getMinChild(parent);
				childHash = this->getTableInd(root[minChild].getContent());
				parentHash = this->getTableInd(root[parent].getContent());
				if(root[parent].getFreq() > root[minChild].getFreq()) {
					//swap
					node temp(root[minChild].getContent(), root[minChild].getFreq());
					root[minChild].setContent(root[parent].getContent());
					root[minChild].setFreq(root[parent].getFreq());
					root[parent].setContent(temp.getContent());
					root[parent].setFreq(temp.getFreq());

					//adjust address in the hashTable
					table[childHash].setLoc(&root[parent]);
					table[parentHash].setLoc(&root[minChild]);

				} else if(root[parent].getFreq() == root[minChild].getFreq() && this->getOrder(root[parent].getContent()) > this->getOrder(root[minChild].getContent())) {
					//swap
					node temp(root[minChild].getContent(), root[minChild].getFreq());
					root[minChild].setContent(root[parent].getContent());
					root[minChild].setFreq(root[parent].getFreq());
					root[parent].setContent(temp.getContent());
					root[parent].setFreq(temp.getFreq());

					//adjust address in the hashTable
					table[childHash].setLoc(&root[parent]);
					table[parentHash].setLoc(&root[minChild]);
				} else {
					break;
				}
				
			}
		}

		//returns the index of the minimum of the two children given a parent.
		int getMinChild(int index) {
			if(root[2*index].getFreq() > 0) {
				if((root[2*index].getFreq() < root[2*index + 1].getFreq()) || root[2*index + 1].getFreq() == 0) {
					return 2*index;
				} else if ((root[2*index].getFreq() > root[2*index + 1].getFreq())&& root[2*index + 1].getFreq() > 0) {
					return (2*index + 1);
				} else if (root[2*index].getFreq() == root[2*index + 1].getFreq()) {
					if(this->getOrder(root[2*index].getContent()) < this->getOrder(root[2*index + 1].getContent())) {
						return 2*index;
					} else {
						return (2*index + 1);
					}
				} 
				return -1;
			} else {
				//at the leaf level
				return -1;
			}
		}

		void printHeap() {
			cout << "Heap (min to max):" << endl;
			for(int i = 1; i <= currentSize; i++) {
				cout << i << ": " << root[i].getContent() << ", " << root[i].getFreq() << endl;
			}
			cout << "Current size is " << currentSize << endl;
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

		void printOrder() {
			for(int i = 0; i < order.size(); i++) {
				cout << i << ": " << order.at(i) << endl;
			}
		}
};

#endif
