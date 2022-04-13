/*
Name : Harshal Dupare
Roint Number : 18MA20015
Assignment Number : 
*/

#include <iostream>
#include <stdio.h>

using namespace std;

/*
dp defination that we are using is

dp is of size n*n*N = n*n*5 = O(n^2)
dp[i][j][k] = number of ways that we can form souce k by using string s[i....j]
initial condition d[i][i][s[i]] = 1 and everything else is 0

and dp[i][j][k] = sum over t =i...j-1 of { dp[i][t][s1]*dp[t+1][j][s2] }
st. k = M[s1][s2]

note here that N = 5 , constant

hence to compute each i,j,k pair we need O(n*N*N)=O(n*25)=O(n) time at max

and there are total n*n*N such terms hence we need O(n^3*N^3) time = O(n^3) time

and for space we need O(n*n*N) = O(n^2)

hence both have the required time and space complexity

*/

// function signature is different as we require the length of string
int possible(int n, int N, int id, int *s, int **M)
{
    // initlize dp table to all zeros
    int ***dp = (int ***)malloc((n + 1) * sizeof(int **));
    for (int i = 0; i < n + 1; i++)
    {
        dp[i] = (int **)malloc((n + 1) * sizeof(int *));
        for (int j = 0; j < n + 1; j++)
        {
            dp[i][j] = (int *)malloc((N + 1) * sizeof(int));
            for (int t = 0; t <= N; t++)
            {
                dp[i][j][t] = 0;
            }
        }
    }

    // intilize the dp[i][i][s[i]] value to 1
    // as it can be obtained in 1 way i.e. itself
    for (int i = 1; i <= n; i++)
    {
        dp[i][i][s[i]] = 1;
    }

    // use the dp defination to calculate for the value of dp for further cases
    for (int l = 2; l <= n; l++)
    {
        for (int i = 1; i + l - 1 <= n; i++)
        {
            int j = i + l - 1;
            for (int s1 = 1; s1 <= N; s1++)
            {
                for (int s2 = 1; s2 <= N; s2++)
                {
                    int sr = M[s1][s2];
                    for (int t = i; t < j; t++)
                    {
                        dp[i][j][sr] += dp[i][t][s1] * dp[t + 1][j][s2];
                    }
                }
            }
        }
    }

    if (dp[1][n][id] > 0)
    {
        return 1;
    }
    return 0;
}

// function signature is different as we require the length of string
void countways(int n, int N, int *s, int **M)
{
    // initlize dp table to all zeros
    int ***dp = (int ***)malloc((n + 1) * sizeof(int **));
    for (int i = 0; i < n + 1; i++)
    {
        dp[i] = (int **)malloc((n + 1) * sizeof(int *));
        for (int j = 0; j < n + 1; j++)
        {
            dp[i][j] = (int *)malloc((N + 1) * sizeof(int));
            for (int t = 0; t <= N; t++)
            {
                dp[i][j][t] = 0;
            }
        }
    }

    // intilize the dp[i][i][s[i]] value to 1
    // as it can be obtained in 1 way i.e. itself
    for (int i = 1; i <= n; i++)
    {
        dp[i][i][s[i]] = 1;
    }

    // use the dp defination to calculate for the value of dp for further cases
    for (int l = 2; l <= n; l++)
    {
        for (int i = 1; i + l - 1 <= n; i++)
        {
            int j = i + l - 1;
            for (int s1 = 1; s1 <= N; s1++)
            {
                for (int s2 = 1; s2 <= N; s2++)
                {
                    int sr = M[s1][s2];
                    for (int t = i; t < j; t++)
                    {
                        dp[i][j][sr] += dp[i][t][s1] * dp[t + 1][j][s2];
                    }
                }
            }
        }
    }

    cout << "Number of ways in which mixing ";
    for (int i = 1; i <= n; i++)
    {
        cout << s[i];
    }
    cout << " results in" << endl;
    for (int i = 1; i <= N; i++)
    {
        cout << i << ": " << dp[1][n][i] << endl;
    }
    return;
}

int main()
{
    int N = 5;
    int **M = (int **)malloc((N + 1) * sizeof(int *));
    for (int i = 0; i < N + 1; i++)
    {
        M[i] = (int *)malloc((N + 1) * sizeof(int));
    }
    cout << "Enter M:" << endl;
    cout << endl;
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cin >> M[i][j];
        }
    }
    cout << endl;

    int n_max = 100;
    int *s = (int *)malloc((n_max + 1) * sizeof(int));
    char str[n_max + 1];
    cout << "Enter string: ";
    scanf("%s", str);

    int n = 0;
    while (str[n] != '\0')
    {
        s[n + 1] = str[n] - '0';
        n++;
    }
    cout << endl;
    for (int i = 1; i <= N; i++)
    {
        cout << i << ": ";
        if (possible(n, N, i, s, M) == 0)
        {
            cout << "Not Possible" << endl;
        }
        else
        {
            cout << "Possible" << endl;
        }
    }
    cout << endl;

    countways(n, N, s, M);

    return 0;
}