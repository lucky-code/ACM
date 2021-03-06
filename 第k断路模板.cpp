/*
 *算法引入：
 *在单源点最短路径问题中,实际运用时还需知道最短路径外,次短路或者第三短路;
 *即要知道多条最短路,并排出其长度增加的顺序,即为K最短路问题;
 *
 *算法思想：
 *单源点最短路径+高级搜索A*;
 *A*算法结合了启发式方法和形式化方法;
 *启发式方法通过充分利用图给出的信息来动态地做出决定而使搜索次数大大降低;
 *形式化方法不利用图给出的信息,而仅通过数学的形式分析;
 *
 *算法通过一个估价函数f(h)来估计图中的当前点p到终点的距离,并由此决定它的搜索方向;
 *当这条路径失败时,它会尝试其他路径;
 *对于A*,估价函数=当前值+当前位置到终点的距离,即f(p)=g(p)+h(p),每次扩展估价函数值最小的一个;
 *
 *对于K短路算法来说,g(p)为当前从s到p所走的路径的长度;h(p)为点p到t的最短路的长度;
 *f(p)的意义为从s按照当前路径走到p后再走到终点t一共至少要走多远;
 *
 *为了加速计算,h(p)需要在A*搜索之前进行预处理,只要将原图的所有边反向,再从终点t做一次单源点最短路径就能得到每个点的h(p)了;
 *
 *算法步骤：
 *(1),将有向图的所有边反向,以原终点t为源点,求解t到所有点的最短距离;
 *(2),新建一个优先队列,将源点s加入到队列中;
 *(3),从优先级队列中弹出f(p)最小的点p,如果点p就是t,则计算t出队的次数;
 *如果当前为t的第k次出队,则当前路径的长度就是s到t的第k短路的长度,算法结束;
 *否则遍历与p相连的所有的边,将扩展出的到p的邻接点信息加入到优先级队列;
 *
 *算法测试：
 *PKU2449(Remmarguts' Date)
 *
 *题目大意：
 *求从s到t的第k短路的长度;
 */

#include<iostream>
#include<cstring>
#include<cstdlib>
#include<queue>
#include<cstdio>
#include<climits>
#include<algorithm>
using namespace std;

const int INF=0xffffff;
const int N=1010;
const int M=100010;

struct node1
{
    int to;
    int w;
    int next;
};

node1 edge1[M],edge2[M];
int head1[M],head2[M];
int idx1,idx2;
int dist[N];

struct node2
{
    int to;
    //g(p)为当前从s到p所走的路径的长度;h(p)为点p到t的最短路的长度;
    int g,f;//f=g+h,f(p)的意义为从s按照当前路径走到p后再走到终点t一共至少要走多远;
    bool operator<(const node2 &r ) const
    {
        if(r.f==f)
            return r.g<g;
        return r.f<f;
    }
};

void Addedge1(int u,int v,int w)
{
    edge1[idx1].w=w;
    edge1[idx1].to=v;
    edge1[idx1].next=head1[u];
    head1[u]=idx1++;
}

void Addedge2(int u,int v,int w)
{
    edge2[idx2].w=w;
    edge2[idx2].to=v;
    edge2[idx2].next=head2[u];
    head2[u]=idx2++;
}

bool SPFA(int s,int n,int head[],node1 edge[],int dist[])
{
    queue<int>Q1;
    int inq[N];
    for(int i=0; i<=n; i++)
    {
        dist[i]=INF;
        inq[i]=0;
    }
    dist[s]=0;
    Q1.push(s);
    inq[s]++;
    while(!Q1.empty())
    {
        int q=Q1.front();
        Q1.pop();
        inq[q]--;
        if(inq[q]>n)//负权环
            return false;
        int k=head[q];
        while(k>=0)
        {
            if(dist[edge[k].to]>dist[q]+edge[k].w)
            {
                dist[edge[k].to]=edge[k].w+dist[q];
                if(!inq[edge[k].to])
                {
                    inq[edge[k].to]++;
                    Q1.push(edge[k].to);
                }
            }
            k=edge[k].next;
        }
    }
    return true;
}

int A_star(int s,int t,int n,int k,int head[],node1 edge[],int dist[])
{
    node2 e,ne;
    int cnt=0;
    priority_queue<node2>Q;
    if(s==t)//当s==t时,距离为0的路不能算在这k短路中,所以需要求k+1短路;
        k++;
    if(dist[s]==INF)
        return -1;
    e.to=s;
    e.g=0;
    e.f=e.g+dist[e.to];
    Q.push(e);

    while(!Q.empty())
    {
        e=Q.top();
        Q.pop();
        if(e.to==t)//找到一条最短路径
        {
            cnt++;
        }
        if(cnt==k)//找到k短路
        {
            return e.g;
        }
        for(int i=head[e.to]; i!=-1; i=edge[i].next)
        {
            ne.to=edge[i].to;
            ne.g=e.g+edge[i].w;
            ne.f=ne.g+dist[ne.to];
            Q.push(ne);
        }
    }
    return -1;
}

int main()
{
    int n,m;
    //freopen("C:\\Users\\Administrator\\Desktop\\kd.txt","r",stdin);
    while(~scanf("%d%d",&n,&m))
    {
        memset(head1, -1, sizeof(head1));
        memset(head2, -1, sizeof(head2));
        idx1=idx2=0;
        int u,v,w;
        for(int i=0; i<m; i++)
        {
            scanf("%d%d%d",&u,&v,&w);
            Addedge1(u,v,w);
            Addedge2(v,u,w);
        }
        int s,t,k;
        scanf("%d%d%d",&s,&t,&k);
        SPFA(t,n,head2,edge2,dist);//以原终点t为源点,求反向图中t到所有点的最短距离;
        int res=A_star(s,t,n,k,head1,edge1,dist);
        printf("%d\n",res);
    }
    return 0;
}

