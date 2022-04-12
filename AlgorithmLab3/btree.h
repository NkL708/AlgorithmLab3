#pragma once
#include <iostream>
#include <iomanip>
#include <stack>
#include <queue>

template <typename Data, typename Key>
class Tree
{
	struct Node
	{
		Key key;
		Data data;
		Node* right;
		Node* left;
		Node* parent;
		int consoleW;
		int bal;

		Node(Key key, Data data = nullptr, Node* right = nullptr,
			Node* left = nullptr, Node* parent = nullptr, int bal = 0, int consoleW = 0);
	};

	// �� ���������������� ������
	Node* getMinNode(Node* root);									// ����� ��-� � ����������� ������
	Node* getMaxNode(Node* root);									// ����� ��-� � ������������ ������
	void recalculateTreeBalance();									// �������� ��������� ������������ ������
	void rebalanceTree();											// ������������ ������
	void add(Key key, Data data, Node*& root);						// ���������� �������� ��� ������������
	void deleteNode(Key key, Node*& root);							// �������� �������� ��� ������������
	void L_t_R(Node*& root, std::queue<Node*>& queue);				// ��� ������� ����
	void L_t_R(Node*& root, std::stack<Node*>& stack);				// ��� ��������� ����

public:
	Node* root = nullptr;
	int size = 0;
	int viewedNodes = 0;

	Tree();															// �����������
	~Tree();														// ����������
	int getSize();													// ��������� �������
	void clear(Node*& root);										// �������
	bool isEmpty();													// �������� �� �������
	// �������� ������
	Data read(Key key);												// ����� ��������
	void edit(Key key, Data newData, Node*& root);					// ��������� ��������
	void addB(Key key, Data data, Node*& root);						// ���������� �������� � �������������
	void deleteNodeB(Key key, Node*& root);							// �������� �������� � �������������
	// �������������� ������
	int getNodeHeight(Node*& root);									// ��������� ������ ��������
	void t_Lt_Rt(Node*& root);										// ����� ������������������ ������ �� ����� t_Lt_Rt
	// ���������� ������
	bool isContain(Key key, Node*& root);							// �������� �� ������ ��������
	int getViewedNodes();											// �������� ���-�� ������������� ��-�� ��������� ���������
	void resetViewed();												// ����� ���-�� ������������� ��-��
	void printTreeH(Node*& root, int indent = 0);					// �������������� �����

	// ���������
	class Iterator
	{
	public:
		Node* nodePtr;
		Tree* treePtr;

		Iterator(Node* nodePtr = nullptr, Tree* treePtr = nullptr);
		Data& operator*();
		void operator++(int);
		void operator--(int);
		bool operator==(const Iterator& it);
		bool operator!=(const Iterator& it);
	};
public:
	Iterator begin();
	Iterator end();

	class ReverseIterator
	{
	public:
		Node* nodePtr;
		Tree* treePtr;

		ReverseIterator(Node* nodePtr = nullptr, Tree* treePtr = nullptr);
		Data& operator*();
		void operator++(int);
		void operator--(int);
		bool operator ==(const ReverseIterator& it);
		bool operator !=(const ReverseIterator& it);
	};

	ReverseIterator rBegin();
	ReverseIterator rEnd();
};

template<typename Data, typename Key>
inline Tree<Data, Key>::Node::Node(Key key, Data data, Node* right, Node* left, Node* parent, int bal, int consoleW)
{
	this->key = key;
	this->data = data;
	this->right = right;
	this->left = left;
	this->parent = parent;
	this->bal = bal;
	this->consoleW = consoleW;
}

template<typename Data, typename Key>
inline Tree<Data, Key>::Node* Tree<Data, Key>::getMinNode(Node* root)
{
	while (root)
	{
		if (root->left)
			root = root->left;
		else
			return root;
	}
}

template<typename Data, typename Key>
inline Tree<Data, Key>::Node* Tree<Data, Key>::getMaxNode(Node* root)
{
	while (root)
	{
		if (root->right)
			root = root->right;
		else
			return root;
	}
}

template<typename Data, typename Key>
inline Tree<Data, Key>::Tree()
{

}

