/*
Name : Harshal Dupare
Roint Number : 18MA20015
Assignment Number : 8
*/

#include <iostream>
#include <stdio.h>

using namespace std;

struct node
{
    int data;
    node *left_child;
    node *right_child;
    node *parent;
    int dir;
    int height;

    node(int _data)
    {
        this->data = _data;
        this->dir = 0; // -1 for left and 1 for right and 0 for root
        this->left_child = nullptr;
        this->right_child = nullptr;
        this->parent = nullptr;
        this->height = 1;
    }
};

int height(node *n)
{
    if (n == nullptr)
        return 0;

    return n->height;
}

int get_balance(node *n)
{
    if (n == nullptr)
        return 0;
    return height(n->left_child) - height(n->right_child);
}

struct tree
{
    node *root;
    int size = 0;

    tree()
    {
        this->root = nullptr;
    }

    tree(node *n)
    {
        this->root = n;
        this->size = 1;
    }

    void insert(int key, node *in_root)
    {
        if (this->root == nullptr)
        {
            this->root = new node(key);
            this->size++;
            this->root->parent = this->root;
            this->root->dir = 0;
            return;
        }

        if (in_root->data < key)
        {
            if (in_root->right_child != nullptr)
            {
                this->insert(key, in_root->right_child);
            }
            else
            {
                in_root->right_child = new node(key);
                in_root->right_child->parent = in_root;
                in_root->right_child->dir = 1;
                this->size++;
            }
        }
        else
        {
            // assuming all keys are distinct in_root->data > key
            if (in_root->left_child != nullptr)
            {
                this->insert(key, in_root->left_child);
            }
            else
            {
                in_root->left_child = new node(key);
                in_root->left_child->parent = in_root;
                in_root->left_child->dir = -1;
                this->size++;
            }
        }

        in_root->height = max(height(in_root->right_child), height(in_root->left_child)) + 1;

        return;
    }

    void inorder(node *in_root)
    {
        if (in_root == nullptr)
        {
            return;
        }
        this->inorder(in_root->left_child);
        cout << in_root->data << ", ";
        this->inorder(in_root->right_child);
        return;
    }

    void postorder(node *in_root)
    {
        if (in_root == nullptr)
        {
            return;
        }
        this->postorder(in_root->right_child);
        cout << in_root->data << ", ";
        this->postorder(in_root->left_child);
        return;
    }

    void inorder(node *in_root, int *a, int &i)
    {
        if (in_root == nullptr)
        {
            return;
        }
        this->inorder(in_root->left_child, a, i);
        a[i] = in_root->data;
        i++;
        this->inorder(in_root->right_child, a, i);
        return;
    }

    void delete_n(int key, node *in_root)
    {
        if (in_root == nullptr)
        {
            return;
        }
        if (in_root->data != key)
        {
            if (in_root->data > key)
            {
                this->delete_n(key, in_root->left_child);
            }
            else
            {
                this->delete_n(key, in_root->right_child);
            }

            in_root->height = 1 + max(height(in_root->left_child), height(in_root->right_child));
        }
        else
        {
            if (in_root->right_child == nullptr)
            {
                if (in_root->left_child == nullptr)
                {
                    // if no child then just remove
                    if (in_root->dir == -1)
                    {
                        in_root->parent->left_child = nullptr;
                    }
                    else if (in_root->dir == 1)
                    {
                        in_root->parent->right_child = nullptr;
                    }
                    else
                    {
                        // this is root
                        this->root = nullptr;
                    }
                }
                else
                {
                    if (in_root->dir == -1)
                    {
                        in_root->parent->left_child = in_root->left_child;
                    }
                    else if (in_root->dir == 1)
                    {
                        in_root->parent->right_child = in_root->left_child;
                    }
                    else
                    {
                        // this is root
                        this->root = in_root->left_child;
                        this->root->dir = 0;
                        this->root->parent = this->root;
                    }
                }
            }
            else
            {
                if (in_root->left_child == nullptr)
                {
                    if (in_root->dir == -1)
                    {
                        in_root->parent->left_child = in_root->right_child;
                    }
                    else if (in_root->dir == 1)
                    {
                        in_root->parent->right_child = in_root->right_child;
                    }
                    else
                    {
                        // this is root
                        this->root = in_root->right_child;
                        this->root->dir = 0;
                        this->root->parent = this->root;
                    }
                }
                else
                {
                    node *inorder_pred = in_root->left_child;
                    while (inorder_pred->right_child != nullptr)
                    {
                        inorder_pred = inorder_pred->right_child;
                    }

                    in_root->data = inorder_pred->data;
                    // since the inorder_pred doesnt have a right child it implies we can simply remove if
                    // it also doesnt have left child
                    // and if it does have we just assign that left child to its parent
                    // i.e. the case where node as only left child and a right child

                    if (inorder_pred->dir == -1)
                    {
                        inorder_pred->parent->left_child = inorder_pred->left_child;
                    }
                    else if (inorder_pred->dir == 1)
                    {
                        inorder_pred->parent->right_child = inorder_pred->left_child;
                    }
                    else
                    {
                        cout << "problem" << endl;
                        // this condition should be never reached
                    }
                }
            }
            this->size--;
        }
    }

