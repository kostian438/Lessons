#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

using namespace std;

struct Offense //Одно правонарушение
{
	char* offense;
	Offense * Next;
};

class ListOffense
{
	Offense * Head;
	Offense * Tail;
	int Count; //Количество элементов списка
public:
	ListOffense();
	~ListOffense();
	void AddOffense(char* data); //(Новый элемент становится последним)
	void DelOffense(); //(Удаляется головной элемент)
	void DelAllOffense(); //Удаление всего списка
	void PrintOffense(); //(Распечатка начинается с головного элемента)
	int GetCount(); //Получение количества элементов списка
	
};

struct Elem
{
	char Number[9]; //Номер машины
	ListOffense* list; //Список правонарушений
	Elem * left, *right, *parent;
};

class Tree {
	Elem * root; //корень
public:
	Tree();
	~Tree();
	void Print(Elem * Node); //печать от указанного узла
	Elem * Search(Elem * Node, char* key); //поиск от указанного узла
	Elem * Min(Elem * Node); //min от указанного узла
	Elem * Max(Elem * Node); //max от указанного узла
	Elem * Next(Elem * Node); //следующий для указанного узла
	Elem * Previous(Elem * Node); //предыдущий для указанного узла
	void Insert(Elem * z); //вставка узла
	void Del(Elem * z = 0); //удаление ветки для указанного узла, 0 - удаление всего дерева
	Elem * GetRoot(); //получить корень
};

Tree::Tree() { root = NULL; }

Tree::~Tree() { cout << "Это деструктор БД!";  Del(); system("pause");}

void Tree::Print(Elem * Node) //Рекурсивный обход дерева
{
	if (Node != 0)
	{
		Print(Node->left);
		cout << "Номер машины: ";
		for (int i = 0; i < 8; i++) {
			cout << Node->Number[i];
		}
		cout << endl;
		cout << "Нарушения: ";
		Node->list->PrintOffense();
		
		Print(Node->right);
	}
}

Elem * Tree::Search(Elem * Node, char* k)
{
	//Пока есть узлы и ключи не совпадают
	int flag = 10;
	while (Node != 0 && flag!=0) 
	{
		flag = 0;
		for (int i = 0; i < 8; i++) {
			if (k[i] < Node->Number[i]) { flag = -1; break; }
			if (k[i] > Node->Number[i]) { flag = 1; break; }
		}
		if (flag < 0) 
			Node = Node->left;
		if(flag > 0)
			Node = Node->right;
	}
	return Node;
}

Elem * Tree::Min(Elem * Node)
{
	//Поиск самого "левого" узла
	if (Node != 0)
		while (Node->left != 0)
			Node = Node->left;
	return Node;
}

Elem * Tree::Max(Elem * Node)
{
	//Поиск самого "правого" узла
	if (Node != 0)
		while (Node->right != 0)
			Node = Node->right;
	return Node;
}

Elem * Tree::Next(Elem * Node)
{
	Elem * y = 0;
	if (Node != 0)
	{
		//если есть правый потомок
		if (Node->right != 0)
			return Min(Node->right);
		//родитель узла
		y = Node->parent;
		//если Node не корень и Node справа
		while (y != 0 && Node == y->right)
		{
			//Движемся вверх
			Node = y;
			y = y->parent;
		}
	}
	return y;
}

Elem * Tree::Previous(Elem * Node)
{
	Elem * y = 0;
	if (Node != 0)
	{
		//если есть левый потомок
		if (Node->left != 0)
			return Max(Node->left);
		//родитель узла
		y = Node->parent;
		//если Node не корень и Node слева
		while (y != 0 && Node == y->left)
		{
			//Движемся вверх
			Node = y;
			y = y->parent;
		}
	}
	return y;
}

Elem * Tree::GetRoot() { return root; }

void Tree::Insert(Elem * z)
{
	//потомков нет
	z->left = NULL;
	z->right = NULL;
	Elem * y = NULL;
	Elem * Node = root;
	//поиск места
	while (Node != 0)
	{
		//будущий родитель
		y = Node;
		int flag = 1;
		for (int i = 0; i < 8; i++) {
			if (z->Number[i] < Node->Number[i]) { flag = -1; }
		}
		if ( flag < 0) //strcmp(z->Number, Node->Number)
			Node = Node->left;
		else
			Node = Node->right;
	}
	//заполняем родителя
	z->parent = y;
	if (y == 0) //элемент первый (единственный)
		root = z;
	//чей ключ больше?
	else {
		int flag = 1;
		for (int i = 0; i < 8; i++) {
			if (z->Number[i] < y->Number[i]) { flag = -1; }
		}
		if (flag < 0) //strcmp(z->Number, y->Number)
			y->left = z;
		else
			y->right = z;
	}
}

