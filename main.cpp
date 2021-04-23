#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include "node.h"

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
	
	cout << "No. of Top-K elements: " << k << endl;
	cout << "First K strings: " << endl;
	for (int i = 0; i < k; i++) {
		cout << lines.at(i) << endl;
	}
	
	node* n = new node("halo", 12);
	cout << n->getFreq() << endl;

	return 0;
}
