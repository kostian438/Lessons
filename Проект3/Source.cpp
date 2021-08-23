#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

using namespace std;

struct Offense //���� ��������������
{
	char* offense;
	Offense * Next;
};

class ListOffense
{
	Offense * Head;
	Offense * Tail;
	int Count; //���������� ��������� ������
public:
	ListOffense();
	~ListOffense();
	void AddOffense(char* data); //(����� ������� ���������� ���������)
	void DelOffense(); //(��������� �������� �������)
	void DelAllOffense(); //�������� ����� ������
	void PrintOffense(); //(���������� ���������� � ��������� ��������)
	int GetCount(); //��������� ���������� ��������� ������
	
};

struct Elem
{
	char Number[9]; //����� ������
	ListOffense* list; //������ ��������������
	Elem * left, *right, *parent;
};

class Tree {
	Elem * root; //������
public:
	Tree();
	~Tree();
	void Print(Elem * Node); //������ �� ���������� ����
	Elem * Search(Elem * Node, char* key); //����� �� ���������� ����
	Elem * Min(Elem * Node); //min �� ���������� ����
	Elem * Max(Elem * Node); //max �� ���������� ����
	Elem * Next(Elem * Node); //��������� ��� ���������� ����
	Elem * Previous(Elem * Node); //���������� ��� ���������� ����
	void Insert(Elem * z); //������� ����
	void Del(Elem * z = 0); //�������� ����� ��� ���������� ����, 0 - �������� ����� ������
	Elem * GetRoot(); //�������� ������
};

Tree::Tree() { root = NULL; }

Tree::~Tree() { cout << "��� ���������� ��!";  Del(); system("pause");}

void Tree::Print(Elem * Node) //����������� ����� ������
{
	if (Node != 0)
	{
		Print(Node->left);
		cout << "����� ������: ";
		for (int i = 0; i < 8; i++) {
			cout << Node->Number[i];
		}
		cout << endl;
		cout << "���������: ";
		Node->list->PrintOffense();
		
		Print(Node->right);
	}
}

Elem * Tree::Search(Elem * Node, char* k)
{
	//���� ���� ���� � ����� �� ���������
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
	//����� ������ "������" ����
	if (Node != 0)
		while (Node->left != 0)
			Node = Node->left;
	return Node;
}

Elem * Tree::Max(Elem * Node)
{
	//����� ������ "�������" ����
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
		//���� ���� ������ �������
		if (Node->right != 0)
			return Min(Node->right);
		//�������� ����
		y = Node->parent;
		//���� Node �� ������ � Node ������
		while (y != 0 && Node == y->right)
		{
			//�������� �����
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
		//���� ���� ����� �������
		if (Node->left != 0)
			return Max(Node->left);
		//�������� ����
		y = Node->parent;
		//���� Node �� ������ � Node �����
		while (y != 0 && Node == y->left)
		{
			//�������� �����
			Node = y;
			y = y->parent;
		}
	}
	return y;
}

Elem * Tree::GetRoot() { return root; }

void Tree::Insert(Elem * z)
{
	//�������� ���
	z->left = NULL;
	z->right = NULL;
	Elem * y = NULL;
	Elem * Node = root;
	//����� �����
	while (Node != 0)
	{
		//������� ��������
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
	//��������� ��������
	z->parent = y;
	if (y == 0) //������� ������ (������������)
		root = z;
	//��� ���� ������?
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
	//�������� �����
	if (z != 0)
	{
		Elem * Node, *y;
		//�� 2 �������
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
		//��������� �������� ����?
		if (y->parent == 0)
			root = Node;
		else if (y == y->parent->left)
			//����� �� ��������?
			y->parent->left = Node;
		else
			//������ �� ��������?
			y->parent->right = Node;
		if (y != z)
		{
			//����������� ������ ����
			for (int i = 0; i < 8; i++) {
				z->Number[i] = y->Number[i];
			}
			z->list = new ListOffense;
			*z->list = *y->list;
		}
		y->list->DelAllOffense();
		delete y;
	}
	
	else //�������� ����� ������
		while (root != 0)
			Del(root);
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//------------������ ������ ��������������------------------------------

ListOffense::ListOffense()
{
	Head = Tail = NULL; //���������� ������ ����
	Count = 0;
}
ListOffense::~ListOffense()
{
	DelAllOffense(); //����� ������� ��������
}
int ListOffense::GetCount()
{
	return Count;
}
void ListOffense::AddOffense(char* data)
{
	Offense * temp = new Offense; //�������� ������ ��������
	int len = strlen(data) + 1;
	temp->offense = new char[len];
	strcpy_s(temp->offense, len, data); //���������� �������
	temp->Next = NULL; //��������� ������� �����������
					   //����� ������� ���������� ��������� ��������� ������
					   //���� �� �� ������ �����������
	if (Head != NULL) {
		Tail->Next = temp;
		Tail = temp;
	}
	//����� ������� ���������� ������������
	//���� �� ������ �����������
	else {
		Head = Tail = temp;
	}
	Count++;

}

void ListOffense::DelOffense()
{
	Offense * temp = Head; //���������� ����� ��������� ��������
	Head = Head->Next; //������������� ������ �� ��������� �������
	delete[] temp->offense;
	delete temp; //������� ������ �������� �������
}
void ListOffense::DelAllOffense()
{
	while (Head != 0) //���� ��� ���� ��������
		DelOffense(); //������� �������� �� ������
}
void ListOffense::PrintOffense()
{
	Offense * temp = Head; //���������� ����� ��������� ��������
	while (temp != 0) //���� ��� ���� ��������
	{
		cout << temp->offense << " "; //������� ������
		temp = temp->Next; //��������� �� ��������� �������
	}
	cout << "\n\n";
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------

Tree BD; //���� ������ ���

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
		//���������� ������
		BD.Insert(temp);
	}
	else temp->list->AddOffense(str);
}

void main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	/*char N[8]; //����� ������
	char Offe1[] = "������ �� ������� ����"; //��������������
	cout << "������� ����� ������: ";
	cin >> N;*/
		
	//NewOffense(N, Offe1); //��������� ����� �������������� 

	//BD.Print(BD.GetRoot()); //����� �����������
	//cout << "-------------------------------------------" << endl;
	NewOffense("��1111��", "���������1");
	NewOffense("��2222��", "���������2");
	NewOffense("��5555��", "���������5");
	NewOffense("��3333��", "���������3");
	NewOffense("��4444��", "���������4_1");
	NewOffense("��4444��", "���������4_2");
	

	BD.Print(BD.GetRoot());
	cout << "--------------------------------" << endl;
	Elem* A = new Elem;
	A = BD.Search(BD.GetRoot(), "��4444��");
	cout << A->Number << endl;

	system("pause");
}