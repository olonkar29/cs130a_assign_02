#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

//Creating a struct for Nodes that we will make an array of for our heap and our hash table will be a vector of node*

class node{
	private:
		std::string content;
		int freq;
	
	public:
		node() {content = ""; freq = 0;}	
		node(std::string cont, int f){content = cont; freq = f;}
		void setContent(std::string c) {content = c;}
		void setFreq(int f) {freq = f;}
		void incFreq() {freq++;}

		std::string getContent() {return content;}
		int getFreq() {return freq;}
};

class hashNode{
	private:
		//points to the minHeap nodes
		node* loc;

		//0 is unused, 1  is active, 2 is deleted
		int state;
	public:
		hashNode(){loc = NULL; state = 0;}
		void setLoc(node* n) {loc = n;}
		void setState(int s) {state = s;}
		node* getLoc() {return loc;}
		int getState() {return state;}
};

#endif