    void lrotate(node *in_root)
    {
        node *rc = in_root->right_child;
        node *rclc = rc->left_child;
        rc->left_child = in_root;
        rc->dir = in_root->dir;
        if (in_root->dir == 0)
        {
            rc->parent = rc;
        }
        else if (in_root->dir == 1)
        {
            rc->parent = in_root->parent;
            in_root->parent->right_child = rc;
        }
        else
        {
            rc->parent = in_root->parent;
            in_root->parent->left_child = rc;
        }
        rclc->parent = in_root;
        in_root->right_child = rclc;
        in_root->parent = rc;
        rclc->dir = 1;

        rc->height = 1 + max(height(rc->left_child), height(rc->right_child));
        in_root->height = 1 + max(height(in_root->left_child), height(in_root->right_child));
    }

    void rrotate(node *in_root)
    {
        node *lc = in_root->left_child;
        node *lcrc = lc->right_child;
        lc->right_child = in_root;
        lc->dir = in_root->dir;
        if (in_root->dir == 0)
        {
            lc->parent = lc;
        }
        else if (in_root->dir == 1)
        {
            lc->parent = in_root->parent;
            in_root->parent->right_child = lc;
        }
        else
        {
            lc->parent = in_root->parent;
            in_root->parent->left_child = lc;
        }
        lcrc->parent = in_root;
        in_root->left_child = lcrc;
        in_root->parent = lc;
        lcrc->dir = -1;

        lc->height = 1 + max(height(lc->left_child), height(lc->right_child));
        in_root->height = 1 + max(height(in_root->left_child), height(in_root->right_child));
    }

    void insert_avl(int key, node *in_root)
    {
        if (this->root == nullptr)
        {
            this->root = new node(key);
            this->size++;
            this->root->parent = this->root;
            this->root->dir = 0;
            return;
        }

        if (in_root->data < key)
        {
            if (in_root->right_child != nullptr)
            {
                this->insert(key, in_root->right_child);
            }
            else
            {
                in_root->right_child = new node(key);
                in_root->right_child->parent = in_root;
                in_root->right_child->dir = 1;
                this->size++;
            }
        }
        else
        {
            // assuming all keys are distinct in_root->data > key
            if (in_root->left_child != nullptr)
            {
                this->insert(key, in_root->left_child);
            }
            else
            {
                in_root->left_child = new node(key);
                in_root->left_child->parent = in_root;
                in_root->left_child->dir = -1;
                this->size++;
            }
        }

        in_root->height = max(height(in_root->right_child), height(in_root->left_child)) + 1;

        int bal = get_balance(in_root);

        if (bal > 1 && key < in_root->left_child->data)
        {
            this->rrotate(in_root);
        }

        if (bal < -1 && key > in_root->right_child->data)
        {
            this->lrotate(in_root);
        }

        if (bal > 1 && key > in_root->left_child->data)
        {
            this->lrotate(in_root->left_child);
            this->rrotate(in_root);
        }

        if (bal < -1 && key < in_root->right_child->data)
        {
            this->rrotate(in_root->right_child);
            this->lrotate(in_root);
        }
    }
};

struct min_heap
{
    int * a;
    int N = 1<<8; // max size of heap
    int n;
    min_heap()
    {
        this->n = 0;
        this->a = (int *)malloc(this->N * sizeof(int));
    }