template<typename Data, typename Key>
inline Tree<Data, Key>::~Tree()
{
	clear(root);
}

template<typename Data, typename Key>
inline int Tree<Data, Key>::getSize()
{
	return size;
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::clear(Node*& root)
{
	if (!root)
		return;
	if (root->left)
		clear(root->left);
	if (root->right)
		clear(root->right);
	root = nullptr;
	size--;
}

template<typename Data, typename Key>
inline bool Tree<Data, Key>::isEmpty()
{
	if (size < 0)
		return true;
	else
		return false;
}

template<typename Data, typename Key>
inline Data Tree<Data, Key>::read(Key key)
{
	Node* current = root;
	while (current)
	{
		viewedNodes++;
		if (key == current->key)
			return current->data;
		else if (key < current->key)
			current = current->left;
		else if (key > current->key)
			current = current->right;
	}
	// �� ����� �������
	throw std::exception("����������");
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::edit(Key key, Data newData, Node*& root)
{
	viewedNodes++;
	if (!root)
		return;
	if (key < root->key)
		edit(key, newData, root->left);
	if (key > root->key)
		edit(key, newData, root->right);
	else if (key == root->key)
		root->data = newData;
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::addB(Key key, Data data, Node*& root)
{
	add(key, data, root);
	recalculateTreeBalance();
	rebalanceTree();
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::deleteNodeB(Key key, Node*& root)
{
	deleteNode(key, root);
	recalculateTreeBalance();
	rebalanceTree();
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::add(Key key, Data data, Node*& root)
{
	Node* current = root;
	Node* parent = nullptr;
	Node** connectionPtr = nullptr;
	while (current)
	{
		viewedNodes++;
		if (key < current->key)
		{
			parent = current;
			current = current->left;
		}
		else if (key > current->key)
		{
			parent = current;
			current = current->right;
		}
		else
			return;
	}
	// ������ ������ ��������
	if (parent)
	{
		if (key < parent->key)
			connectionPtr = &parent->left;
		else
			connectionPtr = &parent->right;
		*connectionPtr = new Node(key, data, nullptr, nullptr, parent);
	}
	else
		root = new Node(key, data, nullptr, nullptr, parent);
	size++;
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::deleteNode(Key key, Node*& root)
{
	Node* current = root;
	Node* parent = nullptr;
	Node** connectionPtr = nullptr;
	while (key != current->key)
	{
		viewedNodes++;
		if (key < current->key)
		{
			parent = current;
			current = current->left;
		}
		else if (key > current->key)
		{
			parent = current;
			current = current->right;
		}
		// ����������� ������� � ������
		if (!current)
			return;
	}
	// ������������� ��������� �� �����
	if (parent) 
	{
		if (key < parent->key)
			connectionPtr = &parent->left;
		else
			connectionPtr = &parent->right;
	}
	// ������������� ������
	if (!current->left && !current->right)
		*connectionPtr = nullptr;
	else if (!current->left)
		*connectionPtr = current->right;
	else if (!current->right)
		*connectionPtr = current->left;
	else if (current->left && current->right) 
	{
		Node* min = getMinNode(current->right);
		Data tempData = min->data;
		Key tempKey = min->key;
		deleteNode(tempKey, root);
		current->data = tempData;
		current->key = tempKey;
		// ������ �� �����������
		size++;
	}
	size--;
}

// ����������� �����
template<typename Data, typename Key>
inline void Tree<Data, Key>::printTreeH(Node*& root, int indent)
{
	if (!root)
	{
		std::cout << "������ ������" << std::endl;
		return;
	}
	if (root->right)
		printTreeH(root->right, indent + 4);
	if (indent)
		std::cout << std::setw(indent) << ' ';
	if (root->right)
		std::cout << " /\n" << std::setw(indent) << ' ';
	std::cout << "�:" << root->key << " �:" << root->data << " �:" << root->bal << std::endl;
	if (root->left)
	{
		std::cout << std::setw(indent) << ' ' << " \\\n";
		printTreeH(root->left, indent + 4);
	}
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::recalculateTreeBalance()
{
	Node* current = nullptr;
	std::queue<Node*> queue;
	queue.push(root);
	while(!queue.empty())
	{
		current = queue.front();
		queue.pop();
		current->bal = getNodeHeight(current->right) - getNodeHeight(current->left);
		if (current->left)
			queue.push(current->left);
		if (current->right)
			queue.push(current->right);
	}
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::rebalanceTree()
{
	Node* current = nullptr;
	std::queue<Node*> queue;
	queue.push(root);
	while (!queue.empty())
	{
		current = queue.front();
		queue.pop();
		// ������������ ������� ���������
		if (current->bal > 1)
		{
			Node* min = getMinNode(current->right);
			Data tempDataMin = min->data;
			Key tempKeyMin = min->key;
			deleteNode(tempKeyMin, current);
			Data tempDataCur = current->data;
			Key tempKeyCur = current->key;
			current->data = tempDataMin;
			current->key = tempKeyMin;
			add(tempKeyCur, tempDataCur, current->left);
		}
		// ������������ ������ ���������
		else if (current->bal < -1) 
		{
			Node* max = getMaxNode(current->left);
			Data tempDataMax = max->data;
			Key tempKeyMax = max->key;
			deleteNode(tempKeyMax, current);
			Data tempDataCur = current->data;
			Key tempKeyCur = current->key;
			current->data = tempDataMax;
			current->key = tempKeyMax;
			add(tempKeyCur, tempDataCur, current->right);
		}
		if (current->left)
			queue.push(current->left);
		if (current->right)
			queue.push(current->right);
	}
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::t_Lt_Rt(Node*& root)
{
	if (!root)
		return;
	std::cout << root->key << " ";
	t_Lt_Rt(root->left);
	t_Lt_Rt(root->right);
}

template<typename Data, typename Key>
inline int Tree<Data, Key>::getNodeHeight(Node*& root)
{
	int h1 = 0, h2 = 0;
	if (!root)
		return 0;
	if (root->left)
		h2 = getNodeHeight(root->left);
	if (root->right)
		h1 = getNodeHeight(root->right);
	return (std::max(h1, h2) + 1);
}

template<typename Data, typename Key>
inline bool Tree<Data, Key>::isContain(Key key, Node*& root)
{
	if (!root)
		return false;
	else if (key < root->key)
		return isContain(key, root->left);
	else if (key > root->key)
		return isContain(key, root->right);
	else
		return true;
}

template<typename Data, typename Key>
inline int Tree<Data, Key>::getViewedNodes()
{
	return viewedNodes;
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::resetViewed()
{
	viewedNodes = 0;
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::L_t_R(Node*& root, std::queue<Node*>& queue)
{
	if (!root)
		return;
	if (root->left)
		L_t_R(root->left, queue);
	// ���� ���������� ������� � �������� ��������� � ����������
	queue.push(root);
	if (root->right)
		L_t_R(root->right, queue);
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::L_t_R(Node*& root, std::stack<Node*>& stack)
{
	if (!root)
		return;
	if (root->left)
		L_t_R(root->left, stack);
	// ���� ���������� ������� � �������� ��������� � ����������
	stack.push(root);
	if (root->right)
		L_t_R(root->right, stack);
}

template<typename Data, typename Key>
inline Tree<Data, Key>::Iterator Tree<Data, Key>::begin()
{
	Iterator it{};
	if (this->size)
		it.nodePtr = getMinNode(root);
	it.treePtr = this;
	return it;
}

template<typename Data, typename Key>
inline Tree<Data, Key>::Iterator Tree<Data, Key>::end()
{
	Iterator it{};
	if (this->size)
		it.nodePtr = getMaxNode(root);
	it.treePtr = this;
	return it;
}

template<typename Data, typename Key>
inline Tree<Data, Key>::Iterator::Iterator(Node* nodePtr, Tree* treePtr)
{
	this->nodePtr = nodePtr;
	this->treePtr = treePtr;
}

template<typename Data, typename Key>
inline Data& Tree<Data, Key>::Iterator::operator*()
{
	return this->nodePtr->data;
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::Iterator::operator++(int)
{
	std::queue<Node*> queue;
	this->treePtr->L_t_R(this->treePtr->root, queue);
	Node* current;
	while (!queue.empty())
	{
		current = queue.front();
		queue.pop();

		// ���� ����� �� ������ ������
		if (queue.empty())
		{
			// ���� ����� �� ������ ������
			throw std::exception("����������");
		}
		// ���� ������� ��������� � ����������
		// �� ��������� ������� ��������� � ���������
		else if (current == this->nodePtr)
		{
			this->nodePtr = queue.front();
			return;
		}
	}
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::Iterator::operator--(int)
{
	std::stack<Node*> stack;
	this->treePtr->L_t_R(this->treePtr->root, stack);
	Node* current;
	while (!stack.empty())
	{
		current = stack.top();
		stack.pop();
		// ���� ����� �� ������ ������
		if (stack.empty())
		{
			// ���� ����� �� ������ ������
			throw std::exception("����������");
		}
		// ���� ������� ��������� � ����������
		// �� ��������� ������� ��������� � ���������
		else if (current == this->nodePtr)
		{
			this->nodePtr = stack.top();
			return;
		}
	}
}

template<typename Data, typename Key>
inline bool Tree<Data, Key>::Iterator::operator==(const Iterator& it)
{
	if (nodePtr == it.nodePtr)
		return true;
	else
		return false;
}

template<typename Data, typename Key>
inline bool Tree<Data, Key>::Iterator::operator!=(const Iterator& it)
{
	if (nodePtr != it.nodePtr)
		return true;
	else
		return false;
}

template<typename Data, typename Key>
inline Tree<Data, Key>::ReverseIterator Tree<Data, Key>::rBegin()
{
	ReverseIterator it{};
	if (this->size)
		it.nodePtr = getMaxNode(root);
	it.treePtr = this;
	return it;
}

template<typename Data, typename Key>
inline Tree<Data, Key>::ReverseIterator Tree<Data, Key>::rEnd()
{
	ReverseIterator it{};
	if (this->size)
		it.nodePtr = getMinNode(root);
	it.treePtr = this;
	return it;
}

template<typename Data, typename Key>
inline Tree<Data, Key>::ReverseIterator::ReverseIterator(Node* nodePtr, Tree* treePtr)
{
	this->nodePtr = nodePtr;
	this->treePtr = treePtr;
}

template<typename Data, typename Key>
inline Data& Tree<Data, Key>::ReverseIterator::operator*()
{
	return this->nodePtr->data;
}


template<typename Data, typename Key>
inline void Tree<Data, Key>::ReverseIterator::operator++(int)
{
	std::stack<Node*> stack;
	this->treePtr->L_t_R(this->treePtr->root, stack);
	Node* current;
	while (!stack.empty())
	{
		current = stack.top();
		stack.pop();
		// ���� ����� �� ������ ������
		if (stack.empty())
		{
			// ���� ����� �� ������ ������
			throw std::exception("����������");
		}
		// ���� ������� ��������� � ����������
		// �� ��������� ������� ��������� � ���������
		else if (current == this->nodePtr)
		{
			this->nodePtr = stack.top();
			return;
		}
	}
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::ReverseIterator::operator--(int)
{
	std::queue<Node*> queue;
	this->treePtr->L_t_R(this->treePtr->root, queue);
	Node* current;
	while (!queue.empty())
	{
		current = queue.front();
		queue.pop();

		// ���� ����� �� ������ ������
		if (queue.empty())
		{
			// ���� ����� �� ������ ������
			throw std::exception("����������");
		}
		// ���� ������� ��������� � ����������
		// �� ��������� ������� ��������� � ���������
		else if (current == this->nodePtr)
		{
			this->nodePtr = queue.front();
			return;
		}
	}
}

template<typename Data, typename Key>
inline bool Tree<Data, Key>::ReverseIterator::operator==(const ReverseIterator& it)
{
	if (nodePtr == it.nodePtr)
		return true;
	else
		return false;
}

template<typename Data, typename Key>
inline bool Tree<Data, Key>::ReverseIterator::operator!=(const ReverseIterator& it)
{
	if (nodePtr != it.nodePtr)
		return true;
	else
		return false;
}