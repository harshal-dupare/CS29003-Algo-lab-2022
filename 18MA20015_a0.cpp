/*
Name : Harshal Dupare
Roll Number : 18MA20015
Assignment Number : 0
*/

#include <iostream>
#include <cstdlib>

using namespace std;

long long print_a(int n, int m, int m_done, int n_remain, int *s)
{
    // if we have assigned all the children sweets then --
    if (m_done == m)
    {
        // -- check if the condition in (a) is satisfied
        int tot = 0;
        for (int i = 0; i < m; i++)
            tot += s[i];

        // if condition doesn't hold return 0
        if (tot != n)
            return 0;

        // if condition holds print and return 1
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < s[i]; j++)
            {
                cout << "S";
            }
            if (i != m - 1)
            {
                cout << "|";
            }
        }
        cout << endl;
        return (long long)1;
    }

    // if assigning last child the sweet then --
    if (m_done == m - 1)
    {
        // -- assign all the remaining sweets else if its 0 then not possible
        if (n_remain == 0)
            return 0;
        s[m_done] = n_remain;
        long long ct = print_a(n, m, m_done + 1, 0, s);
        s[m_done] = 0;
        return ct;
    }

    // else assign all possible numbers of sweets and then --
    long long ct = 0;
    for (int i = 1; i <= n_remain; i++)
    {
        s[m_done] = i;
        // -- continue to check for the next child
        ct += print_a(n, m, m_done + 1, n_remain - i, s);
        s[m_done] = 0;
    }
    return ct;
}

long long print_b(int n, int m, int m_done, int n_remain, int *s)
{
    // if we have assigned all the children sweets then --
    if (m_done == m)
    {
        // -- check if the condition in (b) is satisfied
        int tot = 0;
        bool non_adj = true;
        for (int i = 0; i < m; i++)
            tot += s[i];
        for (int i = 0; i < m - 1; i++)
        {
            // if no adj child get 0 then --> sum of adj child > 0
            if (s[i] + s[i + 1] == 0)
            {
                non_adj = false;
                break;
            }
        }

        // if condition doesn't hold return 0
        if (tot != n || !non_adj)
            return 0;

        // if condition holds print and return 1
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < s[i]; j++)
            {
                cout << "S";
            }
            if (i != m - 1)
            {
                cout << "|";
            }
        }
        cout << endl;
        return (long long)1;
    }

    // if assigning last child the sweet then --
    if (m_done == m - 1)
    {
        // -- assign all the remaining sweets
        s[m_done] = n_remain;
        long long ct = print_b(n, m, m_done + 1, 0, s);
        s[m_done] = 0;
        return ct;
    }

    // else assign all possible number of sweets and then --
    long long ct = 0;
    for (int i = 0; i <= n_remain; i++)
    {
        s[m_done] = i;
        // -- continue to check for the next child
        ct += print_b(n, m, m_done + 1, n_remain - i, s);
        s[m_done] = 0;
    }
    return ct;
}

int main()
{
    int n, m;
    cout << "n = ";
    cin >> n;
    cout << "m = ";
    cin >> m;


    // even though its given that 2 positive integers 
    // I have added case checks below to make sure that input is correct
    // informing user the restrictions on m
    if (m < 1)
    {
        cout << "m >= 1, as  part (a) & (b) requires atleast 1 child to be there" << endl;
        return 0;
    }
    // informing user the restrictions on n
    if (n < 1)
    {
        cout << "n >= 1, as  part (a) requires each child to be given atleast 1 sweets" << endl;
        return 0;
    }

    // even through I have added edge case check, note that the edge cases for a and b are different
    // (a) n >= m and m > 0
    // (b) m > 0 and n >= m/2
    // --> m > 0 and n >= m > 0 --> n >= 1 --> n > 0
    // hence using the most reasonable restrictions
    // if those conditions aren't satisfied then no such assignment exists

    int *s = (int *)malloc(m * sizeof(int));
    for (int i = 0; i < m; i++)
    {
        s[i] = 0;
    }

    cout << "\n(a)" << endl;
    long long tot_a = print_a(n, m, 0, n, s);
    cout << "\nTotal number of distributions = " << tot_a << endl;
    cout << "\n(b)" << endl;
    long long tot_b = print_b(n, m, 0, n, s);
    cout << "\nTotal number of distributions = " << tot_b << endl;
    return 0;
}