    void insert(int key)
    {
        if(this->n + 1 > this->N)
        {
            cout<<"Capacity overflow"<<endl;
            return;
        }
        
        this->a[this->n] = key;
        this->n++;
        int pi = this->n;
        pi/=2;
        while(pi>0)
        {
            int lc=pi*2;
            int rc=pi*2+1;
            if(rc>this->n)
            {
                // swap as parent must be smaller than child in min-heap
                if(a[rc-1] < a[pi-1])
                {
                    int tp = a[rc-1];
                    a[rc-1] = a[pi-1];
                    a[pi-1] = tp;
                }
            }
            else
            {
                // both are present..
                // we need to find the smaller one among both child and compare it with parent
                int id=rc;
                if(a[rc-1] > a[lc-1]) id = lc;
                if(a[id-1] < a[pi-1])
                {
                    int tp = a[id-1];
                    a[id-1] = a[pi-1];
                    a[pi-1] = tp;
                }
                
            }
            pi/=2;
        }

    }

    int extract_min()
    {
        // swap with last element
        int tp1 = a[0];
        // cout<<"tp1"<<tp1<<endl;
        a[0] = a[this->n-1];
        a[this->n-1] = tp1;
        // decrease size
        this->n--;

        int i = 1;

        while(i<=this->n)
        {
            // cout<<i<<"::"<<endl;
            int rc = 2*i+1;
            int lc = 2*i;
            if(rc>this->n)
            {
                if(lc>this->n)
                {
                    // skip
                    break;
                }
                else
                {
                    // lc is only there
                    if(a[lc-1]<a[i-1])
                    {
                        int swp1=i-1;
                        int swp2=lc-1;
                        int tp = a[swp1];
                        a[swp1] = a[swp2];
                        a[swp2] = tp;
                        i = lc;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                if(lc>this->n)
                {
                    // case never happens
                    cout<<"Problem"<<endl;
                }
                else
                {
                    // both child there
                    int id = rc;
                    if(a[lc-1]<a[rc-1]) id = lc;
                    if(a[id-1]<a[i-1])
                    {
                        // swap id and parent
                        int swp1=i-1;
                        int swp2=id-1;
                        int tp = a[swp1];
                        a[swp1] = a[swp2];
                        a[swp2] = tp;
                        i = id;
                    }
                    else
                    {
                        // as parent is smallest 
                        break;
                    }
                }
            }
        }

        // cout<<"tp1"<<tp1<<endl;
        return tp1;
    }
};

struct max_heap
{
    int * a;
    int N = 1<<8; // max size of heap
    int n;
    max_heap()
    {
        this->n = 0;
        this->a = (int *)malloc(this->N * sizeof(int));
    }

    void insert(int key)
    {
        if(this->n + 1 > this->N)
        {
            cout<<"Capacity overflow"<<endl;
            return;
        }
        
        this->a[this->n] = key;
        this->n++;
        int pi = this->n;
        pi/=2;
        while(pi>0)
        {
            int lc=pi*2;
            int rc=pi*2+1;
            if(rc>this->n)
            {
                // swap as parent must be smaller than child in min-heap
                if(a[rc-1] > a[pi-1])
                {
                    int tp = a[rc-1];
                    a[rc-1] = a[pi-1];
                    a[pi-1] = tp;
                }
            }
            else
            {
                // both are present..
                // we need to find the smaller one among both child and compare it with parent
                int id=rc;
                if(a[rc-1] < a[lc-1]) id = lc;
                if(a[id-1] > a[pi-1])
                {
                    int tp = a[id-1];
                    a[id-1] = a[pi-1];
                    a[pi-1] = tp;
                }
                
            }
            pi/=2;
        }

    }

    int extract_max()
    {
        // swap with last element
        int tp1 = a[0];
        a[0] = a[this->n-1];
        a[this->n-1] = tp1;
        
        // decrease size
        this->n--;

        int i = 1;

        while(i<=this->n)
        {
            int rc = 2*i+1;
            int lc = 2*i;
            if(rc>this->n)
            {
                if(lc>this->n)
                {
                    // skip
                    break;
                }
                else
                {
                    // lc is only there
                    if(a[lc-1]>a[i-1])
                    {
                        int swp1=i-1;
                        int swp2=lc-1;
                        int tp = a[swp1];
                        a[swp1] = a[swp2];
                        a[swp2] = tp;
                        i = lc;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                if(lc>this->n)
                {
                    // case never happens
                    cout<<"Problem"<<endl;
                }
                else
                {
                    // both child there
                    int id = rc;
                    if(a[lc-1]>a[rc-1]) id = lc;
                    if(a[id-1]>a[i-1])
                    {
                        // swap id and parent
                        int swp1=i-1;
                        int swp2=id-1;
                        int tp = a[swp1];
                        a[swp1] = a[swp2];
                        a[swp2] = tp;
                        i = id;
                    }
                    else
                    {
                        // as parent is smallest 
                        break;
                    }
                }
            }
        }

        return tp1;
    }
};

struct main_heap
{
    min_heap mnhp;
    max_heap mxhp;
    int n;
    main_heap()
    {
        this->n = 0;
    }

    int get_median()
    {
        // median is always the top of left heap
        return this->mxhp.a[0];
    }
    
    void insert(int key)
    {
        if(this->n==0)
        {
            this->mxhp.insert(key);
            this->n++;
            return;
        }

        if(key < this->mxhp.a[0])
        {
            this->mxhp.insert(key);
        }
        else
        {
            // cout<<"Inserting in right"<<endl;
            this->mnhp.insert(key);
            // this->log();
        }

        if(this->mxhp.n - this->mnhp.n > 1)
        {
            // cout<<"S L -> R:: "<<this->mxhp.a[0]<<endl;
            this->mnhp.insert(this->mxhp.a[0]);
            int lmx = this->mxhp.extract_max();
        }
        
        if(this->mxhp.n < this->mnhp.n )
        {
            // cout<<"S R -> L:: "<<this->mnhp.a[0]<<endl;
            this->mxhp.insert(this->mnhp.a[0]);
            int oklmn = this->mnhp.extract_min();
        }
        this->n++;
    }

    void log()
    {
        cout<<"Main heap"<<endl;
        cout<<this->n<<endl;
        cout<<this->mxhp.n<<","<<this->mnhp.n<<endl;
        cout<<"Left:";for(int i=0;i<this->mxhp.n;i++)cout<<this->mxhp.a[i]<<"; ";
        cout<<endl;
        cout<<"Right:";for(int i=0;i<this->mnhp.n;i++)cout<<this->mnhp.a[i]<<"; ";
        cout<<endl;
    }
};

int main()
{
    /*
    Idea is to store 2 heaps one as max and other as min, left i.e. max will have all the values smaller including median
    and right will have all the values greater than median and will be of type min heap
    */
    cout << "1. Insert\n2. Find max\n3. Find min\n4. Find median\n5. Print in non-decreasing order\n6. Exit\n";
    cout << endl;

    int c = 0;
    int mx = -(int)1e8; // assuming min is -10000000
    int mn = (int)1e8;  // assuming max is  10000000
    int med = -1;
    main_heap mh;
    // use avl tree to maintain the list for printing in O(n) and inserting in  O(log(n))
    // and use min-heap and max-heap for getting median in O(1) and inserting in it in O(log(n))
    tree t;
    while (c != 6)
    {
        cin >> c;
        if (c == 1)
        {
            int key;
            cout << "Write element to be inserted: ";
            cin >> key;
            if (key < mn)
            {
                mn = key;
            }
            if (key > mx)
            {
                mx = key;
            }
            t.insert(key, t.root);
            mh.insert(key);
            cout << "List in non-increasing order: (";
            t.postorder(t.root);
            cout << ") " << endl;
        }
        else if (c == 2)
        {
            cout << "Maximum: " << mx << endl;
        }
        else if (c == 3)
        {
            cout << "Minimum: " << mn << endl;
        }
        else if (c == 4)
        {
            // there is some bug in it
            med = mh.get_median();
            cout << "Median: " << med << endl;
        }
        else if (c == 5)
        {
            cout << "List in non-decreasing order: (";
            t.inorder(t.root);
            cout << ") " << endl;
        }
        else if (c == 6)
        {
            cout << "Program exists" << endl;
            break;
        }
        // mh.log();
    }
    return 0;
}