/*
Name : Harshal Dupare
Roint Number : 18MA20015
Assignment Number : test 1
*/

#include <iostream>
#include <stdio.h>

using namespace std;

// gives the end value if it was linear
int linearize_end(int *start, int *end, int i)
{
    if (start[i] > end[i])
    {
        return end[i] + 360;
    }
    return end[i];
}

// returns true if i & j overlap
bool overlap(int *start, int *end, int i, int j)
{
    if (start[i] > end[i])
    {
        if (start[j] > end[j])
        {
            // both cross 0 so always a overlap
            return true;
        }
        else
        {
            // i crooses
            // j doesn't crosses
            if (end[j] >= start[i] || start[j] <= end[i])
            {
                return true;
            }
        }
    }
    else
    {
        // i doesnt cross
        if (start[j] > end[j])
        {
            // j does
            if (end[i] >= start[j] || start[i] <= end[j])
            {
                return true;
            }
        }
        else
        {
            // j doesnt
            if (start[i] <= end[j] || start[j] <= end[i])
            {
                return true;
            }
        }
    }

    return false;
}

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
        if (linearize_end(p, a, i) <= linearize_end(p, a, t))
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

// earliest dealine first with edge cases
void greedy(int n, int *start, int *end, int *selected)
{
    // sort by end angle in no-decreasing order
    mergesort(end, start, 0, n - 1);

    // find the one with the overlap with out first greedy choice
    // with max start angle and which crosses the zero line
    int o_id = -1;
    for (int i = 1; i < n; i++)
    {
        if (start[i] > end[i] && overlap(start, end, 0, i))
        {
            // this one crosses the 0 line
            // and overlap with our firsts gredy choice
            // so among all these we need to choose the one with maximum start
            // as that will have the minimum length and inimize the
            // overlap with any other candidates
            if (o_id == -1)
            {
                o_id = i;
            }
            else
            {
                if (start[i] > start[o_id])
                {
                    o_id = i;
                }
            }
        }
    }

    // none such found so apply simple greedy algorithm starting
    // with first greedy choice
    int curr_end = linearize_end(start, end, 0);
    selected[0] = 1;
    int id = 1;
    while (id < n)
    {
        if (start[id] > curr_end && linearize_end(start, end, id) - 360 < start[0])
        {
            selected[id] = 1; // this is selected
            curr_end = linearize_end(start, end, id);
        }
        else
        {
            selected[id] = 0;
        }
        id++;
    }

    if (o_id != -1)
    {
        // if there is a overlap candidate so we will start from it insted
        // and then compare both the last one and the one with this to pick the best one
        int *nselected = (int *)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++)
            nselected[i] = 0;
        nselected[o_id] = 1;
        id = 0;
        curr_end = end[o_id];
        while (id < n)
        {
            if (id == o_id)
            {
                id++;
                continue;
            }

            if (start[id] > curr_end && linearize_end(start, end, id) < start[o_id])
            {
                nselected[id] = 1; // this is selected
                curr_end = linearize_end(start, end, id);
            }
            else
            {
                nselected[id] = 0;
            }
            id++;
        }

        int ct = 0;
        int nct = 0;
        for (int i = 0; i < n; i++)
        {
            if (selected[i] == 1)
            {
                ct++;
            }

            if (nselected[i] == 1)
            {
                nct++;
            }
        }

        if (nct > ct)
        {
            for (int i = 0; i < n; i++)
            {
                selected[i] = nselected[i];
            }
        }
    }
    return;
}

int main()
{

    /*
    logic is same as finding the max such in linear space 
    but there are edge cases here so need to handle them properly

    methode is earliest dealine first with some modification to account for the crossing
    0 and overlap with first choice

    complexity is O(nlog(n))
    */
    int n;
    cout << "Write the number of objects: ";
    cin >> n;
    int *start = (int *)malloc(n * sizeof(int));
    int *end = (int *)malloc(n * sizeof(int));
    int *selected = (int *)malloc(n * sizeof(int));
    cout << "Write the start and end angles of the " << n << " Objects" << endl;

    for (int i = 0; i < n; i++)
    {
        cout << "start=";
        cin >> start[i];
        cout << "end=";
        cin >> end[i];
        selected[i] = 0; // i.e. not selected
    }
    greedy(n, start, end, selected);

    cout << "Objects Selected:" << endl;
    for (int i = 0; i < n; i++)
    {
        if (selected[i] != 1)
        {
            continue;
        }
        cout << "start=";
        cout << start[i];
        cout << " end=";
        cout << end[i] << endl;
    }
    return 0;
}

/*

reference test cases and edge cases

Write the number of objects: 5
Write the start and end angles of the 5 Objects
start=350
end=30
start=40
end=50
start=20
end=80
start=70
end=100
start=90
end=110
Objects Selected:
start=40 end=50
start=70 end=100
start=350 end=30

>> counter eg for earlist deadline first without handeling the edge case

Write the number of objects: 4
Write the start and end angles of the 4 Objects
start=30
end=20
start=40
end=50
start=60
end=80
start=85
end=95
Objects Selected:
start=40 end=50
start=60 end=80
start=85 end=95

wrong output will be 
start=30 end=20

>> test case for edge case when the optimal is the one we find in other loop

Write the number of objects: 5
Write the start and end angles of the 5 Objects
start=10
end=30
start=20
end=50
start=40
end=350
start=60
end=300
start=310
end=15
Objects Selected:
start=20 end=50
start=60 end=300
start=310 end=15

wrong output will be without handeling the back end overlap with first choice is

start=10 end=30
start=60 end=300
start=310 end=15



*/