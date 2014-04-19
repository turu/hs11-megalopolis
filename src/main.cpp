/*
                     HS11MEGA - Megalopolis. Autor: Piotr Turek

    Glowny zamysl problemu polega na tym, ze dostajemy pewien wazony, skierowany graf
i naszym zadaniem jest znalezc taki podzbior krawedzi, ze suma ich wag jest mozliwie
najwieksza, a po ich usunieciu i 'ulepszeniu' pewnych innych krawedzi (przez ulepszenie
nalezy rozumiec uczynienie ich dwukierunkowymi) bedzie mozna dojsc z kazdego wierzcholka
do kazdego innego. Dodatkowym obostrzeniem jest to, że nie mozemy usuwac (ani ulepszac)
krawedzi, ktore znajduja sie wewnatrz jakiejs silnie spojnej skladowej (megalopolis).
Przez 'znajdowanie sie krawedzi wewnatrz' SCC nalezy tu rozumiec sytuacje w ktorej oba
wierzcholki danej krawedzi naleza do tej samej SCC.

    Rozwiazanie polega na zidentyfikowaniu w grafie wszystkich silnie spojnych skladowych
co mozemy wykonac w czasie O(|V|+|E|), a nastepnie znalezieniu minimalnego drzewa
spinajacego dla nieskierowanego, wazonego grafu silnie spojnych skladowych (zwanego dalej
Gscc), co możemy wykonac w czasie O(|V| log |E|). Jezeli przez Sc oznaczymy sume wag na
krawedziach grafu Gscc, a przez Ss sume wag na krawedziach minimalnego drzewa spinajacego
Gscc, to wynikiem - maksymalna suma wag na krawedziach, ktore mozemy usunac - jest roznica
Sc - Ss. Dzieje sie tak dlatego, iz Gscc zawiera wszystkie krawedzie, ktore potencjalnie
mozemy usunac tj. nie leza one wewnatrz zadnej SCC. Z definicji minimalnego drzewa spinajacego
wynika zas, ze suma wag na jego krawedziach jest najmniejsza mozliwa. Szukanym zbiorem krawedzi
do usuniecia beda wiec wszystkie krawedzie Gscc nienalezace do jego minimalnego drzewa
spinajacego, a suma ich wag będzie równa Sc - Ss.

*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <utility>
#include <cstdio>
#include <cassert>

#define Min(a, b) (a) < (b) ? (a) : (b)

using namespace std;

//structure representing an edge of undirected, weighted graph
struct E
{
    int v;
    int w;
    int c;
    E() : v(0), w(0), c(0)
    {}
    E(int a, int b, int d) : v(a), w(b), c(d)
    {}
};

bool comp(const E & lhs, const E & rhs)
{
    return lhs.c < rhs.c;
}

int myTime = 0; //time variable
int total_cost = 0; //total annual maintenance cost of inter-metropolis motorways
vector<vector<pair<int, int> > > graph;
vector<int> low, index, myroot; //lowlink and visitation time of nodes; myroot[v] keeps id of v's SCC root
vector<bool> visited, onstack; //true if a node has already been visited; true if a node is on the SCC stack
deque<int> myStack; //SCC stack
vector<E> edges; //set of selected edges

vector<int> rank, father; //disjoint set structure

//Tarjan SCC algorithm -->
void dfs_scc(int s)
{
	index[s] = low[s] = myTime++;
	myStack.push_back(s);
	onstack[s] = true;
	visited[s] = true;
	for(vector<pair<int, int> >::iterator v = graph[s].begin(); v != graph[s].end(); v++)
	{
		if(!visited[v->first])
		{
			dfs_scc(v->first);
			low[s] = Min(low[s], low[v->first]);
		}
		else if(onstack[v->first])
		{
			low[s] = Min(low[s], index[v->first]);
		}
	}
	int v;
	if(low[s] == index[s])
	{
		do {
			v = myStack.back();
			myroot[v] = s; //sets 'root' of v to s, cause s is the root of v's SCC
			onstack[v] = false;
			myStack.pop_back();
			//printf("%d ", v);
		} while(v != s);
		//printf("\n");
	}
}

//finds strongly connected components (megalopolis) of a graph
void tarjan(int n)
{
	myTime = 0;
	index.clear(); low.clear(); onstack.clear(); myStack.clear(); myroot.clear(); visited.clear();
	index.resize(n, 0); low.resize(n, 0); onstack.resize(n, 0); myroot.resize(n, -1); visited.resize(n, 0);
	int i;
	for(i = 0; i < n; i++)
	{
		if(!visited[i])
			dfs_scc(i);
	}
}
//<---

//select such a subset E' of E (edges), that none of v<->w belonging to E' satisfies condition that both v and w belong to the same SCC (matropolis)
//select() visits every edge from E and adds it to the selected ones if it connects two different strongly connected components
void select(int n)
{
    edges.clear();
    int i;
    for(i = 0; i < n; i++)
    {
        for(vector<pair<int, int> >::iterator v = graph[i].begin(); v != graph[i].end(); v++)
        {
            //if root of i is not the same as root of v, then i and v belong to different SCCs
            //so we can safely add i<->v to our set of selected edges
            if(myroot[i] != myroot[v->first])
            {
                edges.push_back(E(i, v->first, v->second));
                total_cost += v->second;
            }
        }
    }
}

//initialises disjoint set structure
//every node belongs to its own set
void init_ds(int n)
{
    father.clear(); rank.clear();
    father.resize(n); rank.resize(n);
    for(int i = 0; i < n; i++)
    {
        father[i] = i;
        rank[i] = 0;
    }
}

//finds set of s
int find_set(int s)
{
    if(father[s] != s)
        father[s] = find_set(father[s]);
    return father[s];
}

//joins two sets by their roots
int link(int u, int v)
{
    if(rank[u] > rank[v])
        father[v] = u;
    else
    {
        if(rank[u] == rank[v])
            rank[v]++;
        father[u] = v;
    }
}

//merges two sets
void _union(int u, int v)
{
    link(find_set(u), find_set(v));
}

//finds a minimum spanning tree of SCC graph
int kruskal(int n)
{
    int sum = 0; //total, minimal annual maintenance cost
    init_ds(n);
    sort(edges.begin(), edges.end(), comp);
    for(vector<E>::iterator e = edges.begin(); e != edges.end(); e++)
    {
        //if both ends belong to different sets then we shall add
        //the currently proceeded edge to the MST
        if(find_set(myroot[e->v]) != find_set(myroot[e->w]))
        {
            _union(myroot[e->v], myroot[e->w]);
            sum += e->c;
        }
    }
    for(int i = 1; i < n; i++)
    {
        assert(find_set(myroot[i]) == find_set(myroot[0]));
    }
    return sum;
}

int solve(int n)
{
    tarjan(n);
    select(n);
    return total_cost - kruskal(n);
}

int main()
{
    int n, m, a, b, c;
    scanf("%d %d", &n, &m);
    assert(n <= 1000000 && m <= 1000000);
    graph.clear();
    graph.resize(n);
    unsigned long long costtest=0;
    for(int i = 0; i < m; i++)
    {
        scanf("%d %d %d", &a, &b, &c);
        costtest+=c;
        assert(a>0 && a <= n && b > 0 && b <= n && c <= 1000);
        graph[a - 1].push_back(make_pair(b - 1, c));
    }
    assert(costtest<=1000000000ULL);
    printf("%d\n", solve(n));
    return 0;
}