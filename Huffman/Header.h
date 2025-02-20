#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <bitset>

using namespace std;

// Структура для узла дерева Хаффмана
struct Node {
    string data; // Для хранения символа или RGB-значения
    int freq;
    Node* left;
    Node* right;

    Node(string d, int frequency);
    ~Node(); // Объявление деструктора
};

// Компаратор для приоритетной очереди
struct Compare {
    bool operator()(Node* l, Node* r);
};

// Прототипы функций
Node* createHuffmanTree(const unordered_map<string, int>& frequency);
void generateHuffmanCodes(Node* root, const string& str, unordered_map<string, string>& huffmanCodes);
void compressText(const string& inputFilePath, const string& outputFilePath);
void compressBMP(const string& inputFilePath, const string& outputFilePath);
void decompressText(const string& inputFilePath, const string& outputFilePath);
void decompressBMP(const string& inputFilePath, const string& outputFilePath);

#endif // HUFFMAN_H
