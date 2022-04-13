/*
Name : Harshal Dupare
Roint Number : 18MA20015
Assignment Number : 4
*/

#include <iostream>

using namespace std;

// merge sort
void mymerge(int *a, int *p, int i, int mid, int j)
{
    int n1 = mid - i + 1;
    int n2 = j - mid;
    int *ra = (int *)malloc((n1 + n2) * sizeof(int));
    int *rp = (int *)malloc((n1 + n2) * sizeof(int));
    int id = 0;
    int t = mid + 1;

    while (i < mid + 1 && t < j + 1)
    {
        if (a[i] <= a[t])
        {
            ra[id] = a[i];
            rp[id] = p[i];
            i++;
        }
        else
        {
            ra[id] = a[t];
            rp[id] = p[t];
            t++;
        }
        id++;
    }
    while (i < mid + 1)
    {
        ra[id] = a[i];
        rp[id] = p[i];
        i++;
        id++;
    }
    while (t < j + 1)
    {
        ra[id] = a[t];
        rp[id] = p[t];
        t++;
        id++;
    }
    i -= n1;
    t = i;
    for (; i < j + 1; i++)
    {
        a[i] = ra[i - t];
        p[i] = rp[i - t];
    }
}

void mergesort(int *a, int *p, int i, int j)
{
    if (i >= j)
        return;

    int mid = (i + j) / 2;
    mergesort(a, p, i, mid);
    mergesort(a, p, mid + 1, j);
    mymerge(a, p, i, mid, j);
    return;
}
// merge sort

// function to get a random permutation or identity permutation
void permit(int n, int *p, bool reset = false)
{
    for (int i = 0; i < n; i++)
    {
        p[i] = i;
    }

    if (reset)
    {
        return;
    }

    srand(time(0));
    for (int i = n; i > 0; i--)
    {
        int id = rand() % i;
        if (id < 0)
        {
            id += i;
            id %= i;
        }
        // swap i-1 and id
        int t = p[id];
        p[id] = p[i - 1];
        p[i - 1] = t;
    }
}

// function to get max lateness of the permutation p
int get_lateness(int n, int *l, int *d, int *p, bool d_changed = true)
{
    int lam = -1;
    int curr_start = 0;
    for (int i = 0; i < n; i++)
    {
        // update the max lateness
        // note here if d is ordered as per p then we need to sub d[i] else
        // if d is not ordered as per p then we need to subtract d[p[i]]
        lam = max(lam, max(0, curr_start + l[p[i]] - d[(d_changed ? i : p[i])]));
        // update the next starting time
        curr_start += l[p[i]];
    }
    return lam;
}

// struct to store the permutation data
struct perm
{
    bool poss;
    int _n;
    int *p;
    int lam;
    perm(int n)
    {
        this->_n = n;
        this->lam = -1;
        this->p = (int *)malloc(n * sizeof(int));
        this->poss = false;
    }
};

// idea is to set index and check if the lateness so far exceeds our limit
// if it does then we dont need to compute that branch
// this is recurssion with pruning
perm is_poss(int n, int *l, int *d, int *p, int k, int lam, int curr_lam, int curr_start)
{
    if (k == n)
    {
        // if we reach the end
        // -> that curr_lam <= lam
        // hence make it true and return
        // process the permutation
        perm pp(n);
        for (int i = 0; i < n; i++)
        {
            pp.p[i] = p[i];
        }
        int c_lam = get_lateness(n, l, d, p, false);
        pp.lam = c_lam;
        pp.poss = c_lam <= lam;
        return pp;
    }
    for (int i = k; i < n; i++)
    {
        int t = p[k];
        p[k] = p[i];
        p[i] = t;
        int new_lam = curr_lam;
        new_lam = max(new_lam, max(0, curr_start + l[p[i]] - d[p[i]]));
        int new_start = curr_start + l[p[i]];

        if (k > 0 && d[p[k - 1]] > d[p[k]])
        {
            // using the exchage argument to prune the tree
            t = p[i];
            p[i] = p[k];
            p[k] = t;
            continue;
        }

        if (new_lam <= lam)
        {
            // recurse this tree down only if
            // the new_lam is less than tbe target lam
            perm pq = is_poss(n, l, d, p, k + 1, lam, new_lam, new_start);
            if (pq.poss)
            {
                return pq;
            }
        }
        // restore the k'th index
        // and repeat
        t = p[i];
        p[i] = p[k];
        p[k] = t;
    }
    // if not found return as it is
    perm pp(n);
    return pp;
}

