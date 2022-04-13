/*
Name : Harshal Dupare
Roint Number : 18MA20015
Assignment Number : test 2
*/

#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <cstdlib>

using namespace std;

// as used in last assignments
void read_graph(int n, int ** adjl, int * deg)
{
    cout << "Reading edges..." << endl;
    int *temp = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n-1; i++)
    {
        cout << i << ": ";
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
    return;
}

// logic is as described in problem statement
void greedy_path(int n, int** adjl, int * deg)
{
    int curr = 0;
    int l = 1;
    int * path = (int *)malloc((n+1) *sizeof(int));
    path[0] = 0;

    while(curr!=n-1)
    {
        int best_choice = n+1;
        // note that for this we dont need for loop if it 
        // is assumed that the edges given in input are in increasing order
        // but since no such info is given we need a for loop for this
        for(int i=0;i<deg[curr];i++)
        {
            if(adjl[curr][i] < best_choice)
            {
                best_choice = adjl[curr][i];
            }
        }
        path[l] = best_choice;
        curr = best_choice;
        l++;
    }
    
    cout<<"Greedy:"<<endl;
    cout<<"   Length of the longest path = "<<l-1<<endl;
    cout<<"   Path: ";
    for(int i=0;i<l;i++)
    {
        cout<<path[i];
        if(i!=l-1) cout<<" -> ";
    }
    cout<<endl;
    return;
}

// complexity O(E)
void dp_path(int n, int ** adjl, int * deg)
{
    int * succ = (int *)malloc((n+1) *sizeof(int)); // the optimal next node to jump to
    // len :: the dp value , i.e. the length of longest path from this node to reach the end node
    // follows the dp update while doung dp from right to left (i.e. from high value to low values)
    // len[i] = 1 + max(len[adj[i][0]],len[adj[i][1]],....)
    // as this required the values to be computed of the next nodes before we are going from right to left
    int * len = (int *)malloc((n+1) *sizeof(int)); 
    for(int i=0;i<n+1;i++)
    {
        succ[i] = -1;
        len[i] = -1;
    }

    // initilize the len::dp and succ value
    succ[n-1] = -2;
    len[n-1] = 0;

    for(int i=n-2;i>=0;i--)
    {
        // find the best forward edge complexity O(deg[i])
        int id = -1;
        int mlen = -1;
        for(int j = 0;j<deg[i];j++)
        {
            if(mlen<len[adjl[i][j]])
            {
                mlen = len[adjl[i][j]];
                id = adjl[i][j];
            }
        }
        succ[i] = id;
        len[i] = mlen+1;
    }

    // using the same process as greedy one to recover the entire path in the path array
    int curr = 0;
    int l = 0;
    int * path = (int *)malloc((n+1) *sizeof(int));
    while(curr!=-2)
    {
        path[l] = curr;
        l++;
        curr = succ[curr];
    }

    cout<<"Dynamic Programming:"<<endl;
    cout<<"   Length of the longest path = "<<l-1<<endl;
    cout<<"   Path: ";
    for(int i=0;i<l;i++)
    {
        cout<<path[i];
        if(i!=l-1) cout<<" -> ";
    }
    cout<<endl;
    return;
}

int main()
{
    // freopen("input.txt", "r", stdin); // for text file input
    int n;
    cout << "n = ";
    cin >> n;

    int **adjl = (int **)malloc(n * sizeof(int *));
    int *deg = (int *)malloc(n * sizeof(int));

    // part a
    cout<<endl;
    read_graph(n,adjl,deg);

    // part b
    cout<<endl;
    greedy_path(n,adjl,deg);

    // part c
    /*
    example where greedy doesn't work

    n = 5

    Reading edges...
    0: 1 2 -1
    1: 4 -1
    2: 3 -1
    3: 4 -1

    Greedy:
    Length of the longest path = 2
    Path: 0 -> 1 -> 4

    Dynamic Programming:
    Length of the longest path = 3
    Path: 0 -> 2 -> 3 -> 4
    */

    // part d
    cout<<endl;
    dp_path(n,adjl,deg);

    return 0;
}

