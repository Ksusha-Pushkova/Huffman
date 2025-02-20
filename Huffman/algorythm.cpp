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

// ������� ��� ������ BMP �����
void compressBMP(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath, ios::binary);

    // ������ ��������� BMP
    unsigned char header[54];
    inputFile.read(reinterpret_cast<char*>(header), sizeof(header));

    // ��������� ������ � ������ �����������
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    // ������ ��������
    unordered_map<string, int> frequency;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            unsigned char pixel[3];
            inputFile.read(reinterpret_cast<char*>(pixel), sizeof(pixel));
            string color = string(pixel, pixel + 3);
            frequency[color]++;
        }

        // ������� ������������ (padding)
        inputFile.ignore((4 - (width * 3) % 4) % 4);
    }

    inputFile.close();

    // �������� ������ ��������
    Node* root = createHuffmanTree(frequency);

    // ��������� ����� ��������
    unordered_map<string, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    // ������ ������
    string encodedString;

    inputFile.open(inputFilePath, ios::binary);

    // ���������� ���������
    inputFile.read(reinterpret_cast<char*>(header), sizeof(header));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            unsigned char pixel[3];
            inputFile.read(reinterpret_cast<char*>(pixel), sizeof(pixel));
            string color = string(pixel, pixel + 3);
            encodedString += huffmanCodes[color];
        }

        // ������� ������������ (padding)
        inputFile.ignore((4 - (width * 3) % 4) % 4);
    }

    inputFile.close();

    ofstream outputFile(outputFilePath, ios::binary);

    outputFile.write(reinterpret_cast<char*>(header), sizeof(header));

    // �������������� �������������� ������ � �����
    for (size_t i = 0; i < encodedString.size(); i += 8) {
        bitset<8> byte(encodedString.substr(i, 8));
        outputFile.put(static_cast<unsigned char>(byte.to_ulong()));
    }

    // ���� ����� �������������� ������ �� ������ 8, ��������� ����������� ����
    if (encodedString.size() % 8 != 0) {
        bitset<8> byte(encodedString.substr(encodedString.size() - (encodedString.size() % 8)).append(8 - (encodedString.size() % 8), '0'));
        outputFile.put(static_cast<unsigned char>(byte.to_ulong()));
    }

    outputFile.close();
    delete root;
}

// ������� ��� ������ ����� �� �����
string readBitsFromFile(ifstream& inputFile) {
    string bitString;
    unsigned char byte;
    while (inputFile.get((char&)byte)) {
        for (int i = 7; i >= 0; --i) {
            bitString += ((byte >> i) & 1) ? '1' : '0';
        }
    }
    return bitString;
}

// ������� ��� �������� ���������� �����
void decompressText(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath, ios::binary);
    if (!inputFile.is_open()) {
        cerr << "������: �� ������� ������� ���� ��� ������������: " << inputFilePath << endl;
        return;
    }

    // ������ ������� ������ �� �����
    string bitString = readBitsFromFile(inputFile);
    inputFile.close();

    // �������� ��������� ����������� ����� 
    unordered_map<string, string> huffmanCodes;
    unordered_map<string, int> frequency;
    string symbol;

    //������� ������
    if (!inputFile.is_open()) {
        ifstream inputFile(inputFilePath);
        string line;

        // ������� ������� �������� � ������ ����� ������
        while (getline(inputFile, line)) {
            for (char c : line) {
                frequency[string(1, c)]++;
            }
            frequency["\n"]++; 
        }

        inputFile.close();
    }

    // �������� ������ 
    Node* root = createHuffmanTree(frequency);

    // ��������� �����
    generateHuffmanCodes(root, "", huffmanCodes);
    unordered_map<string, string> reverseHuffmanCodes;
    for (const auto& pair : huffmanCodes) {
        reverseHuffmanCodes[pair.second] = pair.first;
    }

    // ������������� ������
    string decodedString;
    string currentCode;
    for (char bit : bitString) {
        currentCode += bit;
        if (reverseHuffmanCodes.count(currentCode)) {
            decodedString += reverseHuffmanCodes[currentCode];
            currentCode = "";
        }
    }

    // ������ ��������� �����
    ofstream outputFile(outputFilePath);
    outputFile << decodedString;
    outputFile.close();

    // ������������ ������
    delete root;
}

// ������� ��� ������������ BMP �����
void decompressBMP(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath, ios::binary);
    if (!inputFile.is_open()) {
        cerr << "������: �� ������� ������� ���� ��� ������������: " << inputFilePath << endl;
        return;
    }

    // ������ ��������� BMP
    unsigned char header[54];
    inputFile.read(reinterpret_cast<char*>(header), sizeof(header));

    // ��������� ������ � ������ �����������
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    // ������ ������� ������ �� ����� (����� ���������)
    string bitString;
    unsigned char byte;

    // ���������� ���������
    inputFile.seekg(54, ios::beg); // ���������� ��������� ������ ����� ���������
    while (inputFile.get((char&)byte)) {
        for (int i = 7; i >= 0; --i) {
            bitString += ((byte >> i) & 1) ? '1' : '0';
        }
    }
    inputFile.close();

    // �������� ��������� ����������� ����� 
    unordered_map<string, string> huffmanCodes;
    unordered_map<string, int> frequency;

    //������� ������
    if (!inputFile.is_open()) {
        ifstream inputFile(inputFilePath, ios::binary);
        // ���������� ���������
        inputFile.seekg(54, ios::beg);

        // ������ ��������
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                unsigned char pixel[3];
                inputFile.read(reinterpret_cast<char*>(pixel), sizeof(pixel));
                string color = string(pixel, pixel + 3);
                frequency[color]++;
            }

            // ������� ������������ (padding)
            inputFile.ignore((4 - (width * 3) % 4) % 4);
        }

        inputFile.close();
    }

    // �������� ������ 
    Node* root = createHuffmanTree(frequency);

    // ��������� ����� 
    generateHuffmanCodes(root, "", huffmanCodes);

    unordered_map<string, string> reverseHuffmanCodes;
    for (const auto& pair : huffmanCodes) {
        reverseHuffmanCodes[pair.second] = pair.first;
    }

    // ������������� ������
    vector<string> decodedColors;
    string currentCode;
    for (char bit : bitString) {
        currentCode += bit;
        if (reverseHuffmanCodes.count(currentCode)) {
            decodedColors.push_back(reverseHuffmanCodes[currentCode]);
            currentCode = "";
        }
    }

    // ������ ��������� �����
    ofstream outputFile(outputFilePath, ios::binary);
    outputFile.write(reinterpret_cast<char*>(header), sizeof(header));

    // ������ ��������
    int colorIndex = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (colorIndex < decodedColors.size()) {
                string color = decodedColors[colorIndex++];
                outputFile.write(color.data(), 3); // ������ 3 ������ �����
            }
            else {
                // ��������� ������, ���� ������ ������������
                unsigned char blackPixel[3] = { 0, 0, 0 }; // ������ �������
                outputFile.write(reinterpret_cast<char*>(blackPixel), sizeof(blackPixel));
            }
        }

        // ������ ������������ (padding)
        for (int k = 0; k < (4 - (width * 3) % 4) % 4; ++k) {
            outputFile.put(0);
        }
    }

    outputFile.close();


    // ������������ ������
    delete root;
}
