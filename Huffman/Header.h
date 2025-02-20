#ifdef HEADER_H
#define HEADER_H
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <bitset>
using namespace std;
struct Node {
	string data;
	int freq;
	Node* left;
	Node* right;

	Node(string d, int frequency);
	~Node()
};

struct Compare {
	bool operator()(Node* l, Node* r);
};
Node* createHuffmanTree(const unordered_map<string, int>& frequency);
void generateHuffmanCodes(Node* root, const string& str, unordered_map<string, string>& huffmanCodes);

#endif;