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

	// Не пользовательские методы
	Node* getMinNode(Node* node);									// Поиск эл-а с минимальным ключом
	Node* getMaxNode(Node* node);									// Поиск эл-а с максимальным ключом
	void calculateTreeBalance();									// Пересчёт параметра балансировки всего дерева
	int calculateNodeBalance(Node*& node);							// Расчёт балансировки элемента
	Node* balance(Node* node);										// Балансировка поддерева
	Node* llRotate(Node* node);
	Node* rrRotate(Node* node);
	Node* lrRotate(Node* node);
	Node* rlRotate(Node* node);
	void L_t_R(Node*& root, std::queue<Node*>& queue);				// Для прямого хода
	void L_t_R(Node*& root, std::stack<Node*>& stack);				// Для обратного хода

public:
	Node* root = nullptr;
	int size = 0;
	int viewedNodes = 0;

	Tree();															// Конструктор
	~Tree();														// Деструктор
	int getSize();													// Получение размера
	void clear(Node*& root);										// Очистка
	bool isEmpty();													// Проверка на пустоту
	// Основные методы
	Data read(Key key);												// Поиск элемента
	void edit(Key key, Data newData, Node*& root);					// Изменение элемента
	void addB(Key key, Data data, Node*& root);						// Добавление элемента с балансировкой
	void deleteNodeB(Key key, Node*& root);							// Удаление элемента с балансировкой
	// Дополнительные методы
	int getNodeHeight(Node*& node);									// Получение высоты элемента
	void t_Lt_Rt(Node*& root);										// Вывод последовательности ключей по схеме t_Lt_Rt
	// Отладочные методы
	bool isContain(Key key, Node*& root);							// Содержит ли дерево значение
	int getViewedNodes();											// Получить кол-во просмотренных эл-ов последней операцией
	void resetViewed();												// Сброс кол-ва просмотренных эл-ов
	void printTreeH(Node*& root, int indent = 0);					// Горизонтальный вывод

	// Итераторы
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
inline Tree<Data, Key>::Node* Tree<Data, Key>::getMinNode(Node* node)
{
	while (node)
	{
		if (node->left)
			node = node->left;
		else
			return node;
	}
}

template<typename Data, typename Key>
inline Tree<Data, Key>::Node* Tree<Data, Key>::getMaxNode(Node* node)
{
	while (node)
	{
		if (node->right)
			node = node->right;
		else
			return node;
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
	// Не нашли элемент
	throw std::exception("Исключение");
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
	// Вставка элемента в дерево
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
		// Ключ уже есть в дереве
		else
			return;
	}
	// размер дерева > 0
	if (parent)
	{
		if (key < parent->key)
			connectionPtr = &parent->left;
		else
			connectionPtr = &parent->right;
		*connectionPtr = new Node(key, data, nullptr, nullptr, parent);
		// Возвращение обратно и проверка баланса
		current = *connectionPtr;
 		while (current)
		{
			if (abs(calculateNodeBalance(current)) > 1) 
				current = balance(current);
			current = current->parent;
		}
	}
	else
		root = new Node(key, data, nullptr, nullptr, parent);
	size++;
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::deleteNodeB(Key key, Node*& root)
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
		// Отсутствует элемент в дереве
		if (!current)
			return;
	}
	// Устанавливаем указатель на связь
	if (parent)
	{
		if (key < parent->key)
			connectionPtr = &parent->left;
		else
			connectionPtr = &parent->right;
	}
	// Корректировка связей
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
		deleteNodeB(tempKey, root);
		current->data = tempData;
		current->key = tempKey;
		// Размер не увеличиваем
		size++;
	}
	// Возвращение обратно и проверка баланса
	if (connectionPtr)
		current = *connectionPtr;
	while (current)
	{
		if (abs(calculateNodeBalance(current)) > 1)
			current = balance(current);
		current = current->parent;
	}
	size--;
}

// Рекурсивный вывод
template<typename Data, typename Key>
inline void Tree<Data, Key>::printTreeH(Node*& root, int indent)
{
	calculateTreeBalance();
	if (!root)
	{
		std::cout << "Дерево пустое" << std::endl;
		return;
	}
	if (root->right)
		printTreeH(root->right, indent + 4);
	if (indent)
		std::cout << std::setw(indent) << ' ';
	if (root->right)
		std::cout << " /\n" << std::setw(indent) << ' ';
	std::cout << root->key << ":" << root->bal << std::endl;
	if (root->left)
	{
		std::cout << std::setw(indent) << ' ' << " \\\n";
		printTreeH(root->left, indent + 4);
	}
}

