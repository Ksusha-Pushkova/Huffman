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