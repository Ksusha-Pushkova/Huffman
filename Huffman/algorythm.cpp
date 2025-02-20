#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <bitset>
#include "Header.h"
#include <locale> // Для работы с русской локалью
#include <codecvt> // Для преобразования кодировок


// Реализация деструктора Node
Node::~Node() {
    delete left;
    delete right;
}

// Реализация конструктора Node
Node::Node(string d, int frequency) : data(d), freq(frequency), left(nullptr), right(nullptr) {}

// Реализация компаратора Compare
bool Compare::operator()(Node* l, Node* r) {
    return l->freq > r->freq;
}

// Функция для создания дерева Хаффмана
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

// Функция для создания кодов Хаффмана
void generateHuffmanCodes(Node* root, const string& str, unordered_map<string, string>& huffmanCodes) {
    if (!root) return;

    if (root->left == nullptr && root->right == nullptr) {
        huffmanCodes[root->data] = str;
    }

    generateHuffmanCodes(root->left, str + "0", huffmanCodes);
    generateHuffmanCodes(root->right, str + "1", huffmanCodes);
}

// Функция для сжатия текстового файла
void compressText(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath);
    unordered_map<string, int> frequency;
    string line;

    // Подсчет частоты символов с учётом новой строки
    while (getline(inputFile, line)) {
        for (char c : line) {
            frequency[string(1, c)]++;
        }
        frequency["\n"]++; 
    }

    inputFile.close();

    // Создание дерева
    Node* root = createHuffmanTree(frequency);

    // Генерация кодов
    unordered_map<string, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    // Сжатие данных
    string encodedString;

    inputFile.open(inputFilePath);

    while (getline(inputFile, line)) {
        for (char c : line) {
            encodedString += huffmanCodes[string(1, c)];
        }
        encodedString += huffmanCodes["\n"]; // Добавляем код новой строки
    }

    inputFile.close();

    // Запись сжатого файла
    ofstream outputFile(outputFilePath, ios::binary);

    // Преобразование закодированной строки в байты
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

// Функция для сжатия BMP файла
void compressBMP(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath, ios::binary);

    // Чтение заголовка BMP
    unsigned char header[54];
    inputFile.read(reinterpret_cast<char*>(header), sizeof(header));

    // Получение ширины и высоты изображения
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    // Чтение пикселей
    unordered_map<string, int> frequency;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            unsigned char pixel[3];
            inputFile.read(reinterpret_cast<char*>(pixel), sizeof(pixel));
            string color = string(pixel, pixel + 3);
            frequency[color]++;
        }

        // Пропуск заполнителей (padding)
        inputFile.ignore((4 - (width * 3) % 4) % 4);
    }

    inputFile.close();

    // Создание дерева Хаффмана
    Node* root = createHuffmanTree(frequency);

    // Генерация кодов Хаффмана
    unordered_map<string, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    // Сжатие данных
    string encodedString;

    inputFile.open(inputFilePath, ios::binary);

    // Пропускаем заголовок
    inputFile.read(reinterpret_cast<char*>(header), sizeof(header));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            unsigned char pixel[3];
            inputFile.read(reinterpret_cast<char*>(pixel), sizeof(pixel));
            string color = string(pixel, pixel + 3);
            encodedString += huffmanCodes[color];
        }

        // Пропуск заполнителей (padding)
        inputFile.ignore((4 - (width * 3) % 4) % 4);
    }

    inputFile.close();

    ofstream outputFile(outputFilePath, ios::binary);

    outputFile.write(reinterpret_cast<char*>(header), sizeof(header));

    // Преобразование закодированной строки в байты
    for (size_t i = 0; i < encodedString.size(); i += 8) {
        bitset<8> byte(encodedString.substr(i, 8));
        outputFile.put(static_cast<unsigned char>(byte.to_ulong()));
    }

    // Если длина закодированной строки не кратна 8, добавляем недостающие нули
    if (encodedString.size() % 8 != 0) {
        bitset<8> byte(encodedString.substr(encodedString.size() - (encodedString.size() % 8)).append(8 - (encodedString.size() % 8), '0'));
        outputFile.put(static_cast<unsigned char>(byte.to_ulong()));
    }

    outputFile.close();
    delete root;
}

// функция для чтения битов из файла
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

