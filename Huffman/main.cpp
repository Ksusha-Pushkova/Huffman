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
	cout << "'compress' ��� ������,'decompress' ��� ��������:";
	cin >> operation;
	cout << "���� � �������� �����: ";
	cin >> file_path;
	cout << "���� � ��������� �����: ";
	cin >> output_file_path;
    if (operation == "compress") {
        if (file_path.substr(file_path.find_last_of(".") + 1) == "txt") {
            compressText(file_path, output_file_path); // �������� ����
            cout << "��������� ���� ������� ����!" << endl;
        }
        else if (file_path.substr(file_path.find_last_of(".") + 1) == "bmp") {
            compressBMP(file_path, output_file_path); // �������� ����
            cout << "BMP ���� ������� ����!" << endl;
        }
        else {
            cout << "���������������� ������ �����!" << endl;
        }
    }
    else if (operation == "decompress") {
        if (file_path.substr(file_path.find_last_of(".") + 1) == "txt") {
            decompressText(file_path, output_file_path); // �������� ����
            cout << "��������� ���� ������� ������!" << endl;
        }
        else if (file_path.substr(file_path.find_last_of(".") + 1) == "bmp") {
            decompressBMP(file_path, output_file_path); // �������� ����
            cout << "BMP ���� ������� ������!" << endl;
        }
        else {
            cout << "���������������� ������ �����!" << endl;
        }
    }
    else {
        cout << "�������� ��������. ����������� 'compress' ��� 'decompress'." << endl;
    }
    return 0;
}