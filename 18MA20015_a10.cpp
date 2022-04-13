/*
Name : Harshal Dupare
Roint Number : 18MA20015
Assignment Number : 10
*/

#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <cstdlib>

using namespace std;

// struct to store the weight
struct weight
{
    int isum;
    int iprod;
    weight(int _isum = 0, int _iprod = 0)
    {
        this->isum = _isum;
        this->iprod = _iprod;
    }
};

// ignore these as they are for debugging purpose
void printarr(int *, int);
void printwt(weight *, int);

// function the add the weights
weight add(const weight w1, const weight w2)
{
    return weight(w1.isum + w2.isum, w1.iprod + w2.iprod);
}

// function to get the value of the weight in int
int value_of(const weight w)
{
    return w.isum + w.iprod;
}

void dijkstra(int n, int s, int *deg, int **adjl, weight **edge_w, weight *d, const int INF)
{
    int *done = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        done[i] = 0; // 0 for not done
    }
    d[s].iprod = 0; // except for start which is at 0 distance hence also done
    d[s].isum = 0;  // except for start which is at 0 distance hence also done

    for (int i = 0; i < n; i++)
    {
        // find the smallest distance node from the previous itteration
        int v = -1;
        for (int j = 0; j < n; j++)
        {
            if (done[j] == 0 && (v == -1 || value_of(d[j]) < value_of(d[v])))
            {
                v = j;
            }
        }

        if (d[v].isum == INF)
        {
            break;
        }

        done[v] = 1; // mark it as done
        for (int _i = 0; _i < deg[v]; _i++)
        {
            int to = adjl[v][_i];

            // calculate the new weight
            weight _wt = edge_w[v][_i];
            weight new_wt = add(_wt, d[v]);

            // compare and if smaller then update
            if (value_of(new_wt) < value_of(d[to]))
            {
                d[to].iprod = new_wt.iprod;
                d[to].isum = new_wt.isum;
            }
            // printwt(d,n);
        }
    }
}

// ignore these as they are for debugging purpose
void printarr(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << ", ";
    }
    cout << endl;
}

void printwt(weight *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << "(" << arr[i].isum << "," << arr[i].iprod << "), ";
    }
    cout << endl;
}

// main
int main()
{
    // freopen("input.txt", "r", stdin); // for text file input
    int n;
    cout << "Write the number of vertices in the undirected graph: ";
    cin >> n;
    cout << "Write -1 to indicate the end of neighbors" << endl;

    int **adjl = (int **)malloc(n * sizeof(int *));
    int **edge_w = (int **)malloc(n * sizeof(int *));
    int *node_w = (int *)malloc(n * sizeof(int));
    int *deg = (int *)malloc(n * sizeof(int));
    int *temp = (int *)malloc(n * sizeof(int));

    // take graph edges as input
    for (int i = 0; i < n; i++)
    {
        edge_w[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
        {
            edge_w[i][j] = 0;
        }

        cout << "Write neighbors of vertex " << i + 1 << ": ";
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
            adjl[i][j] = temp[j] - 1;
        }
    }

    // take edge weights as input
    for (int u = 0; u < n; u++)
    {
        for (int d = 0; d < deg[u]; d++)
        {
            int v = adjl[u][d];
            if (u < v)
            {
                cout << "Weight of the edge {" << u + 1 << "," << v + 1 << "}: ";
                cin >> edge_w[u][v];
                edge_w[v][u] = edge_w[u][v];
            }
        }
    }

    // take node weights as input
    for (int u = 0; u < n; u++)
    {
        cout << "Weight of the vertex " << u + 1 << ": ";
        cin >> node_w[u];
    }

    int s;
    cout << "Write source vertex: ";
    cin >> s;
    s--;

    // My idea is to transform the problem on n nodes for undirected graph G to
    // 2n nodes on directed graph H in the following way
    // for each node u in G we have 2 nodes in H say uin, uout
    // s.t. there is an edge with tuple as its value (isum,iprod)
    // for any u in G we have uin and uout in H and edge uin->uout  between them as (0,vertex_weight[u])
    // and for all the v which have edge with u in G we have edges in H as follows
    // uout -> vin = (edge_weight[u][v],0), and
    // vout -> uin = (edge_weight[v][u],0) (note both the values are the same)
    // and on tuple addition is defined as (x,y) + (z,w) = (x+z,y*w)
    // after the update on assignment it is chnaged to (x,y) + (z,w) = (x+z,y+w)
    // then on the converted transformed graph H the shortest path will corresponnd to the
    // shortest path for G based on the given discription

    // create the graph H
    int INF = 100000;
    int n_h = 2 * n;
    int **adjl_h = (int **)malloc(n_h * sizeof(int *));
    int *deg_h = (int *)malloc(n_h * sizeof(int));
    weight *dist = (weight *)malloc(n_h * sizeof(weight));
    weight **edge_w_h = (weight **)malloc(n_h * sizeof(weight *));
    // mapping is uin = 2*u and uout = 2*u+1
    for (int i = 0; i < n; i++)
    {
        int iin = 2 * i, iout = 2 * i + 1;

        dist[iin].iprod = INF;
        dist[iin].isum = INF;
        dist[iout].iprod = INF;
        dist[iout].isum = INF;

        adjl_h[iin] = (int *)malloc(1 * sizeof(int));       // this is uin hence only 1 out edge to uout
        adjl_h[iout] = (int *)malloc(deg[i] * sizeof(int)); // uout hence deg[i] edge

        edge_w_h[iin] = (weight *)malloc(1 * sizeof(weight));
        edge_w_h[iout] = (weight *)malloc(deg[i] * sizeof(weight));

        deg_h[iin] = 1;
        deg_h[iout] = deg[i];

        adjl_h[iin][0] = iout;
        edge_w_h[iin][0].isum = 0;
        edge_w_h[iin][0].iprod = node_w[i];

        for (int ii = 0; ii < deg[i]; ii++)
        {
            adjl_h[iout][ii] = 2 * adjl[i][ii]; // to in of other nodes
            edge_w_h[iout][ii].isum = edge_w[i][adjl[i][ii]];
            edge_w_h[iout][ii].iprod = 0;
        }

        // printarr(adjl_h[iin],1);
        // printwt(edge_w_h[iin],1);
        // printarr(adjl_h[iout],deg[i]);
        // printwt(edge_w_h[iout],deg[i]);
    }

    dijkstra(n_h, 2 * s, deg_h, adjl_h, edge_w_h, dist, INF);

    for (int i = 0; i < n; i++)
    {
        if (i == s)
        {
            continue;
        }
        cout << "Distance of vertex " << i + 1 << " from vertex " << s + 1 << ": ";
        cout << value_of(dist[2 * i + 1]) << endl;
    }

    // printwt(dist,n_h);

    return 0;
}