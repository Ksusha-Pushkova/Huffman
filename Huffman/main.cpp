#include "Header.h"
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
using namespace std;

int main() {
	string file_path;
	string operation;
	string output_file_path;
	setlocale(LC_ALL, "Russian");
	cout << "'compress' для сжатия,'decompress' для разжатия:";
	cin >> operation;
	cout << "Путь к входному файлу: ";
	cin >> file_path;
	cout << "Путь к выходному файлу: ";
	cin >> output_file_path;
    if (operation == "compress") {
        if (file_path.substr(file_path.find_last_of(".") + 1) == "txt") {
            compressText(file_path, output_file_path); // Передаем путь
            cout << "Текстовый файл успешно сжат!" << endl;
        }
        else if (file_path.substr(file_path.find_last_of(".") + 1) == "bmp") {
            compressBMP(file_path, output_file_path); // Передаем путь
            cout << "BMP файл успешно сжат!" << endl;
        }
        else {
            cout << "Неподдерживаемый формат файла!" << endl;
        }
    }
    else if (operation == "decompress") {
        if (file_path.substr(file_path.find_last_of(".") + 1) == "txt") {
            decompressText(file_path, output_file_path); // Передаем путь
            cout << "Текстовый файл успешно разжат!" << endl;
        }
        else if (file_path.substr(file_path.find_last_of(".") + 1) == "bmp") {
            decompressBMP(file_path, output_file_path); // Передаем путь
            cout << "BMP файл успешно разжат!" << endl;
        }
        else {
            cout << "Неподдерживаемый формат файла!" << endl;
        }
    }
    else {
        cout << "Неверная операция. Используйте 'compress' или 'decompress'." << endl;
    }
    return 0;
}