#include <iostream>
#include <string>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

#define RED 0
#define BLACK 1

class RBTree{
public:
    struct Node{
        int key;
        string value;
        Node* left;
        Node* right;
        Node* parent;
        int color;

        Node(int k, string v){
            key = k;
            value = v;
            left = NULL;
            right = NULL;
            parent = NULL;
            color = RED;
        }
    };
    Node* root;
    Node* nil;

    Node* rotateLeft(Node* x){
        Node* y = x->right;
        x->right = y->left;
        if(y->left != nil){
            y->left->parent = x;
        }
        y->parent = x->parent;
        if(x->parent == nil){
            root = y;
        }
        else if(x == x->parent->left){
            x->parent->left = y;
        }
        else{
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
        return y;
    }

    Node* rotateRight(Node* x){
        Node* y = x->left;
        x->left = y->right;
        if(y->right != nil){
            y->right->parent = x;
        }
        y->parent = x->parent;
        if(x->parent == nil){
            root = y;
        }
        else if(x == x->parent->right){
            x->parent->right = y;
        }
        else{
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
        return y;
    }

    RBTree(){
        nil = new Node(0, "");
        nil->color = BLACK;
        root = nil;
    }

    Node* insert(int k, const string& v){
        Node* z = new Node(k, v);
        Node* y = nil;
        Node* x = root;
        while(x != nil){
            y = x;
            if(z->key < x->key){
                x = x->left;
            }
            else{
                x = x->right;
            }
        }
        z->parent = y;
        if(y == nil){
            root = z;
        }
        else if(z->key < y->key){
            y->left = z;
        }
        else{
            y->right = z;
        }
        z->left = nil;
        z->right = nil;
        z->color = RED;
        insertFixup(z);
        return z;
    }

    void insertFixup(Node* z){
        while(z->parent->color == RED){
            if(z->parent == z->parent->parent->left){
                Node* y = z->parent->parent->right;
                if(y->color == RED){
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else{
                    if(z == z->parent->right){
                        z = z->parent;
                        rotateLeft(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateRight(z->parent->parent);
                }
            }
            else{
                Node* y = z->parent->parent->left;
                if(y->color == RED){
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else{
                    if(z == z->parent->left){
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void transplant(Node* u, Node* v){
        if(u->parent == nil){
            root = v;
        }
        else if(u == u->parent->left){
            u->parent->left = v;
        }
        else{
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    Node* minimum(Node* x){
        while(x->left != nil){
            x = x->left;
        }
        return x;
    }

    void erase(Node* z){
        Node* y = z;
        Node* x;
        int y_original_color = y->color;
        if(z->left == nil){
            x = z->right;
            transplant(z, z->right);
        }
        else if(z->right == nil){
            x = z->left;
            transplant(z, z->left);
        }
        else{
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if(y->parent == z){
                x->parent = y;
            }
            else{
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        if(y_original_color == BLACK){
            eraseFixup(x);
        }
    }

    void eraseFixup(Node* x){
        while(x != root && x->color == BLACK){
            if(x == x->parent->left){
                Node* w = x->parent->right;
                if(w->color == RED){
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }
                if(w->left->color == BLACK && w->right->color == BLACK){
                    w->color = RED;
                    x = x->parent;
                }
                else{
                    if(w->right->color == BLACK){
                        w->left->color = BLACK;
                        w->color = RED;
                        rotateRight(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    rotateLeft(x->parent);
                    x = root;
                }
            }
            else{
                Node* w = x->parent->left;
                if(w->color == RED){
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }
                if(w->right->color == BLACK && w->left->color == BLACK){
                    w->color = RED;
                    x = x->parent;
                }
                else{
                    if(w->left->color == BLACK){
                        w->right->color = BLACK;
                        w->color = RED;
                        rotateLeft(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    Node* find(int k){
        Node* x = root;
        while(x != nil){
            if(k == x->key){
                return x;
            }
            else if(k < x->key){
                x = x->left;
            }
            else{
                x = x->right;
            }
        }
        return nil;
    }

    void clear(){
        while(root != nil){
            erase(root);
        }
    }

    bool empty(){
        if(root == nil){
            return true;
        }
        return false;
    }

    int size(){
        int count = 0;
        Node* x = root;
        while(x != nil){
            count++;
            x = x->right;
        }
        return count;
    }

    void print(Node* root){
        if(root == nil){
            return;
        }
        cout << root->key << "_" << root->value;
        if(root->left != nil || root->right != nil){
            cout << "(";
            print(root->left);
            cout << ",";
            print(root->right);
            cout << ")";
        }
    }

    void iterate(Node* root) {
        if (root != nil) {
            iterate(root->left);
            cout << root->key << " => " << root->value << endl;
            iterate(root->right);
        }
    }

    ~RBTree(){
        clear();
        delete nil;
    }
};

class Map{
public:
    RBTree tree;

    void insert(int key, const string& value) {
        RBTree::Node* existingNode = tree.find(key);

        if (existingNode != tree.nil) {
            existingNode->value = value;
        } else {
            RBTree::Node* newNode = tree.insert(key, value);
        }

        tree.print(tree.root);
        cout << endl;
    }

    void erase(int k){
        RBTree::Node* z = tree.find(k);
        if(z == tree.nil){
            cout << k << " not found";
        }
        else{
            tree.erase(z);
            delete z;
            tree.print(tree.root);
        }
        cout << endl;
    }

    void clear(){
        if(tree.empty()){
            cout << "unsuccessful" << endl;
            return;
        }
        else{
            tree.clear();
            cout << "successful" << endl;
        }
    }

    void find(int k){
        RBTree::Node* z = tree.find(k);
        if(z == tree.nil){
            cout << k << " not found" << endl;
        }
        else{
            cout << k << " found" << endl;
        }
    }

    void empty(){
        if(tree.empty()){
            cout << "yes" << endl;
        }
        else{
            cout << "no" << endl;
        }
    }

    void size(){
        cout << tree.size() << endl;
    }

    void iterate(){
        tree.iterate(tree.root);
    }

    Map(){
        tree = RBTree();
    }
};

int main(){
    Map map;
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    string cmd;
    int cmdCnt;

    while(cin >> cmd){
        if(cmd == "I"){
            cmdCnt++;
            int k;
            string v;
            cin >> k >> v;
            cout << "Cmd " << cmdCnt << ": " << cmd << " " << k << " " << v << endl;
            map.insert(k, v);
            cout << endl;
        }
        else if(cmd == "E"){
            cmdCnt++;
            int k;
            cin >> k;
            cout << "Cmd " << cmdCnt << ": " << cmd << " " << k << endl;
            map.erase(k);
            cout << endl;
        }
        else if(cmd == "Clr"){
            cmdCnt++;
            cout << "Cmd " << cmdCnt << ": " << cmd << endl;
            map.clear();
            cout << endl;
        }
        else if(cmd == "F"){
            cmdCnt++;
            int k;
            cin >> k;
            cout << "Cmd " << cmdCnt << ": " << cmd << " " << k << endl;
            map.find(k);
            cout << endl;
        }
        else if(cmd == "Em"){
            cmdCnt++;
            cout << "Cmd " << cmdCnt << ": " << cmd << endl;
            map.empty();
            cout << endl;
        }
        else if(cmd == "S"){
            cmdCnt++;
            cout << "Cmd " << cmdCnt << ": " << cmd << endl;
            map.size();
            cout << endl;
        }
        else if(cmd == "Itr"){
            cmdCnt++;
            cout << "Cmd " << cmdCnt << ": " << cmd << endl;
            map.iterate();
            cout << endl;
        }
    }

    return 0;
}
