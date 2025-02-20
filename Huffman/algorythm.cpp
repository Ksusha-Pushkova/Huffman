#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <bitset>
#include <locale> // ��� ������ � ������� �������
#include <codecvt> // ��� �������������� ���������

Node::~Node() {
	delete left;
	delete right;
}
// ���������� ������������ Node
Node::Node(string d, int frequency) : data(d), freq(frequency), left(nullptr), right(nullptr) {}

// ���������� ����������� Compare
bool Compare::operator()(Node* l, Node* r) {
	return l->freq > r->freq;
}


// ������� ��� �������� ������ ��������
Node* createHuffmanTree(const unordered_map<string, int>& frequency) {
    priority_queue<Node*, vector<Node*>, Compare> minHeap;

    for (const auto& pair : frequency) {
        minHeap.push(new Node(pair.first, pair.second));
    }

    while (minHeap.size() > 1) {
        Node* left = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();

        Node* sum = new Node("", left->freq + right->freq);
        sum->left = left;
        sum->right = right;

        minHeap.push(sum);
    }

    return minHeap.top();
}
// ������� ��� �������� ����� ��������
void generateHuffmanCodes(Node* root, const string& str, unordered_map<string, string>& huffmanCodes) {
    if (!root) return;

    if (root->left == nullptr && root->right == nullptr) {
        huffmanCodes[root->data] = str;
    }

    generateHuffmanCodes(root->left, str + "0", huffmanCodes);
    generateHuffmanCodes(root->right, str + "1", huffmanCodes);
}