// Функция для разжатия текстового файла
void decompressText(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath, ios::binary);
    if (!inputFile.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл для декомпрессии: " << inputFilePath << endl;
        return;
    }

    // Чтение битовой строки из файла
    string bitString = readBitsFromFile(inputFile);
    inputFile.close();

    // Создание обратного отображения кодов 
    unordered_map<string, string> huffmanCodes;
    unordered_map<string, int> frequency;
    string symbol;

    //Подсчет частот
    if (!inputFile.is_open()) {
        ifstream inputFile(inputFilePath);
        string line;

        // Подсчет частоты символов с учётом новой строки
        while (getline(inputFile, line)) {
            for (char c : line) {
                frequency[string(1, c)]++;
            }
            frequency["\n"]++; 
        }

        inputFile.close();
    }

    // Создание дерева 
    Node* root = createHuffmanTree(frequency);

    // Генерация кодов
    generateHuffmanCodes(root, "", huffmanCodes);
    unordered_map<string, string> reverseHuffmanCodes;
    for (const auto& pair : huffmanCodes) {
        reverseHuffmanCodes[pair.second] = pair.first;
    }

    // Декодирование данных
    string decodedString;
    string currentCode;
    for (char bit : bitString) {
        currentCode += bit;
        if (reverseHuffmanCodes.count(currentCode)) {
            decodedString += reverseHuffmanCodes[currentCode];
            currentCode = "";
        }
    }

    // Запись разжатого файла
    ofstream outputFile(outputFilePath);
    outputFile << decodedString;
    outputFile.close();

    // Освобождение памяти
    delete root;
}

// Функция для декомпрессии BMP файла
void decompressBMP(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath, ios::binary);
    if (!inputFile.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл для декомпрессии: " << inputFilePath << endl;
        return;
    }

    // Чтение заголовка BMP
    unsigned char header[54];
    inputFile.read(reinterpret_cast<char*>(header), sizeof(header));

    // Получение ширины и высоты изображения
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    // Чтение битовой строки из файла (после заголовка)
    string bitString;
    unsigned char byte;

    // Пропускаем заголовок
    inputFile.seekg(54, ios::beg); // Перемещаем указатель чтения после заголовка
    while (inputFile.get((char&)byte)) {
        for (int i = 7; i >= 0; --i) {
            bitString += ((byte >> i) & 1) ? '1' : '0';
        }
    }
    inputFile.close();

    // Создание обратного отображения кодов 
    unordered_map<string, string> huffmanCodes;
    unordered_map<string, int> frequency;

    //Подсчет частот
    if (!inputFile.is_open()) {
        ifstream inputFile(inputFilePath, ios::binary);
        // Пропускаем заголовок
        inputFile.seekg(54, ios::beg);

        // Чтение пикселей
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                unsigned char pixel[3];
                inputFile.read(reinterpret_cast<char*>(pixel), sizeof(pixel));
                string color = string(pixel, pixel + 3);
                frequency[color]++;
            }

            // Пропуск заполнителей (padding)
            inputFile.ignore((4 - (width * 3) % 4) % 4);
        }

        inputFile.close();
    }

    // Создание дерева 
    Node* root = createHuffmanTree(frequency);

    // Генерация кодов 
    generateHuffmanCodes(root, "", huffmanCodes);

    unordered_map<string, string> reverseHuffmanCodes;
    for (const auto& pair : huffmanCodes) {
        reverseHuffmanCodes[pair.second] = pair.first;
    }

    // Декодирование данных
    vector<string> decodedColors;
    string currentCode;
    for (char bit : bitString) {
        currentCode += bit;
        if (reverseHuffmanCodes.count(currentCode)) {
            decodedColors.push_back(reverseHuffmanCodes[currentCode]);
            currentCode = "";
        }
    }

    // Запись разжатого файла
    ofstream outputFile(outputFilePath, ios::binary);
    outputFile.write(reinterpret_cast<char*>(header), sizeof(header));

    // Запись пикселей
    int colorIndex = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (colorIndex < decodedColors.size()) {
                string color = decodedColors[colorIndex++];
                outputFile.write(color.data(), 3); // Запись 3 байтов цвета
            }
            else {
                // Обработка случая, если данных недостаточно
                unsigned char blackPixel[3] = { 0, 0, 0 }; // Черный пиксель
                outputFile.write(reinterpret_cast<char*>(blackPixel), sizeof(blackPixel));
            }
        }

        // Запись заполнителей (padding)
        for (int k = 0; k < (4 - (width * 3) % 4) % 4; ++k) {
            outputFile.put(0);
        }
    }

    outputFile.close();


    // Освобождение памяти
    delete root;
}
