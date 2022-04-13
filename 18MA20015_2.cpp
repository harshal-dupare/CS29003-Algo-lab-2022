/*
Name : Harshal Dupare
Roll Number : 18MA20015
Assignment Number : 3
*/

#include <iostream>

using namespace std;

// struct to store the sell pattern
// only required for recursive function
struct sellpattern
{
    int m;
    int sell_val;
    int *sell;
    sellpattern()
    {
        m = 0;
        sell_val = 0;
        sell = nullptr;
    }

    sellpattern(int _m)
    {
        this->m = _m;
        this->sell_val = 0;
        this->sell = (int *)malloc(_m * sizeof(int));
    }
};

// this take lot of time even for the first one as it has huge dependence of value of n
// rec_sell insted which is a better rec def without using memoization
sellpattern rec_sell_slow(int n, int M, int m, int *fz, int *fp, int *sell, int &sell_val)
{
    sellpattern msp(M);
    for (int i = 0; i < M; i++)
    {
        msp.sell[i] = sell[i];
    }
    msp.sell_val = sell_val;

    for (int i = 0; i < M; i++)
    {
        // if num of apples requierd is less then continue
        if (n < fz[i])
            continue;

        // updaet the sell pattern
        sell[i] += fz[i];
        sell_val += fp[i] * fz[i];

        // use the rec defination
        sellpattern sp = rec_sell_slow(n - fz[i], M, m, fz, fp, sell, sell_val);
        if (sp.sell_val > msp.sell_val)
        {
            // if new is better update the max selling pattern
            msp.sell_val = sp.sell_val;
            for (int j = 0; j < m; j++)
            {
                msp.sell[j] = sp.sell[j];
            }
        }
        // restore the selling pattern
        sell[i] -= fz[i];
        sell_val -= fp[i] * fz[i];
    }

    return msp;
}

// this is much faster version of the recursion
// converting it to dp requires hash function or other complex data structure
// so switched to the `rec_sell` with little modified recursion definition insted
// complexity is O[n^m/(fz[0]*fz[1]*...fz[m-1])] - this is very crude upper bound
sellpattern rec_sell(int n, int M, int m, int *fz, int *fp, int *sell, int &sell_val)
{
    // here insted of adding fz[j] each time we add multiples of fz[j] as to decrease the branching at those levels
    // hence we also dont need to visit any family twice hence this further improves the complexity
    // so this is much faster version
    if (m == 0)
    {
        // if no more family left then just copy the sell pattern in struct
        // and return
        sellpattern sp(M);
        for (int i = 0; i < M; i++)
        {
            sp.sell[i] = sell[i];
        }
        sp.sell_val = sell_val;
        return sp;
    }

    // struct to store the max sell object
    sellpattern msp(m);
    int id = m - 1; // id of the family
    for (int si = 0; si * fz[id] <= n; si++)
    {
        // add the selling value si*fz[id] to the sell array
        sell[id] = si * fz[id];
        // add its value
        sell_val += si * fp[id] * fz[id];
        // recursive defination
        sellpattern sp = rec_sell(n - si * fz[id], M, m - 1, fz, fp, sell, sell_val);
        if (sp.sell_val > msp.sell_val)
        {
            // if new sell pattern is better assign it to the max sell pattern msp
            msp.sell_val = sp.sell_val;
            for (int i = 0; i < M; i++)
            {
                msp.sell[i] = sp.sell[i];
            }
        }
        // restore the original sell pattern
        sell[id] = 0;
        sell_val -= si * fp[id] * fz[id];
    }
    return msp;
}

// complexity is O(n*m*m)
int dp_sell(int n, int m, int *fz, int *fp, int *sell)
{
    // create the required arrays
    // and initilize em as 0 sell pattern i.e. no apples sold to anyone
    int *dp = (int *)malloc((n + 1) * sizeof(int));
    int **dpl = (int **)malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++)
    {
        dp[i] = 0;
        dpl[i] = (int *)malloc(m * sizeof(int));
        for (int j = 0; j < m; j++)
            dpl[i][j] = 0;
    }

    // we go in the order defined by recursion by calculating for increasing n values
    for (int i = 0; i <= n; i++)
    {
        // for each i value see if the last set of apple was given to which family
        // if it was given to family j then get the optimal value of dp[i-fz[j]]
        // and loop through all j and choose the max of them
        for (int j = 0; j < m; j++)
        {
            if (fz[j] <= i)
            {
                // if new is maximum update the dpl[i] accordingly
                if (dp[i] < dp[i - fz[j]] + fp[j] * fz[j])
                {
                    dp[i] = dp[i - fz[j]] + fp[j] * fz[j];
                    for (int t = 0; t < m; t++)
                    {
                        dpl[i][t] = dpl[i - fz[j]][t];
                    }
                    dpl[i][j] += fz[j];
                }
            }
        }
    }

    // return in the sell array the optimal selling pattern
    for (int i = 0; i < m; i++)
    {
        sell[i] = dpl[n][i];
    }
    return dp[n];
}

int main()
{
    // problem formulation
    // max Sum fp[i]*fz[i]*sell[i]
    // given Sum fz[i]*sell[i] <= n
    // we we define rec on rec[i] = max selling that can be done with i applies
    // now we can recurse as rec[n] = max{rec[n-fz[i]]+fz[i]*fp[i]}
    // for dp: we can use bottom up dp using this definition to traverse from i = 0 ... n and calculate for each
    // using bottom up as it is natural order to calculate it
    int n, m;
    cout << "Write the number of apples that Ramu has: ";
    cin >> n;
    cout << "Write the number of buyers: ";
    cin >> m;
    cout << "Write family sizes:" << endl;
    int *fz = (int *)malloc(m * sizeof(int));
    int *fp = (int *)malloc(m * sizeof(int));
    int *sell = (int *)malloc(m * sizeof(int));
    for (int i = 0; i < m; i++)
    {
        cin >> fz[i];
        sell[i] = 0;
    }
    cout << "Write prices:" << endl;
    for (int i = 0; i < m; i++)
    {
        cin >> fp[i];
    }

    // use fast version of recursion
    int sell_val = 0;
    sellpattern max_sell_rec = rec_sell(n, m, m, fz, fp, sell, sell_val);
    cout << "Output of recursive implementation" << endl;
    cout << "Maximum earning is " << max_sell_rec.sell_val << ". Selling pattern is" << endl;
    for (int i = 0; i < m; i++)
    {
        cout << max_sell_rec.sell[i] << ", ";
    }
    cout << endl;

    int max_sell_dp = dp_sell(n, m, fz, fp, sell);
    cout << "Output of dynamic programming implementation" << endl;
    cout << "Maximum earning is " << max_sell_dp << ". Selling pattern is" << endl;
    for (int i = 0; i < m; i++)
    {
        cout << sell[i] << ", ";
    }
    cout << endl;
    return 0;
}