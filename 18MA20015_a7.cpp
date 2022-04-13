/*
Name : Harshal Dupare
Roint Number : 18MA20015
Assignment Number : 7
*/

#include <iostream>
#include <stdio.h>

using namespace std;

struct node;

struct lst
{
    node* ts;
    lst* nxt;
    
    lst(node* _n)
    {
        this->ts=_n;
        this->nxt = nullptr;
    }

    void add_n(node* _n)
    {
        if(this->ts==nullptr)
        {
            this->ts == _n;
            this->nxt = nullptr;
            return;
        }
        lst* lt = this;
        while(lt->nxt!=nullptr)
        {
            lt = lt->nxt;
        }
        lt->nxt = new lst(_n);
    }
};

struct node
{
    int z;
    int x;
    int y;
    node *lc;
    node *rc;
    node *parent;
    lst * allsamez;


    node(int _x, int _y, int _z)
    {
        this->x = _x;
        this->y = _y;
        this->z = _z;
        this->lc = nullptr;
        this->rc = nullptr;
        this->parent = nullptr;
        this->allsamez = new lst(this);
    }

    bool operator<(const node &on)
    {
        return this->z < on.z;
    }
};

void traverse_print(node *r)
{
    if (r == nullptr)
    {
        return;
    }
    cout << "(" << r->x << "," << r->y << "," << r->z << "), ";
    traverse_print(r->lc);
    traverse_print(r->rc);
}

struct min_heap
{
    node *root;
    node *last;
    min_heap()
    {
        this->root = nullptr;
    }

    bool is_empty()
    {
        return this->root == nullptr;
    }
};

void shift_up(min_heap *h, node *node_to_move)
{
    if (node_to_move == nullptr)
    {
        return;
    }

    node *par = node_to_move->parent;
    if (par->parent == nullptr)
    {
        h->root = node_to_move;
    }
    else
    {
        if (par == par->parent->rc)
        {
            par->parent->rc = node_to_move;
        }
        else
        {
            par->parent->lc = node_to_move;
        }
    }

    node_to_move->parent = par->parent;

    bool l_or_r = (node_to_move == par->rc);

    node *c = nullptr;
    if (!l_or_r)
    {
        c = par->rc;
    }
    else
    {
        c = par->lc;
    }

    par->lc = node_to_move->lc;
    if (par->lc != nullptr)
    {
        par->lc->parent = par;
    }

    par->rc = node_to_move->rc;
    if (par->rc)
    {
        par->rc->parent = par;
    }

    if (l_or_r)
    {
        node_to_move->rc = par;
        par->parent = node_to_move;
        node_to_move->lc = c;
    }
    else
    {
        node_to_move->lc = par;
        par->parent = node_to_move;
        node_to_move->rc = c;
    }

    if (c != nullptr)
    {
        c->parent = node_to_move;
    }

    if (node_to_move == h->last)
    {
        h->last = par;
    }
}

void replace_with(min_heap *h, node *n1, node *n2)
{
    if (n1->parent != nullptr)
    {
        if (n1 == n1->parent->rc)
        {
            n1->parent->rc = n2;
        }
        else
        {
            n1->parent->lc = n2;
        }
    }
    else
    {
        h->root = n2;
    }
    n2->parent = n1->parent;

    n2->lc = n1->lc;
    if (n2->lc != nullptr)
    {
        n2->lc->parent = n2;
    }

    n2->rc = n1->rc;
    if (n2->rc != nullptr)
    {
        n2->rc->parent = n2;
    }
}

void insert(min_heap *h, node *new_node)
{
    if (h->root == nullptr)
    {
        h->root = new_node;
        h->last = h->root;
        return;
    }

    node *curr = h->last;
    while ((curr->parent != nullptr) && (curr == curr->parent->rc))
    {
        curr = curr->parent;
    }

    if (curr->parent != nullptr)
    {
        if (curr->parent->rc != nullptr)
        {
            curr = curr->parent->rc;
            while (curr->lc != nullptr)
            {
                curr = curr->lc;
            }
        }
        else
        {
            curr = curr->parent;
        }
    }
    else
    {
        while (curr->lc != nullptr)
        {
            curr = curr->lc;
        }
    }

    h->last = new_node;
    if (curr->lc != nullptr)
    {
        curr->rc = new_node;
    }
    else
    {
        curr->lc = new_node;
    }

    new_node->parent = curr;
    new_node->lc = nullptr;
    new_node->rc = nullptr;

    while ((new_node->parent != nullptr))
    {
        if(new_node->parent->z < new_node->z)
        {   
            break;
        }
        else if(new_node->parent->z < new_node->z)
        {
            new_node->parent->allsamez->add_n(new_node);
            new_node->allsamez->add_n(new_node->parent);
        }
        shift_up(h, new_node);
    }
}

