#include<cstdio>
#include<cstring>
#include<algorithm>
#include<vector>
using namespace std;
const int N = 105;
const int M = 1050;
const int inf = 1987654321;
int a,b;
int t;	//t is the total time of the game
int k;	//k is the number of battlefields
int d; //each turn change at most d troops
int schemeA[M][M],schemeB[M][M];	//the strategies of players
bool transA[M][M];	//transposition of player A
bool transB[M][M];	//transposition of player B
int payoff[M][M];	//payoff of player A
int n,m;	//n is the number of status of player A, and m is that of player B
struct Stra
{
	vector<int> index;
	int val;
};
void makeScheme(int x,int last,int& cc,int array[][M])
{
	if(x>=k)
	{
		array[0][x]=last;
		cc++;
		int i;
		for(i=1;i<=k;i++)
			array[cc][i]=array[0][i];
		return;
	}
	int i;
	for(i=0;i<=last;i++)
	{
		array[0][x]=i;
		makeScheme(x+1,last-i,cc,array);
	}
}
void makeGraph()	//transA,transB,payoff,  schemes
{
	int i,j,l;
	n=m=0;
	makeScheme(1,a,n,schemeA);
	makeScheme(1,b,m,schemeB);
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=m;j++)
		{
			
			payoff[i][j]=0;
			for(l=1;l<=k;l++)
			{
				if(schemeA[i][l]>schemeB[j][l])
					payoff[i][j]++;
				if(schemeA[i][l]<schemeB[j][l])
					payoff[i][j]--;
			}
		}
	}
	for(i=0;i<=n;i++)
	{
		for(j=1;j<=n;j++)
		{
			if(i==0)
				transA[i][j]=1;
			else
			{
				int now=0;
				for(l=1;l<=k;l++)
				{
					if(schemeA[i][l]>schemeA[j][l])
						now+=schemeA[i][l]-schemeA[j][l];
				}
				if(now<=d)
					transA[i][j]=1;
			}
		}
	}
	for(i=0;i<=m;i++)
	{
		for(j=1;j<=m;j++)
		{
			if(i==0)
				transB[i][j]=1;
			else
			{
				int now=0;
				for(l=1;l<=k;l++)
				{
					if(schemeB[i][l]>schemeB[j][l])
						now+=schemeB[i][l]-schemeB[j][l];
				}
				if(now<=d)
					transB[i][j]=1;
			}
		}
	}
}
void getData()	//a,b,k,d,t,transA,transB,payoff
{
	a=b=6;
	k=5;
	d=3;
	makeGraph();
	t=2;
}
Stra dfsB(int x,int lasta,int lastb);
Stra dfsA(int x,int lasta,int lastb)
{
	if(x>t)
	{
		Stra res;
		res.index.clear();
		res.val=payoff[lasta][lastb];
		return res;
	}
	Stra res;
	res.index.clear();
	res.val=-inf;
	int i,j;
	for(i=1;i<=n;i++)
	{
		if(transA[lasta][i])
		{
			Stra now=dfsB(x,i,lastb);
			if(now.val>res.val)
			{
				res=now;
				res.index.push_back(i);
			}
		}
	}
	return res;
}
Stra dfsB(int x,int lasta,int lastb)
{
	Stra res;
	res.index.clear();
	res.val=inf;
	int i,j;
	for(i=1;i<=m;i++)
	{
		if(transB[lastb][i])
		{
			Stra now=dfsA(x+1,lasta,i);
			if(now.val<res.val)
			{
				res=now;
				res.index.push_back(i);
			}
		}
	}
	return res;
}
void calc()
{
	Stra result=dfsA(1,0,0);
	printf("%d\n",result.val);
	int i,j;
	for(j=result.index.size()-1;j>=0;j--)
	{
		for(i=1;i<=k;i++)
			printf("%d ",schemeA[result.index[j]][i]);
		printf("\n");
	}
}
int main()
{
	getData();
	calc();
	return 0;
}
