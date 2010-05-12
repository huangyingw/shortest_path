#include "stdafx.h"
#include<iostream>
using namespace std;
#define MAX 1000
typedef int Queue,DataType;  //DataType的类型依赖于具体的应用
typedef int VertexType;

#define MaxVertexNum 100//最大结点数目

int dist[MaxVertexNum];//dist[i]表示当前找到的从vo到vi的最短路径的长度

bool S[MaxVertexNum];//S集合存放已经求出最短路径的终点

typedef struct node
{	
	int adjvex; 
	int value;//若要表示边上的权，则应增加一个数据域
	struct node *next;
}EdgeNode;

typedef struct vnode
{
	VertexType vertex; //顶点域
	EdgeNode *firstedge;//边表头指针
}VertexNode;

typedef VertexNode AdjList[MaxVertexNum];//AdjList是邻接表类型

typedef struct ALGraph
{
	//邻接表
	AdjList adjlist;
	//图中当前顶点数
	int n; 
	//图中当前边数
	int e;
}Graphic; //对于简单的应用，无须定义此类型，可直接使用AdjList类型。 

void InitialS(ALGraph *G);
void InitialDist(ALGraph *G);
void PrintDist();

void CreateGraphic(ALGraph *G,int dim,int* data)
{
	EdgeNode *edgenode;
	for(int i=0;i<dim;i++)
	{
		G->adjlist[i].firstedge=NULL;
		G->adjlist[i].vertex=i;
		for(int j=0;j<dim;j++)
		{
			if(data[dim*i+j]<MAX)
			{
				edgenode=new EdgeNode;
				edgenode->adjvex=j;
				edgenode->value=data[dim*i+j];
				edgenode->next=G->adjlist[i].firstedge;
				G->adjlist[i].firstedge=edgenode;
			}
		}
	}
}

void CreateGraphic(ALGraph *G)
{
	cout<<"请输入图的结点数"<<endl;
	cin>>G->n;
	cout<<"请输入图的边数"<<endl;
	cin>>G->e;
	EdgeNode *edgenode;
	for(int k=0;k<G->n;k++)
	{
		G->adjlist[k].firstedge=NULL;
		G->adjlist[k].vertex=k;
	}
	for(int k=0;k<G->e;k++)
	{
		int i,j,value;
		cout<<"请输入边(vi,vj)的结点号"<<endl;
		cin>>i>>j;
		cout<<"请输入边(vi,vj)的权值"<<endl;
		cin>>value;
		edgenode=new EdgeNode;
		edgenode->adjvex=j;
		edgenode->value=value;
		edgenode->next=G->adjlist[i].firstedge;
		G->adjlist[i].firstedge=edgenode;
		edgenode=new EdgeNode;
		edgenode->adjvex=i;
		edgenode->value=value;
		edgenode->next=G->adjlist[j].firstedge;
		G->adjlist[j].firstedge=edgenode;
	}
}

void PrintGraphic(ALGraph *G)
{
	EdgeNode *p;
	cout<<"现在开始打印图："<<endl;
	for(int i=0;i<G->n;i++)
	{
		p=G->adjlist[i].firstedge;
        while(p)
		{
			cout<<p->adjvex<<","<<p->value<<"  ";
			p=p->next; //找vi的下一邻接点
		}
		cout<<endl;
	}
}
//从当前结点出发寻找与它相邻的结点中具有最小边的那个结点
//返回其结点编号
int LeastAdjacence(ALGraph *G,int vertex)
{
	//用来存储当前结点的最小相邻边的结点编号
	//如果没有相邻边的话，就默认为-1
	int LeastAdjIndex=-1;
	EdgeNode *node=G->adjlist[vertex].firstedge;
	while(node!=NULL)
	{
		//用来存储当前结点的最小相邻边的权值
		int LeastAdjValue=MAX;
		if(node->value<LeastAdjValue)
		{
			LeastAdjValue=node->value;
			LeastAdjIndex=node->adjvex;
		}
		node=node->next;
	}
	return LeastAdjIndex;
}