void _remove(min_heap *h, node *node_to_remove)
{
    if (node_to_remove->parent == nullptr && node_to_remove->lc == nullptr && node_to_remove->rc == nullptr)
    {
        h->root = nullptr;
        h->last = nullptr;
        return;
    }

    node *curr = h->last;
    while (curr->parent != nullptr && curr == curr->parent->lc)
    {
        curr = curr->parent;
    }
    if (curr->parent != nullptr)
    {
        curr = curr->parent->lc;
    }
    while (curr->rc != nullptr)
    {
        curr = curr->rc;
    }

    if (h->last == h->last->parent->rc)
    {
        h->last->parent->rc = nullptr;
    }
    else
    {
        h->last->parent->lc = nullptr;
    }

    if (node_to_remove == h->last)
    {
        h->last = curr;
    }
    else
    {
        node *nood_l = h->last;
        replace_with(h, node_to_remove, nood_l);
        if (node_to_remove != curr)
        {
            h->last = curr;
        }

        if (nood_l->parent != nullptr && nood_l->z < nood_l->parent->z)
        {
            do
            {
                shift_up(h, nood_l);
            } while((nood_l->parent != nullptr) && (nood_l->z < nood_l->parent->z));
        }
        else
        {
            while (nood_l->lc != nullptr || nood_l->rc != nullptr)
            {
                bool l_or_r = (nood_l->rc != nullptr) && (nood_l->rc->z <= nood_l->lc->z);
                int n_z;
                if (l_or_r)
                {
                    nood_l->rc->z;
                }
                else
                {
                    nood_l->lc->z;
                }

                if (nood_l->z > n_z)
                {
                    if (l_or_r)
                    {
                        shift_up(h, nood_l->rc);
                    }
                    else
                    {
                        shift_up(h, nood_l->lc);
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
}

node *get_min(node *r)
{
    if (r == nullptr)
    {
        return nullptr;
    }
    node *tmin = r;
    node *lmin = get_min(r->lc);
    node *rmin = get_min(r->rc);
    if (lmin != nullptr)
    {
        if (lmin->z < tmin->z)
        {
            tmin = lmin;
        }
    }
    if (rmin != nullptr)
    {
        if (rmin->z < tmin->z)
        {
            tmin = rmin;
        }
    }
    return tmin;
}

void get_all_z(int z,node* h,lst* ll)
{
    if (h == nullptr)
    {
        return nullptr;
    }
    get_all_z(r->lc,ll);
    get_all_z(r->rc,ll);
    if (h->z == z)
    {
        ll->add_n(h);
    }
}

node *remove_min(min_heap *h)
{
    node *mn = get_min(h->root);
    if (mn != nullptr)
        _remove(h, mn);
    return mn;
}

min_heap *build_heap()
{
    min_heap *pp = new min_heap();
    return pp;
}

void print_all_combo(lst* nl,lst* ll,int& ctt,min_heap* H1, min_heap* H2)
{
    lst *th = nl;
    while(th!=nullptr)
    {
        lst * lh = ll;
        while(lh!=nullptr)
        {
            cout<<th->ts->x<<" "<<th->ts->y<<" "<<lh->ts->x<<" "<<lh->ts->y<<endl;
            ctt++;
            lh=lh->nxt;
        }
        th=th->nxt;
    }

    lst * lh = nl;
    while(lh!=nullptr)
    {
        _remove(H1,lh->ts);
        lh = lh->nxt;
    }

    lst * th = ll;
    while(lh!=nullptr)
    {
        _remove(H2,th->ts);
        th = th->nxt;
    }
}

int main()
{
    int N;
    cout << "n = ";
    cin >> N;
    int ct = 0;
    // populate the nodes
    min_heap *H1 = build_heap();
    min_heap *H2 = build_heap();
    for (int i = 1; i <= N; i++)
    {
        node *nn1 = new node(i, 1, i + 1);
        insert(H1, nn1);

        node *nn2 = new node(i, 1, i * i * i + 1);
        insert(H2, nn2);
    }

    // traverse_print(H1->root);
    // cout << endl;
    // traverse_print(H2->root);
    // cout << endl;

    while ((!H1->is_empty()) && (!H2->is_empty()))
    {
        node *h1min = get_min(H1->root);
        node *h2min = get_min(H2->root);
        int a, b, c, d;
        a = h1min->x;
        b = h1min->y;
        c = h2min->x;
        d = h2min->y;
        // cout<<"\nProcessing "<<h1min->z<<" :: "<<h2min->z<<endl;
        if (h1min->z < h2min->z)
        {

            node *tmp = remove_min(H1);
            // cout << "After removing: ";
            // traverse_print(H1->root);
            // cout << endl;
            if (b < N)
            {
                node *nni = new node(a, b + 1, a + (b + 1) * (b + 1));
                insert(H1, nni);
            }
        }
        else if (h1min->z > h2min->z)
        {
            node *tmp = remove_min(H2);
            // cout << "After removing: ";
            // traverse_print(H2->root);
            // cout << endl;
            if (d < N)
            {
                node *nni = new node(c, d + 1, c * c * c + (d + 1) * (d + 1) * (d + 1) * (d + 1));
                insert(H2, nni);
            }
        }
        else
        {
            lst * l1 = new lst();
            lst * l2 = new lst();
            get_all_z(h1min->z,h1min,l1);
            get_all_z(h2min->z,h2min,l2);

            print_all_combo(l1,l2,H1,H2);
            node *tmp = remove_min(H1);
            // cout << " == After removing: ";
            // traverse_print(H1->root);
            // cout << endl;
            if (b < N)
            {
                node *nni = new node(a, b + 1, a + (b + 1) * (b + 1));
                insert(H1, nni);
            }
            tmp = remove_min(H2);
            // cout << " == After removing: ";
            // traverse_print(H2->root);
            // cout << endl;
            if (d < N)
            {
                node *nni = new node(c, d + 1, c * c * c + (d + 1) * (d + 1) * (d + 1) * (d + 1));
                insert(H2, nni);
            }
        }
        // cout << "++ Adding : ";
        // traverse_print(H1->root);
        // cout << endl;
        // cout << "++ Adding : ";
        // traverse_print(H2->root);
        // cout << endl;
    }

    cout << "Total number of solutions: " << ct << endl;
    return 0;
}