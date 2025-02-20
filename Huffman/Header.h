#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <bitset>

using namespace std;

// ��������� ��� ���� ������ ��������
struct Node {
    string data; // ��� �������� ������� ��� RGB-��������
    int freq;
    Node* left;
    Node* right;

    Node(string d, int frequency);
    ~Node(); // ���������� �����������
};

// ���������� ��� ������������ �������
struct Compare {
    bool operator()(Node* l, Node* r);
};

// ��������� �������
Node* createHuffmanTree(const unordered_map<string, int>& frequency);
void generateHuffmanCodes(Node* root, const string& str, unordered_map<string, string>& huffmanCodes);
void compressText(const string& inputFilePath, const string& outputFilePath);
void compressBMP(const string& inputFilePath, const string& outputFilePath);
void decompressText(const string& inputFilePath, const string& outputFilePath);
void decompressBMP(const string& inputFilePath, const string& outputFilePath);

#endif // HUFFMAN_H