/*添加所有以EdgeNodeIndex为起点的边
此处，EdgeNodeIndex点已经加入到了S集合中，等于是更新相关的
最短距离
*/
void ExtendEdgeNode(ALGraph *G,int EdgeNodeIndex)
{
	EdgeNode *node=G->adjlist[EdgeNodeIndex].firstedge;
	while(node!=NULL)
	{
        //如果node->adjvex结点未曾加入过S集合,则更新它的dist
		if(!S[node->adjvex]&&dist[EdgeNodeIndex]+node->value<dist[node->adjvex])
			dist[node->adjvex]=dist[EdgeNodeIndex]+node->value;
		node=node->next;
	}
}

/*从当前dist数组中找出最小的,并将当前
最小的边对应的终结点加入S集合 */
int LeastDistAddS()
{
	//存储最小dist value值
	int LeastDistValue=MAX;
	//存储最小dist 结点下标值
	int LeastDistIndex=-1;
	for(int i=0;i<5;i++)
	{
		if(dist[i]<LeastDistValue&&!S[i])
		{
			LeastDistValue=dist[i];
			LeastDistIndex=i;
		}
	}
	S[LeastDistIndex]=true;
	return LeastDistIndex;
}

//是否全部结点都加入了S集合
bool IsSFull()
{
	for(int i=0;i<5;i++)
	{
		if(!S[i])
			return false;
	}
	return true;
}
	
//最短路径的Dijkstra实现
void Dijkstra(ALGraph *G)
{
	//结点编号
	int EdgeNodeIndex;
	//先初始化S集合
	InitialS(G);
	/*初始化dist数组*/
	InitialDist(G);
	//假设从0号结点出发
	S[0]=true;
	EdgeNodeIndex=LeastAdjacence(G,0);
	S[EdgeNodeIndex]=true;
	do
	{
		//添加所有以EdgeNodeIndex为起点的边
		ExtendEdgeNode(G,EdgeNodeIndex);
		//从当前dist数组中找出最小的
		EdgeNodeIndex=LeastDistAddS();
		
	}while(!IsSFull());
	//ExtendEdgeNode(G,EdgeNodeIndex);
	PrintDist();
}

//打印dist[]数组中未加过S的相应值,主要做调试用途
void PrintDist()
{
	for(int i=0;i<5;i++)
	{
		cout<<i<<":"<<S[i]<<","<<dist[i]<<endl;
	}
}

/*初始化dist数组,若从vo到vi有边，则dist[i]为该边的权值，
若无边，则为MAX*/
void InitialDist(ALGraph *G)
{
	//边权值
	int EdgeWeight=MAX;
	for(int i=0;i<G->n;i++)
	{
		dist[i]=MAX;
	}
	EdgeNode *node=G->adjlist[0].firstedge;
	while(node!=NULL)
	{
		//用来存储当前结点的最小相邻边的权值
		if(node->value<MAX)
		{
			dist[node->adjvex]=node->value;
		}
		node=node->next;
	}
}

/*判断从v0到vi是否有边，若从vo到vi有边，则返回该边的权值，
若无边，则返回MAX*/
int VIValue(int i)
{
	return i;
}

//先初始化S集合，令所有布尔值为false
void InitialS(ALGraph *G)
{
	for(int i=0;i<G->n;i++)
	{
		S[i]=false;
	}
}

//寻找所有还未加入S集合的边中的最小权值
int _tmain(int argc, _TCHAR* argv[])
{
	ALGraph G;
	G.n=5;
	int resdata[5][5]={{MAX,10,MAX,30,100},{MAX,MAX,50,MAX,MAX},{MAX,MAX,MAX,MAX,10},{MAX,MAX,20,MAX,60},{MAX,MAX,MAX,MAX,MAX}};
	CreateGraphic(&G,5,*resdata);
	//PrintGraphic(&G);
	Dijkstra(&G);
	return 0;
}

