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
	cout << "'compress' ��� ������,'decompress' ��� ��������:";
	cin >> operation;
	cout << "���� � �������� �����: ";
	cin >> file_path;
	cout << "���� � ��������� �����: ";
	cin >> output_file_path;
}