// idea is to find the lower and upper bounds for lam
// and then use recussion and pruning to find it the permutation exists for such lam
int part_1(int n, int *l, int *d, int *p, int m = 10)
{
    // idea will be to get the initial guess we will use m number of random permutations
    // and calculate their lateness and use min of them
    // and then we will go only till that value and not more
    // here m = 10 i.e. we will use 10 random permutations

    int curr_lam = INT32_MAX;
    int *curr_p = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < m; i++)
    {
        // get random permutation
        permit(n, curr_p);
        // update the max lateness
        curr_lam = min(curr_lam, get_lateness(n, l, d, curr_p, false));
    }

    // say lam is lateness then we can write the max starting time for i as
    // s[i] = d[i]-l[i]+lam
    // since no job can start before 0 we need s[i] >= 0 hence lam >= l[i] - d[i]
    // least possible value of lam = max(0,...,l[i] - d[i],...)
    int s_lam = 0; // starting value of lam
    for (int i = 0; i < n; i++)
    {
        s_lam = max(s_lam, l[i] - d[i]);
    }
    // start the loop to find the lower lam than the curr_lam
    bool found_better = false;

    for (int lam = s_lam; lam < curr_lam; lam++)
    {
        // reset p each time
        permit(n, p, true);
        perm pp = is_poss(n, l, d, p, 0, lam, -1, 0);
        if (pp.poss)
        {
            curr_lam = lam;
            for (int i = 0; i < n; i++)
            {
                p[i] = pp.p[i];
            }
            found_better = true;
            break;
        }
    }
    if (!found_better)
    {
        for (int i = 0; i < n; i++)
        {
            p[i] = curr_p[i];
        }
    }
    return curr_lam;
}

int part_2(int n, int *l, int *d, int *p)
{
    // by exchange argument we can prove that the optimal greedy algorithm
    // is the earliest deadline first algorithm
    int lam = -1;
    // sort d and p as per the values of d
    mergesort(d, p, 0, n - 1);
    // get the lateness
    lam = get_lateness(n, l, d, p, true);
    return lam;
}

int main()
{
    int n;
    cout << "Write the number of jobs: ";
    cin >> n;
    int *d = (int *)malloc(n * sizeof(int));
    int *l = (int *)malloc(n * sizeof(int));
    int *p = (int *)malloc(n * sizeof(int));
    cout << "Write the length of the jobs: ";
    for (int i = 0; i < n; i++)
    {
        cin >> l[i];
        p[i] = i;
    }
    cout << "Write the deadline of the jobs: ";
    for (int i = 0; i < n; i++)
    {
        cin >> d[i];
    }

    int lam = part_1(n, l, d, p);
    cout << "Optimal schedule by late guessing algorithm is:\n";
    for (int i = 0; i < n; i++)
    {
        cout << p[i] + 1 << ", ";
    }
    cout << endl;
    cout << "Maximum lateness is " << get_lateness(n, l, d, p, false) << endl;

    for (int i = 0; i < n; i++)
    {
        p[i] = i;
    }
    lam = part_2(n, l, d, p);
    cout << "Optimal schedule by greedy algorithm is:\n";
    for (int i = 0; i < n; i++)
    {
        cout << p[i] + 1 << ", ";
    }
    cout << endl;
    cout << "Maximum lateness is " << lam << endl;

    return 0;
}