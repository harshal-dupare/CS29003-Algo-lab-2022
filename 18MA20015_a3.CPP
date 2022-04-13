/*
Name : Harshal Dupare
Roint Number : 18MA20015
Assignment Number : 3 Divide-and-Conquer
*/

#include <iostream>
using namespace std;

/*
r : pointer to the array of length n
n : length of the array
Complexity : O(n^2)
*/
int dist1(int *r, int n)
{
    int dst = 0;
    // runs for n values
    for (int i = 0; i < n; i++)
    {

        // runs for n-i values
        for (int j = i + 1; j < n; j++)
        {
            // if satisfies then incriment the count
            if (r[i] > 2 * r[j])
            {
                dst++;
            }
        }
    }
    return dst;
}

/*
a : pointer to the array
i : start of array 1
mid : end of array 1 & mid+! : start of array 2
j : end of array 2
Complexity : O(n)
returns the contribution between two arrays
*/
int mymerge(int *a, int i, int mid, int j)
{
    int n1 = mid - i + 1;
    int n2 = j - mid;

    int *ra = (int *)malloc((n1 + n2) * sizeof(int));
    int *donei = (int *)malloc((n1 + n2) * sizeof(int));
    for (int t = 0; t < n1 + n2; t++)
    {
        donei[t] = 1;
        // donei[t] = 1 means the contribution from the a[t] is
        // accounted for else its not accounted for
    }

    int id = n1 + n2 - 1;
    int ti = mid, tj = j;
    int invct = 0;

    // we start by comparing the last terms in both the sorted array
    // we place the max of then terms at the end of the array
    // this loop also manages the sorting and coounts for one part of the dist
    // O(n1+n2)
    while (ti >= i && tj > mid)
    {
        if (a[ti] <= a[tj])
        {
            // implies a[ti] <= 2*a[tj]
            // hence a[k] <= 2*a[tj] for all i <= k <= ti as a[i...mid] is sorted
            // hence no contribution on by this term
            ra[id] = a[tj];
            tj--;
        }
        else
        {
            // i.e. a[ti] > a[tj]
            if (a[ti] > 2 * a[tj])
            {
                // hence a[ti] > 2*a[k] for all tj >= k > mid as a[mid+1....j] is sorted
                invct += tj - mid;
            }
            else
            {
                // here we cant say anything
                // so we will process it later
                donei[ti - i] = 0;
            }
            ra[id] = a[ti];
            ti--;
        }
        id--;
    }
    while (ti >= i)
    {
        ra[id] = a[ti];
        ti--;
        id--;
    }
    while (tj > mid)
    {
        ra[id] = a[tj];
        tj--;
        id--;
    }

    // now we will process the terms a[i] for whose contribution is not accounted for in the first part
    ti = mid;
    tj = j;
    // this loop just counts and doesn't manage sorting
    // O(n1+n2)
    while (ti >= i && tj > mid)
    {
        if (donei[ti - i] == 1)
        {
            // if this is done then skip and go to next
            ti--;
            continue;
        }

        if (a[ti] <= 2 * a[tj])
        {
            // as a[ti] <= 2*a[tj] --> a[k] <= 2*a[tj] ti >= k >= i
            // hence no contribution from this term a[tj]
            // hence go to next term
            tj--;
        }
        else
        {
            // a[ti] > 2*a[tj] --> a[ti] > 2*a[k]  for  tj >= k > mid
            // hence we get a contribution from this term
            invct += tj - mid;
            // now go to next step
            ti--;
        }
    }

    // now put the sorted array back into a
    for (ti = i; ti < j + 1; ti++)
    {
        a[ti] = ra[ti - i];
    }

    return invct;
}

/*
a : pointer to the array
i : start of the array
j : end of the array
Complexity : T(n) = 2*T(n/2)+O(2*n) = O(nlogn)
*/
int dist2(int *a, int i, int j)
{
    if (i >= j)
        return 0;

    int mid = (i + j) / 2;
    int dst = dist2(a, i, mid);
    dst += dist2(a, mid + 1, j);
    dst += mymerge(a, i, mid, j);
    return dst;
}

int main()
{
    int n;
    cout << "n = ";
    cin >> n;
    cout << endl;

    int *r = (int *)malloc(n * sizeof(int));
    cout << "Ranking: ";
    for (int i = 0; i < n; i++)
    {
        cin >> r[i];
    }
    cout << endl;
    int dist = dist1(r, n);
    cout << "Distance by Method 1: " << dist << endl;

    // for this method my idea is to count this dist just as inversion count
    // but note that this is little more complex due to that 2 factor in comprison
    // since i<j , a[i] > a[j] we can do both sort and update the counter
    // but in case of counting a[i] > 2*a[j] we cant always do both sorting and counting
    // so in my first loop I will sort and count the contribution due to one case
    // and in second case I will count the contribution only
    // hence this algorithm has complexity same as merge sort but with an extra factor of 2(approximately)
    dist = dist2(r, 0, n - 1);
    cout << "Distance by Method 2: " << dist << endl;

    return 0;
}