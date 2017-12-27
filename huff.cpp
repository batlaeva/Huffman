#include "stdafx.h"
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <conio.h>
using namespace std;

class Node
{
public:
	int a;
	char c;
	Node *left, *right;

	Node() { left = right = NULL; }

	Node(Node *L, Node *R)
	{
		left = L;
		right = R;
		a = L->a + R->a;
	}
};

struct MyCompare//для сортировки
{
	bool operator()(const Node* l, const Node* r) const { return l->a < r->a; }
};

vector<bool> code;
map<char, vector<bool> > table;//связь 

void BuildTable(Node *nodePtr)//0бх0д дёрёва
{
	if (nodePtr->left != NULL)
	{
		code.push_back(0);
		BuildTable(nodePtr->left);
	}
	if (nodePtr->right != NULL)
	{
		code.push_back(1);
		BuildTable(nodePtr->right);
	}
	if (nodePtr->left == NULL && nodePtr->right == NULL)
		table[nodePtr->c] = code;
	if (code.size())
		code.pop_back();
}

int main()
{
	ifstream f("es.txt", ios::out | ios::binary);
	map<char, int> m;//имя и количество повторов
	while (!f.eof())
	{
		char c = f.get();
		m[c]++;
	}

	//записываем начальные узлы в список list
	list<Node*> nodeList;
	for (map<char, int>::iterator itr = m.begin(); itr != m.end(); ++itr)
	{
		Node *p = new Node;
		p->c = itr->first;
		p->a = itr->second;
		nodeList.push_back(p);
	}

	//создаем дерево		
	while (nodeList.size() != 1)
	{
		nodeList.sort(MyCompare());//сортировка

		Node *SonL = nodeList.front();//берем самый первый и минимальный 
		nodeList.pop_front();//удаляем
		Node *SonR = nodeList.front();
		nodeList.pop_front();

		Node *parent = new Node(SonL, SonR);
		nodeList.push_back(parent);//добавляем полученную из двух родительскую вершину
	}

	Node *nodePtr = nodeList.front();//nodePtr - указатель на вершину дерева
	//создаем пары 'символ-код':	
	BuildTable(nodePtr);
	
	f.clear(); f.seekg(0); // перемещаем указатель снова в начало файла
	
	ofstream g("output.txt", ios::out | ios::binary);
	int count = 0; char buf = 0;
	while (!f.eof()){
		char c = f.get();
		vector<bool> x = table[c];
		for (int n = 0; n<x.size(); n++){
			buf = buf | x[n] << (7 - count);// бинарные операции, читаем с лева на право по 8 символов
			count++;
			if (count == 8) {
				count = 0;
				g << buf;
				buf = 0;
			}
		}
	}
	f.close();
	g.close();

	//считывание из файла output.txt и преобразование обратно
	ifstream F("output.txt", ios::in | ios::binary);

	setlocale(LC_ALL, "Russian");

	Node *p = nodePtr;
	count = 0; char byte;
	byte = F.get();
	while (!F.eof())
	{
		bool b = byte & (1 << (7 - count));
		if (b) p = p->right; else p = p->left;
		if (p->left == NULL && p->right == NULL) { cout << p->c; p = nodePtr; }
		count++;
		if (count == 8) { count = 0; byte = F.get(); }
	}
	F.close();
	_getch();
	return 0;
}
