#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include "node.h"
#include "topK.h"
#include <unordered_map>
using namespace std;

int main(int argc, char *argv[]) {
	//cout << "Hello world!" << endl;
	ifstream source;
	string line;
	vector<std::string> lines;
	int k = stoi(argv[3]);
	string fileName = argv[1];
	source.open(fileName);
	if(!source) {
		cout << "Couldn't open file" << endl;
	} else {
		for(int i = 0; i < k; i++) {
			getline(source, line, ',');
			lines.push_back(line);
		}
	}
	/*	
	cout << "No. of Top-K elements: " << k << endl;
	cout << "First K strings: " << endl;
	for (int i = 0; i < k; i++) {
		cout << lines.at(i) << endl;
	}
	*/	


	topK* priority = new topK(k);
	priority->insert("cool");
	if(priority->searchtable("cool"))
		cout << "found" << endl;

	if(priority->searchtable("jeez"))
		cout << "found" << endl;
	else
		cout << "not found" << endl;
	
	priority->printHeap();
	priority->printTable();
	cout << "-------------------------------" << endl;
	
	priority->insert("cool");
	priority->printHeap();
	priority->printTable();
	cout << "-------------------------------" << endl;
	
	priority->insert("jeez");
	priority->printHeap();
	priority->printTable();
	cout << "-------------------------------" << endl;

	priority->insert("jeez");
	priority->printHeap();
	priority->printTable();
	cout << "-------------------------------" << endl;
	
	priority->insert("greetings!./");
	priority->printHeap();
	priority->printTable();
	cout << "-------------------------------" << endl;
	
	priority->insert("jeez");
	priority->printHeap();
	priority->printTable();
	cout << "-------------------------------" << endl;

	int minFreq = priority->deleteMin();
	priority->printHeap();
	priority->printTable();
	cout << "Min freq: " << minFreq << endl;
	cout << "-------------------------------" << endl;

	return 0;
}
