#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <bitset>
#include <locale> // Для работы с русской локалью
#include <codecvt> // Для преобразования кодировок

Node::~Node() {
	delete left;
	delete right;
}