void Tree::Del(Elem * z)
{
	//удаление куста
	if (z != 0)
	{
		Elem * Node, *y;
		//не 2 ребенка
		if (z->left == 0 || z->right == 0)
			y = z;
		else
			y = Next(z);
		if (y->left != 0)
			Node = y->left;
		else
			Node = y->right;
		if (Node != 0)
			Node->parent = y->parent;
		//Удаляется корневой узел?
		if (y->parent == 0)
			root = Node;
		else if (y == y->parent->left)
			//слева от родителя?
			y->parent->left = Node;
		else
			//справа от родителя?
			y->parent->right = Node;
		if (y != z)
		{
			//Копирование данных узла
			for (int i = 0; i < 8; i++) {
				z->Number[i] = y->Number[i];
			}
			z->list = new ListOffense;
			*z->list = *y->list;
		}
		y->list->DelAllOffense();
		delete y;
	}
	
	else //удаление всего дерева
		while (root != 0)
			Del(root);
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//------------Методы Списка Правонарушений------------------------------

ListOffense::ListOffense()
{
	Head = Tail = NULL; //Изначально список пуст
	Count = 0;
}
ListOffense::~ListOffense()
{
	DelAllOffense(); //Вызов функции удаления
}
int ListOffense::GetCount()
{
	return Count;
}
void ListOffense::AddOffense(char* data)
{
	Offense * temp = new Offense; //создание нового элемента
	int len = strlen(data) + 1;
	temp->offense = new char[len];
	strcpy_s(temp->offense, len, data); //заполнение данными
	temp->Next = NULL; //следующий элемент отсутствует
					   //новый элемент становится последним элементом списка
					   //если он не первый добавленный
	if (Head != NULL) {
		Tail->Next = temp;
		Tail = temp;
	}
	//новый элемент становится единственным
	//если он первый добавленный
	else {
		Head = Tail = temp;
	}
	Count++;

}

void ListOffense::DelOffense()
{
	Offense * temp = Head; //запоминаем адрес головного элемента
	Head = Head->Next; //перебрасываем голову на следующий элемент
	delete[] temp->offense;
	delete temp; //удаляем бывший головной элемент
}
void ListOffense::DelAllOffense()
{
	while (Head != 0) //Пока еще есть элементы
		DelOffense(); //Удаляем элементы по одному
}
void ListOffense::PrintOffense()
{
	Offense * temp = Head; //запоминаем адрес головного элемента
	while (temp != 0) //Пока еще есть элементы
	{
		cout << temp->offense << " "; //Выводим данные
		temp = temp->Next; //Переходим на следующий элемент
	}
	cout << "\n\n";
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------

Tree BD; //База данных ГАИ

void NewOffense(char* number1, char* str) //
{
	int i;
		
	Elem * temp;
	temp = BD.Search(BD.GetRoot(), number1);
	if (temp == 0) {
		temp = new Elem;
		for ( i = 0; i < 9; i++ ) {
			temp->Number[i] = number1[i];
		}
		temp->list = new ListOffense;
		temp->list->AddOffense(str);
		//Добавление записи
		BD.Insert(temp);
	}
	else temp->list->AddOffense(str);
}

void main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	/*char N[8]; //Номер машины
	char Offe1[] = "Проезд на красный цвет"; //Правонарушение
	cout << "Введите номер машины: ";
	cin >> N;*/
		
	//NewOffense(N, Offe1); //Произошло новое правонарушение 

	//BD.Print(BD.GetRoot()); //Вывод результатов
	//cout << "-------------------------------------------" << endl;
	NewOffense("СА1111ВА", "Нарушение1");
	NewOffense("СА2222ВА", "Нарушение2");
	NewOffense("СА5555ВА", "Нарушение5");
	NewOffense("СА3333ВА", "Нарушение3");
	NewOffense("СА4444ВА", "Нарушение4_1");
	NewOffense("СА4444ВА", "Нарушение4_2");
	

	BD.Print(BD.GetRoot());
	cout << "--------------------------------" << endl;
	Elem* A = new Elem;
	A = BD.Search(BD.GetRoot(), "СА4444ВА");
	cout << A->Number << endl;

	system("pause");
}