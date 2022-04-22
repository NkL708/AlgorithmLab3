#include <iostream>
#include "btree.h"

typedef unsigned long long INT_64;

//���������� � ��������� ���������� LineRand()
static INT_64 RRand = 15750;
const INT_64 mRand = (1 << 63) - 1;
const INT_64 aRand = 6364136223846793005;
const INT_64 cRand = 1442695040888963407;

//������� ��������� ������� ���������� ����� �� �����
//����������
void sRand()
{
	srand(time(0));
	RRand = (INT_64)rand();
}

//������� ��������� ���������� �����
//�������� ������������ ��������� Xi+1=(a*Xi+c)%m
//habr.com/ru/post/208178
INT_64 LineRand()
{
	INT_64 y1, y2;
	y1 = (aRand * RRand + cRand) % mRand;
	y2 = (aRand * y1 + cRand) % mRand;
	RRand = y1 & 0xFFFFFFFF00000000LL ^ (y2 &
		0xFFFFFFFF00000000LL) >> 32;
	return RRand;
}

void test_rand(int n)
{
	//�������� ������ ��� 64 � ��������� ������ ���� INT_64
	Tree<INT_64, int> tree;
	//������ ��� ������, ������� ������������ � ������
	INT_64* m = new INT_64[n];
	//��������� ������� ���������� �����
	sRand();
	//���������� ������ � ������� ����������
	//�� ���������� �������
	for (int i = 0; i < n; i++)
	{
		m[i] = LineRand();
		tree.resetViewed();
		tree.addB(m[i], 1, tree.root);
	}
	//����� ������� ������ �� �����
	std::cout << "items count:" << tree.getSize() << std::endl;
	//��������� ��������� ����������� �������,
	//�������� � ������
	double I = 0;
	double D = 0;
	double S = 0;
	//��������� ������ ��������, 10% - ������� ��������
	for (int i = 0; i < n / 2; i++)
	{
		if (i % 10 == 0) //10% ��������
		{
			tree.resetViewed();
			tree.deleteNodeB(LineRand(), tree.root);
			D += tree.getViewedNodes();

			tree.resetViewed();
			tree.addB(m[rand() % n], 1, tree.root);
			I += tree.getViewedNodes();

			try
			{
				tree.resetViewed();
				tree.read(LineRand());
			}
			catch (...) {}
			S += tree.getViewedNodes();
		}
		else //90% �������� ��������
		{
			int ind = rand() % n;
			tree.resetViewed();
			tree.deleteNodeB(m[ind], tree.root);
			D += tree.getViewedNodes();

			INT_64 key = LineRand();
			tree.resetViewed();
			tree.addB(key, 1, tree.root);
			I += tree.getViewedNodes();
			m[ind] = key;

			try
			{
				tree.resetViewed();
				tree.read(m[rand() % n]);
			}
			catch (...) {}
			S += tree.getViewedNodes();
		}
	}
	//����� �����
	//����� �����������:
	//����� ������� ������ ����� �����
	std::cout << "items count:" << tree.getSize() << std::endl;
	//������������� ������ ����������� �������� BST
	std::cout << "1.39*log2(n)=" << 1.39 * (log((double)n) / log(2.0)) << std::endl;
	//����������������� ������ ����������� �������
	std::cout << "Count insert: " << I / (n / 2) << std::endl;
	//����������������� ������ ����������� ��������
	std::cout << "Count delete: " << D / (n / 2) << std::endl;
	//����������������� ������ ����������� ������
	std::cout << "Count search: " << S / (n / 2) << std::endl;
	//������������ ������ ������� m[]
	delete[] m;
}

void test_ord(int n)
{
	//�������� ������ ��� 64 � ��������� ������ ���� INT_64
	Tree<INT_64, int> tree;
	//������ ��� ������, ������� ������������ � ������
	INT_64* m = new INT_64[n];
	//���������� ������ � ������� ����������
	// � ������������� ������� �������
	//�� ��������� [0, 10000, 20000, ... ,10000*n]
	for (int i = 0; i < n; i++)
	{
		m[i] = i * 10000;
		tree.resetViewed();
		tree.addB(m[i], 1, tree.root);
	}
	//����� ������� ������ �� �����
	std::cout << "items count:" << tree.getSize() << std::endl;
	//��������� ��������� ����������� �������,
	// �������� � ������
	double I = 0;
	double D = 0;
	double S = 0;
	//��������� ������� ���������� �����
	sRand();
	//��������� ������ ��������, 10% - ������� ��������
	for (int i = 0; i < n / 2; i++)
	{
		if (i % 10 == 0) //10% ��������
		{
			int k = LineRand() % (10000 * n);
			k = k + !(k % 2); //��������� �������� ����
			tree.resetViewed();
			tree.deleteNodeB(k, tree.root);
			D += tree.getViewedNodes();

			tree.resetViewed();
			tree.addB(m[rand() % n], 1, tree.root);
			I += tree.getViewedNodes();

			k = LineRand() % (10000 * n);
			k = k + !(k % 2); // ��������� �������� ����
			try
			{
				tree.resetViewed();
				tree.read(k);
			}
			catch (...) {}
			S += tree.getViewedNodes();
		}
		else //90% �������� ��������
		{
			int ind = rand() % n;
			tree.resetViewed();
			tree.deleteNodeB(m[ind], tree.root);
			D += tree.getViewedNodes();

			int k = LineRand() % (10000 * n);
			k = k + k % 2; // ��������� ������ ����
			tree.resetViewed();
			tree.addB(k, 1, tree.root);
			I += tree.getViewedNodes();
			m[ind] = k;

			try
			{
				tree.resetViewed();
				tree.read(m[rand() % n]);
			}
			catch (...) {}
			S += tree.getViewedNodes();
		}
	}
	//����� �����������:
	// ����� ������� ������ ����� �����
	std::cout << "items count:" << tree.getSize() << std::endl;
	//������������� ������ ����������� �������� BST
	std::cout << "n/2 =" << n / 2 << std::endl;
	//����������������� ������ ����������� �������
	std::cout << "Count insert: " << I / (n / 2) << std::endl;
	//����������������� ������ ����������� ��������
	std::cout << "Count delete: " << D / (n / 2) << std::endl;
	//����������������� ������ ����������� ������
	std::cout << "Count search: " << S / (n / 2) << std::endl;
	//������������ ������ ������� m[]
	delete[] m;
} //����� �����


