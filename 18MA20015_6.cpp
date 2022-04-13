/*
Name : Harshal Dupare
Roint Number : 18MA20015
Assignment Number :
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

    void preorder(node *in_root)
    {
        if (in_root == nullptr)
        {
            return;
        }
        cout << in_root->data << ", ";
        this->preorder(in_root->left_child);
        this->preorder(in_root->right_child);
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

            in_root->height = 1+max(height(in_root->left_child),height(in_root->right_child));
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
        else if(in_root->dir == 1)
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
        else if(in_root->dir == 1)
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

// T(n) = 2*T(n/2)+O(1) = O(n)
node *make_from_array(int *a, int i, int j)
{
    if (i > j)
    {
        return nullptr;
    }
    else if (i == j)
    {
        node *p = new node(a[i]);
        return p;
    }
    // j>i
    int mid = (i + j) / 2;
    node *left_tree = make_from_array(a, i, mid - 1);
    node *right_tree = make_from_array(a, mid + 1, j);
    node *center = new node(a[mid]);

    center->left_child = left_tree;
    if (left_tree != nullptr)
    {
        left_tree->parent = center;
        left_tree->dir = -1;
    }
    center->right_child = right_tree;
    if (right_tree != nullptr)
    {
        right_tree->parent = center;
        right_tree->dir = 1;
    }

    return center;
}

int main()
{
    cout << "1. insert\n2. delete\n3. make tree balanced (AVL tree based method).\n4. make tree balanced faster\n5. exit\n";
    cout << endl;

    int c = 0;
    tree t;
    while (c != 5)
    {
        cin >> c;
        if (c == 1)
        {
            int key;
            cout << "Write key to be inserted: ";
            cin >> key;
            t.insert(key, t.root);
            cout << "Pre-order traversal: ";
            t.preorder(t.root);
            cout << endl;
        }
        else if (c == 2)
        {
            int key;
            cout << "Write key to be deleted: ";
            cin >> key;
            t.delete_n(key, t.root);
            cout << "Pre-order traversal: ";
            t.preorder(t.root);
            cout << endl;
        }
        else if (c == 3)
        {
            // only this part may have some problem with working
            cout << "Pre-order traversal of tree after balancing with AVL tree based method:";
            tree nt;
            while (t.root != nullptr)
            {
                int key = t.root->data;
                nt.insert_avl(key, nt.root);
                t.delete_n(key, t.root);
            }
            t = nt;
            t.preorder(t.root);
            cout << endl;
        }
        else if (c == 4)
        {
            // this is exactly as what was asked in the question
            cout << "Pre-order traversal of tree after balancing with faster method: ";
            int i = 0;
            int *a = (int *)malloc((t.size + 2) * sizeof(int));
            t.inorder(t.root, a, i);
            t.root = make_from_array(a, 0, t.size - 1);
            t.preorder(t.root);
            cout << endl;
        }
        else if (c == 5)
        {
            cout << "Program exists" << endl;
            break;
        }
    }

    return 0;
}