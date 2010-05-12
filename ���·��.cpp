#include "stdafx.h"
#include<iostream>
using namespace std;
#define MAX 1000
typedef int Queue,DataType;  //DataType�����������ھ����Ӧ��
typedef int VertexType;

#define MaxVertexNum 100//�������Ŀ

int dist[MaxVertexNum];//dist[i]��ʾ��ǰ�ҵ��Ĵ�vo��vi�����·���ĳ���

bool S[MaxVertexNum];//S���ϴ���Ѿ�������·�����յ�

typedef struct node
{	
	int adjvex; 
	int value;//��Ҫ��ʾ���ϵ�Ȩ����Ӧ����һ��������
	struct node *next;
}EdgeNode;

typedef struct vnode
{
	VertexType vertex; //������
	EdgeNode *firstedge;//�߱�ͷָ��
}VertexNode;

typedef VertexNode AdjList[MaxVertexNum];//AdjList���ڽӱ�����

typedef struct ALGraph
{
	//�ڽӱ�
	AdjList adjlist;
	//ͼ�е�ǰ������
	int n; 
	//ͼ�е�ǰ����
	int e;
}Graphic; //���ڼ򵥵�Ӧ�ã����붨������ͣ���ֱ��ʹ��AdjList���͡� 

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
	cout<<"������ͼ�Ľ����"<<endl;
	cin>>G->n;
	cout<<"������ͼ�ı���"<<endl;
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
		cout<<"�������(vi,vj)�Ľ���"<<endl;
		cin>>i>>j;
		cout<<"�������(vi,vj)��Ȩֵ"<<endl;
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
	cout<<"���ڿ�ʼ��ӡͼ��"<<endl;
	for(int i=0;i<G->n;i++)
	{
		p=G->adjlist[i].firstedge;
        while(p)
		{
			cout<<p->adjvex<<","<<p->value<<"  ";
			p=p->next; //��vi����һ�ڽӵ�
		}
		cout<<endl;
	}
}
//�ӵ�ǰ������Ѱ���������ڵĽ���о�����С�ߵ��Ǹ����
//����������
int LeastAdjacence(ALGraph *G,int vertex)
{
	//�����洢��ǰ������С���ڱߵĽ����
	//���û�����ڱߵĻ�����Ĭ��Ϊ-1
	int LeastAdjIndex=-1;
	EdgeNode *node=G->adjlist[vertex].firstedge;
	while(node!=NULL)
	{
		//�����洢��ǰ������С���ڱߵ�Ȩֵ
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

/*���������EdgeNodeIndexΪ���ı�
�˴���EdgeNodeIndex���Ѿ����뵽��S�����У������Ǹ�����ص�
��̾���
*/
void ExtendEdgeNode(ALGraph *G,int EdgeNodeIndex)
{
	EdgeNode *node=G->adjlist[EdgeNodeIndex].firstedge;
	while(node!=NULL)
	{
        //���node->adjvex���δ�������S����,���������dist
		if(!S[node->adjvex]&&dist[EdgeNodeIndex]+node->value<dist[node->adjvex])
			dist[node->adjvex]=dist[EdgeNodeIndex]+node->value;
		node=node->next;
	}
}

/*�ӵ�ǰdist�������ҳ���С��,������ǰ
��С�ı߶�Ӧ���ս�����S���� */
int LeastDistAddS()
{
	//�洢��Сdist valueֵ
	int LeastDistValue=MAX;
	//�洢��Сdist ����±�ֵ
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

//�Ƿ�ȫ����㶼������S����
bool IsSFull()
{
	for(int i=0;i<5;i++)
	{
		if(!S[i])
			return false;
	}
	return true;
}
	
//���·����Dijkstraʵ��
void Dijkstra(ALGraph *G)
{
	//�����
	int EdgeNodeIndex;
	//�ȳ�ʼ��S����
	InitialS(G);
	/*��ʼ��dist����*/
	InitialDist(G);
	//�����0�Ž�����
	S[0]=true;
	EdgeNodeIndex=LeastAdjacence(G,0);
	S[EdgeNodeIndex]=true;
	do
	{
		//���������EdgeNodeIndexΪ���ı�
		ExtendEdgeNode(G,EdgeNodeIndex);
		//�ӵ�ǰdist�������ҳ���С��
		EdgeNodeIndex=LeastDistAddS();
		
	}while(!IsSFull());
	//ExtendEdgeNode(G,EdgeNodeIndex);
	PrintDist();
}

//��ӡdist[]������δ�ӹ�S����Ӧֵ,��Ҫ��������;
void PrintDist()
{
	for(int i=0;i<5;i++)
	{
		cout<<i<<":"<<S[i]<<","<<dist[i]<<endl;
	}
}

/*��ʼ��dist����,����vo��vi�бߣ���dist[i]Ϊ�ñߵ�Ȩֵ��
���ޱߣ���ΪMAX*/
void InitialDist(ALGraph *G)
{
	//��Ȩֵ
	int EdgeWeight=MAX;
	for(int i=0;i<G->n;i++)
	{
		dist[i]=MAX;
	}
	EdgeNode *node=G->adjlist[0].firstedge;
	while(node!=NULL)
	{
		//�����洢��ǰ������С���ڱߵ�Ȩֵ
		if(node->value<MAX)
		{
			dist[node->adjvex]=node->value;
		}
		node=node->next;
	}
}

/*�жϴ�v0��vi�Ƿ��бߣ�����vo��vi�бߣ��򷵻ظñߵ�Ȩֵ��
���ޱߣ��򷵻�MAX*/
int VIValue(int i)
{
	return i;
}

//�ȳ�ʼ��S���ϣ������в���ֵΪfalse
void InitialS(ALGraph *G)
{
	for(int i=0;i<G->n;i++)
	{
		S[i]=false;
	}
}

//Ѱ�����л�δ����S���ϵı��е���СȨֵ
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

