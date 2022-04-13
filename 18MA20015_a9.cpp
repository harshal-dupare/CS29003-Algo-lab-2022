/*
Name : Harshal Dupare
Roint Number : 18MA20015
Assignment Number : 9
*/

#include <iostream>
#include <stdio.h>

using namespace std;

// ignore this myvector struct
// i implimented it thinking wrt the earlier input format without the number of
// edges on each node specified
struct myvector
{
    int capacity = 36;
    int at;
    int *a;
    myvector()
    {
        this->at = 0;
        this->a = (int *)malloc(this->capacity * sizeof(int));
    }

    void push_back(int ele)
    {
        if (this->capacity <= this->at)
        {
            int *new_a = (int *)malloc((2 * this->at) * sizeof(int));
            for (int i = 0; i < this->capacity; i++)
            {
                new_a[i] = this->a[i];
            }
            this->a = new_a;
            this->capacity = (2 * this->at);
        }
        this->a[this->at] = ele;
        this->at++;
        return;
    }

    void pop_back(int ele)
    {
        if (this->at < 1)
        {
            return;
        }
        if (this->capacity >= 2 * (this->at))
        {
            int *new_a = (int *)malloc((this->at + 1) * sizeof(int));
            for (int i = 0; i < this->at - 1; i++)
            {
                new_a[i] = this->a[i];
            }
            this->a = new_a;
            this->capacity = (this->at + 1);
        }
        this->at--;
        return;
    }
};

/*
Ideas is to use dfs and check if we ever visit a vertex of different color
adn we assign color which are forced onto the edges as their neighbour need to
have a different color
if we do then that there a edge between 2 edges of different color
meaning that there a common student
*/
bool dfs_biparted(int c, int n, int e, int v, int *deg, int **adjl, int *color)
{
    color[v] = c;
    for (int i = 0; i < deg[v]; i++)
    {
        int u = adjl[v][i];
        if (color[u] == color[v])
        {
            // if there is edge between same color i.e. its not biparted
            return false;
        }
        if (color[u] == -1)
        {
            // this is not yet visted
            // so call dfs on this too
            bool poss = dfs_biparted((c + 1) % 2, n, e, u, deg, adjl, color);
            if (!poss)
            {
                // is any conflict if found in its dfs childs then return false
                return false;
            }
        }
    }
    return true;
}

bool exists_schedule(int n, int e, int *deg, int **adjl)
{
    // to handel if there are more than one component
    int *color = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        color[i] = -1;

    for (int i = 0; i < n; i++)
    {
        if (color[i] == -1)
        {
            bool poss = dfs_biparted(0, n, e, i, deg, adjl, color);
            if (!poss)
            {
                return false;
            }
        }
    }
    return true;
}

void dfs_cut_vertex(int &time, int n, int v, int pred, int *deg, int **adjl, int *color, int *disc_time, int *low_time)
{
    int dis_count = 0;
    color[v] = 0;        // mark as visited color 0, 1 as being a cut vertex, -1 not yet visited
    time++;              // incriment counter as we viisted one vertex
    disc_time[v] = time; // time at which we discover it
    low_time[v] = time;  // min time of discovering its decendents

    for (int i = 0; i < deg[v]; i++)
    {
        int u = adjl[v][i];
        if (u == pred)
        {
            continue;
        }
        if (color[u] == -1)
        {
            // this is not yet visted
            // so call dfs on this too
            dfs_cut_vertex(time, n, u, v, deg, adjl, color, disc_time, low_time);

            low_time[v] = min(low_time[v], low_time[u]);
            if (pred != -1)
            {
                // meaning this is not the first vertex to be visited in
                // its componennt
                if (low_time[u] >= disc_time[v])
                {
                    // this is a cut vertex
                    color[v] = 1;
                    dis_count++;
                }
            }
        }
        else
        {
            low_time[v] = min(low_time[v], disc_time[u]);
        }
    }

    if (pred == -1 && dis_count > 1)
    {
        // this is also a cut vertex
        color[v] = 1;
    }
}

void find_trvial(int n, int e, int *deg, int **adjl)
{
    int time = 0;
    int *low_time = (int *)malloc(n * sizeof(int));
    int *disc_time = (int *)malloc(n * sizeof(int));
    int *color = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        low_time[i] = -1;
        disc_time[i] = -1;
        color[i] = -1;
    }

    for (int i = 0; i < n; i++)
    {
        if (color[i] == -1)
        {
            dfs_cut_vertex(time, n, i, -1, deg, adjl, color, disc_time, low_time);
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (color[i] == 0)
        {
            cout << i << " ";
        }
    }
    cout << endl;
    return;
}

int main()
{
    int n, e;
    cout << "n = ";
    cin >> n;
    cout << "e = ";
    cin >> e;
    cout << "\nReading edges..." << endl;

    // since it is not mentioned how many edges are there for any of the vertex
    // im assuming that that is given as one input for eq
    // $ 0 : 3 1 8 9
    // which means that 0 has 3 edges on it and those are 1 8 9
    int **adjl = (int **)malloc(n * sizeof(int *));
    int *deg = (int *)malloc(n * sizeof(int));
    int *temp = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        cout << " " << i << ": ";
        int tp = 0;
        do
        {
            cin >> temp[tp];
            tp++;
        } while (temp[tp - 1] != -1);
        tp--;
        deg[i] = tp;
        adjl[i] = (int *)malloc((deg[i] + 1) * sizeof(int));
        for (int j = 0; j < deg[i]; j++)
        {
            adjl[i][j] = temp[j];
        }
    }

    // note that conflict free schedeule meaning that graph is biparted as we can divide it into
    // 2 classes where none of them share an edge
    bool exist_schedule_op = exists_schedule(n, e, deg, adjl);
    if (exist_schedule_op)
    {
        cout << "\nThere exists a conflict-free schedule\n"
             << endl;
    }
    else
    {
        cout << "\nThere exists no conflict-free schedule\n"
             << endl;
    }

    cout << "The trivial vertices of the graph are:" << endl;
    find_trvial(n, e, deg, adjl);

    return 0;
}