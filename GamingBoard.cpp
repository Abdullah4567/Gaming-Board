#include <iostream>
#include<queue>
#include"rectangles.h"
#include"myconsole.h"
#include<Windows.h>
#include<conio.h>
using namespace std;
int MaxX = 0, MaxY = 0;
template<typename T>
struct Node
{
	T* Rectangle;
	Node* Left;
	Node* Right;
	int ID;
	Node(Node* L = NULL, Node* R = NULL)
	{
		this->Rectangle = new T();
		this->Left = L; this->Right = R; this->ID = 0;
		//cout << " Node Created () :-\n";
	}
	Node(int topX, int topY, int bottomX, int bottomY)
	{
		//rectangles R1;
		this->Rectangle = new T(topX, topY, bottomX, bottomY);
		//cout << a <<endl;
		this->Left = this->Right = NULL; this->ID = 0;
		//cout << " Node Created () :-\n";
	}
	void DrawCorridorsAndRooms()
	{
		this->Rectangle->DrawShrinkedRectangle();
	}
	void Draw()
	{
		this->Rectangle->draw();
	}
	void Split(Node<T>* L, Node<T>* R)
	{
		this->Rectangle->split(*L->Rectangle, *R->Rectangle);
	}
	bool isLeaf()
	{
		return !this->Left && !this->Right;
	}
	~Node()
	{
		delete this->Rectangle;
		this->Rectangle = NULL;
		this->Left = this->Right = NULL;
	}
};
template<typename T>
class GamingBoard
{
	Node<T>* Root;
	bool InsertPrivate(Node<T>* N)
	{
		queue<Node<T>*> Q;
		Q.push(this->Root); int Num = 1;
		Q.front()->ID = Num;
		for (int i = 0; i < Q.size(); i++)
		{
			Node<T>* Temp = Q.front(); Q.pop();
			if (Temp->Left)
			{
				Q.push(Temp->Left);
				Num++;
			}
			else
			{
				Temp->Left = N;
				Temp->Left->ID = ++Num;  // Assigning Number in Level order Traversal
				return true;
			}
			if (Temp->Right)
			{
				Q.push(Temp->Right);
				Num++;
			}
			else
			{
				Temp->Right = N;
				Temp->Right->ID = ++Num;
				return true;
			}
		}
	}
	Node<T>* IsInTree(Node<T>* Temp, int N, Node<T>*& Parent)
	{
		if (!Temp)
		{
			return NULL;
		}
		if (Temp->ID == N)
		{
			return Temp;
		}
		Node<T>* A = IsInTree(Temp->Left, N, Temp);
		if (A)
		{
			Parent = Temp;
			return A;
		}
		Node<T>* B = IsInTree(Temp->Right, N, Temp);
		if (B)
		{
			Parent = Temp;
			return B;
		}
	}
	int CalculateHeight(Node<T>* Temp)
	{
		if (!Temp)
		{
			return -1; // Root will be at height 0
		}
		return 1 + max(CalculateHeight(Temp->Left), CalculateHeight(Temp->Right));
	}
	void DeleteSubTree(Node<T>*& Temp)
	{

		if (Temp)
		{
			this->DeleteSubTree(Temp->Left);
			this->DeleteSubTree(Temp->Right);
			delete Temp;
			//N++;
			Temp = NULL;
		}
	}

public:
	GamingBoard()
	{
		//(3, 3, 100, 30);

		//rectangles(int topX, int topY, int bottomX, int bottomY)
		this->Root = new Node<rectangles>(1, 1, MaxX, MaxY - 4); // Coordinates can be adjusted according to Console Size
	}
	GamingBoard(int Height)
	{
		// Overloaded Constructor
		this->Root = new Node<rectangles>(1, 1, MaxX, MaxY - 4); // Coordinates can be adjusted according to Console Size
		if (Height > 0)
		{
			PlaceCursor(MaxX / 2 - 30, 6);
			cout << " Creating Tree of Given Height.... \n";

			while (this->GetHeight() != Height)
			{
				//i++;
				this->Split();
			}

			PlaceCursor(MaxX / 2 - 20, 8);
			cout << " Tree Completed \n";
		}
		else
		{
			PlaceCursor(MaxX / 2 - 30, 6);
			cout << " Invalid Height of Tree ... \n";
			PlaceCursor(MaxX / 2 - 30, 8);
			cout << " Default Screen Created ... \n";
			this->Root = new Node<rectangles>(1, 1, MaxX, MaxY - 4); // Coordinates can be adjusted according to Console Size
		}
		Sleep(800);
	}
	void DrawBoard()
	{
		queue<Node<T>*> Q;
		Q.push(this->Root);
		bool End = false;
		Q.front()->Draw(); // Draw rectangle stored in Root
		while (true)
		{
			Node <T>* Temp = Q.front(); Q.pop();
			if (Temp->Left)
			{
				Q.push(Temp->Left);
				Temp->Left->Draw();
			}
			else
			{
				End = true;
			}
			if (Temp->Right)
			{
				End = false;
				Q.push(Temp->Right);
				Temp->Right->Draw();
			}
			if (End)
			{
				break;
			}
		}
	}
	void DrawCorridorsAndRooms()
	{
		queue<Node<T>*> Q; bool End = false;
		Q.push(this->Root);
		while (true)
		{
			Node <T>* Temp = Q.front(); Q.pop();
			if (Temp->Left)
			{
				Q.push(Temp->Left);
				if (Temp->Left->isLeaf())
				{
					Temp->Left->DrawCorridorsAndRooms();
				}
			}
			else
			{
				End = true;
			}
			if (Temp->Right)
			{
				Q.push(Temp->Right);
				End = false;
				if (Temp->Right->isLeaf())
				{
					Temp->Right->DrawCorridorsAndRooms();
				}
			}
			if (End)
			{
				break;
			}
		}
	}
	bool GetRectangleByNodeNumber(int NodeNumber)
	{
		Node<T>* Parent = NULL;
		Node<T>* Temp = IsInTree(this->Root, NodeNumber, Parent);
		if (Temp)
		{
			Temp->Draw();
			return true;
		}
		else
		{
			return false;
		}
	}
	int GetHeight()
	{
		return (this->CalculateHeight(this->Root));
	}
	void Split(Node<T>* Curr = NULL)
	{
		if (!Curr)
		{
			queue<Node<T>*> Q;
			Q.push(this->Root);
			Node <T>* Temp = NULL;
			while (true) // Finding the Rectangle which will be splitted 
			{
				Temp = Q.front(); Q.pop();
				if (Temp->Left)
				{
					Q.push(Temp->Left);
					//Temp->Left->Draw();
				}
				else
				{
					break;
				}
				if (Temp->Right)
				{
					Q.push(Temp->Right);
					//Temp->Right->Draw();
				}
			}
			Curr = Temp;
		}
		Node<T>* L = new Node <rectangles>;
		Node<T>* R = new Node <rectangles>;
		Curr->Split(L, R);
		this->InsertPrivate(R);
		this->InsertPrivate(L);
	}
	int Insert(int NodeNumber)
	{
		Node<T>* Parent = NULL;
		Node<T>* Temp = IsInTree(this->Root,NodeNumber, Parent);
		if (Temp)
		{
			if (Temp->isLeaf())
			{
				this->Split(Temp);
				return 1;
			}
			else
			{
				return 2;
			}
		}
		else
		{
			return 3;
		}
	}
	int DeletNode(int NodeNumber)
	{
		if (this->Root->ID == NodeNumber) // if target Node is Root of tree
		{
			return 2;
			// It is not Logical to delete the Main Rectangle because
			//Gaming Board Must have atleast 1 Rectangle as Main Rectangle 
		}
		else
		{
			Node<T>* Parent = NULL;
			Node<T>* Temp = IsInTree(this->Root, NodeNumber, Parent);
			if (Temp)
			{
				if (Parent->Left == Temp)
				{
					this->DeleteSubTree(Temp);
					Parent->Left = NULL;
				}
				else if (Parent->Right == Temp)
				{
					this->DeleteSubTree(Temp);
					Parent->Right = NULL;
				}
				return 1;
			}
			else
			{
				return 3;
			}
		}

	}
	~GamingBoard()
	{
		this->DeleteSubTree(this->Root);
		this->Root = NULL;
	}
};
int StartingMenu()
{
	// Return the initial Height of Tree
	int Height = 0;
	PlaceCursor(MaxX / 2 - 30, 2);
	cout << "---------- Welcome to Gaming Board ---------- \n";

	PlaceCursor(MaxX / 2 - 40, 4);
	cout << " Input Initial Height (Non negative Number) of Tree : "; cin >> Height;
	return Height;
}
void Header()
{
	/*PlaceCursor(MaxX / 2 - 30, 2);
	cout << " Press any Key to Continue ... ";*/
	//cin.ignore();
	char ch=_getch();
}
int Menu()
{
	int Choice = 0;
	ClearScreen();
	PlaceCursor(MaxX / 2 - 30, 4);
	cout << "---------- Options ---------- \n";

	PlaceCursor(MaxX / 2 - 15, 7);
	cout << " 1- Draw Gaming Board \n";

	PlaceCursor(MaxX / 2 - 15, 9);
	cout << " 2- Draw Corridors and Rooms of Board \n";

	PlaceCursor(MaxX / 2 - 15, 11);
	cout << " 3- Draw Particular Rectangle by Node Number \n";

	PlaceCursor(MaxX / 2 - 15, 13);
	cout << " 4- Height of Tree \n";

	PlaceCursor(MaxX / 2 - 15, 15);
	cout << " 5- Insert Node by Number \n";

	PlaceCursor(MaxX / 2 - 15, 17);
	cout << " 6- Delete Node by Number \n";

	PlaceCursor(MaxX / 2 - 15, 19);
	cout << " 7- Split Node \n";

	PlaceCursor(MaxX / 2 - 15, 21);
	cout << " 8- Exit \n";

	PlaceCursor(MaxX / 2 - 22, 24);
	cout << " Your Choice : "; cin >> Choice;
	ClearScreen();
	return Choice;
}
void main()
{
	Maximized();
	GetMaxWindowCoordinates(MaxX, MaxY);
	bool Terminator = false; int N = 0,Res=0;
	GamingBoard<rectangles> B1(StartingMenu());
	while (!Terminator)
	{
		switch (Menu())
		{
		case 1: // Gaming Board
			B1.DrawBoard();
			Header();
			break;
		case 2: // Corridors and Rooms
			B1.DrawCorridorsAndRooms(); 
			Header();
			break;
		case 3: // Rectangle by Node Number
			N = 0;
			PlaceCursor(MaxX / 2 - 30, 1);
			cout << " Enter the Node Number  : "; cin >> N;
			if (!(B1.GetRectangleByNodeNumber(N)))
			{
				ClearScreen();
				PlaceCursor(MaxX / 2 - 30, 1);
				cout << " Given Node Number Does not Exist:-\n";
			}
			Header();
			break;
		case 4: // Height
			PlaceCursor(MaxX / 2 - 30, 1);
			cout << " Height of Tree : " << B1.GetHeight() << endl;
			Header();
			break;
		case 5: // Insertion
			N = 0;
			PlaceCursor(MaxX / 2 - 30, 1);
			cout << " Enter the Leaf Node Number : "; cin >> N;
			 Res = B1.Insert(N);
			switch (Res)
			{
			case 1: // Node Inserted

				ClearScreen();
				PlaceCursor(MaxX / 2 - 30, 1);
				cout << " Node Inserted Successfully \n";
				break;
			case 2: //cout << " Node Number is not Leaf Node .. Action Failed :-\n";

				ClearScreen();
				PlaceCursor(MaxX / 2 - 30, 1);
				cout << " Node Number is not Leaf Node .. Action Failed :-\n";
				break;
			case 3: // cout << " No Node Exist against this Node Number :-\n";

				ClearScreen();
				PlaceCursor(MaxX / 2 - 30, 1);
				cout << " No Node Exist against this Node Number :-\n";
				break;
			}
			Header();
			break;
		case 6: // Deletion
			N = 0;
			PlaceCursor(MaxX / 2 - 30, 1);
			cout << " Enter the Node Number you want to Delete : "; cin >> N;
			 Res=B1.DeletNode(N);
			switch (Res)
			{
			case 1: // Node Deleted

				ClearScreen();
				PlaceCursor(MaxX / 2 - 30, 1);
				cout << " Node Deleted Successfully \n";
				break;
			case 2: //cout << " Root Node cannot be Deleted:-\n";

				ClearScreen();
				PlaceCursor(MaxX / 2 - 30, 1);
				cout << " Root Node Cannot be Deleted .. Action Failed :-\n";
				break;
			case 3: // cout << " No Node Exist against this Node Number :-\n";

				ClearScreen();
				PlaceCursor(MaxX / 2 - 30, 1);
				cout << " No Node Exist against this Node Number :-\n";
				break;
			}
			Header();
			break;
		case 7: // Split
			B1.Split();
			PlaceCursor(MaxX / 2 - 30, 1);
			cout << " Action Done Succesfully :-\n";
			Header();
			break;
		case 8: // Exit
			Terminator = true;
			break;
		default:
			PlaceCursor(MaxX / 2 - 30, 1);
			cout << " Invalid Choice... \n";
		}
	}
	ClearScreen();
	PlaceCursor(MaxX / 2 - 30, 10);
	cout << " Good Bye ... \n";
}