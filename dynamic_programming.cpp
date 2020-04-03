#include<cstdio>
#include<cstring>
#include<algorithm>
#include<vector>
using namespace std;
const int num_fields = 105;
const int num_strs = 505;
const int num_times = 105;
const int inf = 1987654321;
int a,b;
int t;	//t is the total time of the game
int k;	//k is the number of battlefields
int d; //each turn change at most d troops
int schemeA[num_strs][num_fields],schemeB[num_strs][num_fields];	//the strategies of players
//schemeA[i][j] denotes the number of soldiers in the jth battlefield in the ith strategy of A
bool transA[num_strs][num_strs];	//transposition of player A
bool transB[num_strs][num_strs];	//transposition of player B
int payoff[num_strs][num_strs];	//payoff of player A
int n,m;	//n is the number of status of player A, and m is that of player B
int bestScoreA[num_times][num_strs][num_strs],bestScoreB[num_times][num_strs][num_strs];
//[i][j][k] denote the best score after i operations starting at scheme j and enemy scheme k
struct Stra
{
	vector<int> index;
	int val;
};
void makeScheme(int x,int last,int& cc,int array[][num_fields])
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
void getData()	//a,b,k,d,t,transA,transB,payoff,scheme
{
	int i,j;
	a=b=6;
	k=5;
	d=3;
	makeGraph();
	t=2;
	
	//add a virtual initial strategy
	n++;
	for(i=1;i<n;i++)
		transA[n][i]=1;
	m++;
	for(i=1;i<m;i++)
		transB[m][i]=1;
}
void calc()
{
	int i,j,l,p,q;
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=m;j++)
		{
			bestScoreA[0][i][j]=payoff[i][j];
		}
	}
	for(l=1;l<=t;l++)
	{
		for(i=1;i<=n;i++)
		{
			for(j=1;j<=m;j++)
			{
				int now=inf;
				for(p=1;p<=m;p++)
				{
					if(transB[j][p])
					{
						now=min(now,bestScoreA[l-1][i][p]);
					}
				}
				bestScoreB[l][j][i]=now;
			}
		}
		for(i=1;i<=n;i++)
		{
			for(j=1;j<=m;j++)
			{
				int now=-inf;
				for(p=1;p<=n;p++)
				{
					if(transA[i][p])
					{
						now=max(now,bestScoreB[l][j][p]);
					}
				}
				bestScoreA[l][i][j]=now;
			}
		}
	}
	printf("%d\n",bestScoreA[t][n][m]);
}
int main()
{
	getData();	//a,b,k,d,t,transA,transB,payoff,scheme
	calc();
	return 0;
}