template<typename Data, typename Key>
inline void Tree<Data, Key>::calculateTreeBalance()
{
	Node* current = nullptr;
	std::queue<Node*> queue;
	queue.push(root);
	while(!queue.empty())
	{
		current = queue.front();
		queue.pop();
		current->bal = calculateNodeBalance(current);
		if (current->left)
			queue.push(current->left);
		if (current->right)
			queue.push(current->right);
	}
}

template<typename Data, typename Key>
inline int Tree<Data, Key>::calculateNodeBalance(Node*& node)
{
	return getNodeHeight(node->right) - getNodeHeight(node->left);
}

template<typename Data, typename Key>
inline Tree<Data, Key>::Node* Tree<Data, Key>::balance(Node* node)
{
	Node* temp = nullptr;
	int bal = calculateNodeBalance(node);
	// Левый потомок несбалансирован
	if (bal < -1) {
		// Если левое поддерево потомка больше или равно правому
		if (getNodeHeight(node->left) >= getNodeHeight(node->right))
			temp = rrRotate(node);
		else
			temp = rlRotate(node);
	}
	// Правый потомок несбалансирован
	else if (bal > 1) {
		// Если правое поддерево потомка больше или равно левому
		if (getNodeHeight(node->right) >= getNodeHeight(node->left))
			temp = llRotate(node);
		else
			temp = lrRotate(node);
	}
	return temp;
}

template<typename Data, typename Key>
inline Tree<Data, Key>::Node* Tree<Data, Key>::llRotate(Node* node)
{
	Node* temp = node->right;
	node->right = temp->left;
	temp->left = node;
	// Родители
	temp->parent = node->parent;
	if (node->parent)
	{
		if (node->key < node->parent->key)
			node->parent->left = temp;
		else
			node->parent->right = temp;
	}
	else
		root = temp;
	if (node->right)
		node->right->parent = node;
	if (temp->left)
		temp->left->parent = temp;
	return temp;
}

template<typename Data, typename Key>
inline Tree<Data, Key>::Node* Tree<Data, Key>::rrRotate(Node* node)
{
	Node* temp = node->left;
	node->left = temp->right;
	temp->right = node;
	// Родители
	temp->parent = node->parent;
	if (node->parent)
	{
		if (node->key < node->parent->key)
			node->parent->left = temp;
		else
			node->parent->right = temp;
	}
	else
		root = temp;
	if (node->left)
		node->left->parent = node;
	if (node->right)
		temp->right->parent = temp;
	return temp;
}

template<typename Data, typename Key>
inline Tree<Data, Key>::Node* Tree<Data, Key>::lrRotate(Node* node)
{
	Node* temp;
	temp = node->right;
	node->right = rrRotate(temp);
	return llRotate(node);
}

template<typename Data, typename Key>
inline Tree<Data, Key>::Node* Tree<Data, Key>::rlRotate(Node* node)
{
	Node* temp;
	temp = node->left;
	node->left = llRotate(temp);
	return rrRotate(node);
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
inline int Tree<Data, Key>::getNodeHeight(Node*& node)
{
	int h1 = 0, h2 = 0;
	if (!node)
		return 0;
	if (node->left)
		h2 = getNodeHeight(node->left);
	if (node->right)
		h1 = getNodeHeight(node->right);
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
	// Если предыдущий элемент в рекурсии совпадает с итератором
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
	// Если предыдущий элемент в рекурсии совпадает с итератором
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

		// Если вышли за предел дерева
		if (queue.empty())
		{
			// Если вышли за предел дерева
			throw std::exception("Исключение");
		}
		// Если элемент совпадает с итератором
		// то следующий элемент итератора в коллекции
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
		// Если вышли за предел дерева
		if (stack.empty())
		{
			// Если вышли за предел дерева
			throw std::exception("Исключение");
		}
		// Если элемент совпадает с итератором
		// то следующий элемент итератора в коллекции
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
		// Если вышли за предел дерева
		if (stack.empty())
		{
			// Если вышли за предел дерева
			throw std::exception("Исключение");
		}
		// Если элемент совпадает с итератором
		// то следующий элемент итератора в коллекции
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

		// Если вышли за предел дерева
		if (queue.empty())
		{
			// Если вышли за предел дерева
			throw std::exception("Исключение");
		}
		// Если элемент совпадает с итератором
		// то следующий элемент итератора в коллекции
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