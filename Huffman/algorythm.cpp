#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <bitset>
#include "Header.h"
#include <locale> // ��� ������ � ������� �������
#include <codecvt> // ��� �������������� ���������


// ���������� ����������� Node
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

// ������� ��� ������ ���������� �����
void compressText(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath);
    unordered_map<string, int> frequency;
    string line;

    // ������� ������� �������� � ������ ����� ������
    while (getline(inputFile, line)) {
        for (char c : line) {
            frequency[string(1, c)]++;
        }
        frequency["\n"]++; 
    }

    inputFile.close();

    // �������� ������
    Node* root = createHuffmanTree(frequency);

    // ��������� �����
    unordered_map<string, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    // ������ ������
    string encodedString;

    inputFile.open(inputFilePath);

    while (getline(inputFile, line)) {
        for (char c : line) {
            encodedString += huffmanCodes[string(1, c)];
        }
        encodedString += huffmanCodes["\n"]; // ��������� ��� ����� ������
    }

    inputFile.close();

    // ������ ������� �����
    ofstream outputFile(outputFilePath, ios::binary);

    // �������������� �������������� ������ � �����
    for (size_t i = 0; i < encodedString.size(); i += 8) {
        bitset<8> byte(encodedString.substr(i, 8));
        outputFile.put(static_cast<unsigned char>(byte.to_ulong()));
    }

    if (encodedString.size() % 8 != 0) {
        bitset<8> byte(encodedString.substr(encodedString.size() - (encodedString.size() % 8)).append(8 - (encodedString.size() % 8), '0'));
        outputFile.put(static_cast<unsigned char>(byte.to_ulong()));
    }

    outputFile.close();
    delete root;
}