int main()
{
	setlocale(LC_ALL, "Russian");
	Tree<int, INT_64> tree;
	Tree<int, INT_64>::Iterator iterator;
	Tree<int, INT_64>::ReverseIterator reverseIterator;
	int value;
	INT_64 key;
	int sw = 0;
	bool exit = false;

	// ��� �������
	//tree.addB(25, 25, tree.root);
	//tree.addB(13, 13, tree.root);
	//tree.addB(100, 100, tree.root);
	//tree.addB(15, 15, tree.root);
	//tree.addB(2, 2, tree.root);
	//tree.addB(63, 63, tree.root);
	//tree.addB(8, 8, tree.root);
	//tree.addB(42, 42, tree.root);
	//tree.deleteNodeB(42, tree.root);
	//tree.deleteNodeB(63, tree.root);
	//tree.printTreeH(tree.root);

	std::string firstMenu[] =
	{
		"\n\t1.  �������� ������ ������\n" ,
		"\t2.  �������� ������\n" ,
		"\t3.  ��������� ������ �� �������\n" ,
		"\t4.  ����� �������\n" ,
		"\t5.  �������� �������\n" ,
		"\t6.  �������� �������\n" ,
		"\t7.  ������� �������\n" ,
		"\t8.  ������� ������ � �������\n" ,
		"\t9.  �������� � ����������\n" ,
		"\t10. �������� � �������� ����������\n" ,
		"\t11. �������������� ��������\n" ,
		"\t0.  ����� �� ���������\n" ,
		"\n������� ����� ��������: "
	};

	std::string iteratorMenu[]
	{
		"\n\t1.  ������� ��������\n" ,
		"\t2.  ������� �������� ���������\n" ,
		"\t3.  �������� �������� ���������\n" ,
		"\t4.  ��������� iterator++\n" ,
		"\t5.  ��������� iterator--\n" ,
		"\t6.  ���������, ����� �� �������� begin()\n" ,
		"\t7.  ���������, ����� �� �������� end()\n" ,
		"\t8.  ���������, �� ����� �� �������� begin()\n" ,
		"\t9. ���������, �� ����� �� �������� end()\n" ,
		"\t0. ��������� �����\n" ,
		"\n������� ����� ��������: "
	};

	std::string reverseIteratorMenu[]
	{
		"\n\t1. ������� �������� ��������\n" ,
		"\t2. ������� �������� ��������� ���������\n" ,
		"\t3. �������� �������� ��������� ���������\n" ,
		"\t4. ��������� reverseIterator++\n" ,
		"\t5. ��������� reverseIterator--\n" ,
		"\t6. ���������, ����� �� �������� �������� rbegin()\n" ,
		"\t7. ���������, ����� �� �������� rend()\n" ,
		"\t8. ���������, �� ����� �� �������� �������� rbegin()\n" ,
		"\t9. ���������, �� ����� �� �������� �������� rend()\n" ,
		"\t0. ��������� �����\n" ,
		"\n������� ����� ��������: "
	};

	std::string extraMenu[]
	{
		"\n\t1. ������� ����� ������� t_Lt_Rt\n" ,
		"\t2. ���������� ������ ������\n" ,
		"\t3. ���� ����������� ���������� ������\n" ,
		"\t4. ���� ����������� ������������ ������\n" ,
		"\t5. ������� ���������� ������������� ��������� ��������� ���������\n" ,
		"\t0. ��������� �����\n" ,
		"\n������� ����� ��������: "
	};

	while (!exit)
	{
		for (int i = 0; i < sizeof(firstMenu) / sizeof(std::string); i++)
		{
			std::cout << firstMenu[i];
		}
		std::cin >> sw;
		switch (sw)
		{
		case 1:
			std::cout << tree.getSize();
			break;
		case 2:
			tree.clear(tree.root);
			reverseIterator = nullptr;
			iterator = nullptr;
			break;
		case 3:
			std::cout << tree.isEmpty();
			break;
		case 4:
			std::cout << "������� ���� ";
			std::cin >> key;
			try
			{
				tree.resetViewed();
				std::cout << tree.read(key) << std::endl;
			}
			catch (std::exception ex)
			{
				std::cout << ex.what() << std::endl;
			}
			break;
		case 5:
			std::cout << "������� ���� ";
			std::cin >> key;
			std::cout << "������� ����� �������� ";
			std::cin >> value;
			tree.resetViewed();
			tree.edit(key, value, tree.root);
			break;
		case 6:
			std::cout << "������� ���� ";
			std::cin >> key;
			std::cout << "������� ����� �������� ";
			std::cin >> value;
			tree.resetViewed();
			tree.addB(key, value, tree.root);
 			break;
		case 7:
			std::cout << "������� ���� ";
			std::cin >> key;
			tree.resetViewed();
			tree.deleteNodeB(key, tree.root);
			break;
		case 8:
			tree.printTreeH(tree.root);
			break;
			// Iterator
		case 9:
			while (!exit)
			{
				for (int i = 0; i < sizeof(iteratorMenu) / sizeof(std::string); i++)
				{
					std::cout << iteratorMenu[i];
				}
				std::cin >> sw;
				switch (sw)
				{
				case 1:
					iterator = tree.begin();
					break;
				case 2:
					try
					{
						if (iterator.nodePtr)
							std::cout << *iterator;
						else
							throw std::exception("����������");
					}
					catch (std::exception ex)
					{
						std::cout << ex.what() << std::endl;
					}
					break;
				case 3:
					std::cout << "������� ����� �������� ";
					std::cin >> value;
					*iterator = value;
					break;
				case 4:
					try
					{
						iterator++;
					}
					catch (std::exception ex)
					{
						std::cout << ex.what() << std::endl;
					}
					break;
				case 5:
					try
					{
						iterator--;
					}
					catch (std::exception ex)
					{
						std::cout << ex.what() << std::endl;
					}
					break;
				case 6:
					std::cout << (iterator == tree.begin());
					break;
				case 7:
					std::cout << (iterator == tree.end());
					break;
				case 8:
					std::cout << (iterator != tree.begin());
					break;
				case 9:
					std::cout << (iterator != tree.end());
					break;
				case 0:
					exit = true;
					break;
				default:
					std::cout << "������� ������� �� ����������\n";
					break;
				}
			}
			exit = false;
			break;
			// ReverseIterator
		case 10:
			while (!exit)
			{
				for (int i = 0; i < sizeof(reverseIteratorMenu) / sizeof(std::string); i++)
				{
					std::cout << reverseIteratorMenu[i];
				}
				std::cin >> sw;
				switch (sw)
				{
				case 1:
					reverseIterator = tree.rBegin();
					break;
				case 2:
					try
					{
						if (reverseIterator.nodePtr)
							std::cout << *reverseIterator;
						else
							throw std::exception("����������");
					}
					catch (std::exception ex)
					{
						std::cout << ex.what() << std::endl;
					}
					break;
				case 3:
					std::cout << "������� ����� �������� ";
					std::cin >> value;
					*reverseIterator = value;
					break;
				case 4:
					try
					{
						reverseIterator++;
					}
					catch (std::exception ex)
					{
						std::cout << ex.what() << std::endl;
					}
					break;
				case 5:
					try
					{
						reverseIterator--;
					}
					catch (std::exception ex)
					{
						std::cout << ex.what() << std::endl;
					}
					break;
				case 6:
					std::cout << (reverseIterator == tree.rBegin());
					break;
				case 7:
					std::cout << (reverseIterator == tree.rEnd());
					break;
				case 8:
					std::cout << (reverseIterator != tree.rBegin());
					break;
				case 9:
					std::cout << (reverseIterator != tree.rEnd());
					break;
				case 0:
					exit = true;
					break;
				default:
					std::cout << "������� ������� �� ����������\n";
					break;
				}
			}
			exit = false;
			break;
			// Extra operations
		case 11:
			while (!exit)
			{
				for (int i = 0; i < sizeof(extraMenu) / sizeof(std::string); i++)
				{
					std::cout << extraMenu[i];
				}
				std::cin >> sw;
				switch (sw)
				{
				case 1:
					tree.t_Lt_Rt(tree.root);
					break;
				case 2:
					std::cout << tree.getNodeHeight(tree.root) << std::endl;
					break;
				case 3:
					std::cout << "������� ������ ";
					std::cin >> value;
					test_rand(value);
					break;
				case 4:
					std::cout << "������� ������ ";
					std::cin >> value;
					test_ord(value);
					break;
				case 5:
					std::cout << tree.getViewedNodes() << std::endl;
					break;
				case 0:
					exit = true;
					break;
				default:
					std::cout << "������� ������� �� ����������\n";
					break;
				}
			}
			exit = false;
			break;
		case 0:
			exit = true;
			break;
		default:
			std::cout << "������� ������� �� ����������\n";
			break;
		}
	}
	return 0;
}