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
}