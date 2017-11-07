#pragma once
#include<iostream>

using namespace std;

const int DefaultVertices = 30;
template<class T,class E>
struct Edge
{						//边结点的定义
	int dest;			//边的另一定点位置
	E cost;				//边上的权值
	Edge<T, E> *link;	//下条边链指针

	Edge() {};
	Edge(int num,E weight):dest(num),cost(weight),link(nullptr){}	//构造函数
	bool operator!=(Edge<T, E>&R)const 
	{
		return (dest != R.dest) ? true : false;						//判边不等否
	}
};

template <class T,class E>
struct Vertex
{						//定点的定义
	T data;				//定点的名字
	Edge<T, E> *adj;	//边链表的头指针
};

template<class T,class E>
class Graph
{
private:
	int maxVertices;						//图中最大顶点数
	int numEdges;							//当前边数
	int numVertices;						//当前顶点数
	Vertex<T,E>*NodeTable;
public:
	Graph(int sz = DefaultVertices);			//构造函数
	~Graph();									//析构函数
	int getVertexPos(const T vertex);
	T getValue(int i);
/*----------------------------------------------------------------------------------------------------------*/
	//判断图空
	bool GraphEmpty()const
	{
		if (numEdges == 0)return true;
		else return false;
	}
	//判断图满
	bool GraphFull()const 
	{
		if (numVertices == maxVertices || numEdges == maxVertices*(maxVertices - 1) / 2)return true;
		else return false;
	}
	
	//返回当前边数
	int NumberOfVertices() { 
		return numVertices; 
	}
	//返回当前顶点数
	int NumberOfEdges() {
		return numEdges; 
	}
/*----------------------------------------------------------------------------------------------------------*/

	E getWeigth(int v1, int v2);			//取边(v1,v2)上的权值
	int getFirstNeighbor(int v);			//取顶点v的第一个邻接顶点
	int getNextNeighbor(int v, int w);		//取顶点v的邻接定点w的下一个邻接顶点
	bool insertVertex(const T& vertex);		//插入一个顶点vertex
	bool insertEdge(int v1,int v2, E cost);	//插入边(v1,v2),权值为cost
	bool removeVertex(int v);				//删除顶点v和所有相关的边
	bool removeEdge(int v1, int v2);		//在图中删除边(v1,v2)
	void show();
};
/*----------------------------------------------------------------------------------------------------------*/

template<class T,class E>//构造函数
Graph<T, E>::Graph(int sz) 
{
	//构造函数：建立一个空的邻接表
	maxVertices = sz; numVertices = 0; numEdges = 0;
	NodeTable = new Vertex<T, E>[maxVertices];
	if (NodeTable == nullptr) { 
		cerr << "存储分配错误！" << endl; 
		exit(1); 
	}
	for (int i = 0; i < maxVertices; i++)
		NodeTable[i].adj = nullptr;
}

template<class T,class E>//析构函数
Graph<T,E>::~Graph()
{
	//析构函数：删除一个邻接表
	for (int i = 0; i < numVertices; i++)		//删除各边链表中的节点
	{		
		Edge<T, E> *p = NodeTable[i].adj;		//找到其对应边链表的首节点
		while (p!=nullptr)						//不断的删除第一个节点	
		{
			NodeTable[i].adj = p->link;
			delete p; p = NodeTable[i].adj;
		}
	}
	delete[]NodeTable;							//删除顶点表数组
}

template<class T, class E>
int Graph<T, E>::getVertexPos(const T vertex)
{
	for (int i = 0; i < numVertices; i++)
		if (NodeTable[i].data == vertex)return i;
	return -1;
}

template<class T, class E>
T Graph<T, E>::getValue(int i)
{
	return(i >= 0 && i < numVertices) ? NodeTable[i].data : 0;
}

/*----------------------------------------------------------------------------------------------------------*/

template<class T, class E>//取边(v1,v2)上的权值
E Graph<T, E>::getWeigth(int v1,int v2)
{
	//函数返回边(v1,v2)上的权值，若边不再图中，则函数返回权值0
	if (v1 != -1 && v2 != -1)
	{
		Edge<T, E> *p = NodeTable[v1].adj;		//v1的第一条关联边
		while (p != nullptr&&p->dest != v2)
			p = p->link;
		if (p != nullptr)
			return p->cost;		//找到此边，返回权值
	}
	return 0;									//边不存在
}

template<class T, class E>//取顶点v的第一个邻接顶点
int Graph<T, E>::getFirstNeighbor(int v) 
{
	if (v != -1) 								//顶点v存在
	{
		Edge<T, E> *p = NodeTable[v].adj;		//对应链表第一个边结点
		if (p != nullptr)
			return p->dest;						//存在，返回第一个邻接节点
	}
	return -1;									//第一个邻接节点不存在
};

template<class T, class E>//取顶点v的邻接定点w的下一个邻接顶点
int Graph<T, E>::getNextNeighbor(int v, int w) 
{
	if (v != -1) 
	{											//顶点v存在
		Edge<T, E> *p = NodeTable[v].adj;		//对应链表第一个边结点
		while (p != nullptr&&p->dest != w)		//寻找邻接顶点w
			p = p->link;
		if (p != nullptr&&p->link!=nullptr)
			return p->link->dest;				//存在，返回第一个邻接节点
	}
	return -1;									//下一个邻接节点不存在
}

template<class T, class E>//插入一个顶点vertex
bool Graph<T, E>::insertVertex(const T& vertex)
{
	if (numVertices == maxVertices)
		return false;								//顶点表满了，不能插入
	NodeTable[numVertices].data = vertex;			//插在表最后
	numVertices++; 
	return true;
}

template<class T, class E>//插入边(v1,v2),权值为cost
bool Graph<T, E>::insertEdge(int v1, int v2, E weight)
{
	if (v1 >= 0 && v1 < numVertices&&v2 >= 0 && v2 < numVertices)
	{
		Edge<T, E> *q, *p = NodeTable[v1].adj;		//v1对应的边链表头指针
		while (p != nullptr&&p->dest != v2)			//寻找邻接顶点v2
			p = p->link;
		if (p != nullptr)
			return false;							//此边已存在，不再插入

		p = new Edge<T, E>;
		q = new Edge<T, E>;							//创建新节点
		p->dest = v2; p->cost = weight;
		p->link = NodeTable[v1].adj;				//链入v1边链表
		NodeTable[v1].adj = p;
		q->dest = v1; q->cost = weight;

		q->link = NodeTable[v2].adj;				//链入v2边链表
		NodeTable[v2].adj = q;
		numEdges++; 
		return true;
	}
	return false;
}

template<class T, class E>//删除顶点v和所有相关的边
bool Graph<T, E>::removeVertex(int v)
{
	if (numVertices == 1 || v < 0 || v >= numVertices)
		return false;								//表空或者顶点号超出范围

	Edge<T, E> *p, *s, *t; int k;
	while (NodeTable[v].adj != nullptr)
	{												//删除第v个边链表中的所有结点
		p = NodeTable[v].adj;
		k = p->dest;								//取邻结点k
		s = NodeTable[k].adj;						//找对称存放的边结点
		t = nullptr;
		while (s != nullptr&&s->dest != v)			//查找对应节点边链表中对应的边
		{
			t = s; 
			s = s->link;
		}
		if (s != nullptr) {							//删除对称存放的边结点
			if (t == nullptr)						//首节点
				NodeTable[k].adj = s->link;			//不是首节点
			else t->link = s->link;
			delete s;
		}
		NodeTable[v].adj = p->link;					//清除边结点v的边链表节点
		delete p;
		numEdges--;									//与顶点v相关的边数减一
	}
	numVertices--;									//图的顶点数减一
	NodeTable[v].data = NodeTable[numVertices].data;//填补
	p = NodeTable[v].adj = NodeTable[numVertices].adj;//转移指针
	while (p != nullptr)							//所填补的节点边链表非空，需要将边链表中的dest值更改为填充后的值
	{
		s = NodeTable[p->dest].adj;
		while (s!=nullptr)
		{
			if(s->dest==numVertices)				//找到对应节点
			{
				s->dest = v;
				break;
			}
			else s = s->link;
		}
		p = p->link;								//更改边链表中下一个边所对应的dest值
	}
	return true;
}

template<class T, class E>//在图中删除边(v1,v2)
bool Graph<T, E>::removeEdge(int v1, int v2)
{
	if (v1 != -1 && v2 != -1) {
		Edge<T, E> *p = NodeTable[v1].adj, *q = nullptr, *s = p;
		while (p!=nullptr&&p->dest!=v2)					//v1对应边链表中查找对应边
		{
			q = p;
			p = p->link;
		}
		if (p != nullptr)								//找到对应边
		{
			if (p==s)
				NodeTable[v1].adj = p->link;			//该节点是边链表的首节点
			else
				q->link = p->link;						//不是首节点，重新连接
			delete p;
		}
		else return false;								//没有找到被删除边

		p = NodeTable[v2].adj; q = nullptr, s = p;		//v2中边链表删除
		while (p->dest!=v1)
		{
			q = p;
			p = p->link;								//寻找被删除节点
		}
		if (p == s)
			NodeTable[v2].adj = p->link;				//该节点是边链表首节点
		else
			q->link = p->link;							//不是，重新连接
		delete p;
		numEdges--;
		return true;
	}
	return false;										//没有找到节点
}

template<class T, class E>
void Graph<T, E> ::show() 
{
	int i, j, n, m; T e1, e2; E w;
	n = NumberOfVertices();
	m = NumberOfEdges();
	cout << n << "," << m << endl;						//输出顶点数和边数
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++)
		{
			w = getWeigth(i, j);
			if (w > 0)
			{
				e1 = getValue(i); e2 = getValue(j);
				cout << "(" << e1 << "," << e2 << "," << w << ")" << endl;
			}
		}
}