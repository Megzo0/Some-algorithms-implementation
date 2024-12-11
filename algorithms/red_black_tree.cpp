#include <iostream>
using namespace std;

enum Color { RED, BLACK };

struct Node {
    int data;
    bool color;
    Node *left, *right, *parent;

    Node(int data) {
        this->data = data;
        left = right = parent = nullptr;
        this->color = RED;
    }
};

class RedBlackTree {
private:
    Node *root;

    void rotateLeft(Node *&ptr) {
        Node *rightChild = ptr->right;
        ptr->right = rightChild->left;

        if (ptr->right != nullptr)
            ptr->right->parent = ptr;

        rightChild->parent = ptr->parent;

        if (ptr->parent == nullptr)
            root = rightChild;
        else if (ptr == ptr->parent->left)
            ptr->parent->left = rightChild;
        else
            ptr->parent->right = rightChild;

        rightChild->left = ptr;
        ptr->parent = rightChild;
    }

    void rotateRight(Node *&ptr) {
        Node *leftChild = ptr->left;
        ptr->left = leftChild->right;

        if (ptr->left != nullptr)
            ptr->left->parent = ptr;

        leftChild->parent = ptr->parent;

        if (ptr->parent == nullptr)
            root = leftChild;
        else if (ptr == ptr->parent->left)
            ptr->parent->left = leftChild;
        else
            ptr->parent->right = leftChild;

        leftChild->right = ptr;
        ptr->parent = leftChild;
    }

    void fixInsert(Node *&ptr) {
        Node *parent = nullptr;
        Node *grandparent = nullptr;

        while (ptr != root && ptr->color == RED && ptr->parent->color == RED) {
            parent = ptr->parent;
            grandparent = parent->parent;

            if (parent == grandparent->left) {
                Node *uncle = grandparent->right;

                if (uncle != nullptr && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    ptr = grandparent;
                } else {
                    if (ptr == parent->right) {
                        rotateLeft(parent);
                        ptr = parent;
                        parent = ptr->parent;
                    }
                    rotateRight(grandparent);
                    swap(parent->color, grandparent->color);
                    ptr = parent;
                }
            } else {
                Node *uncle = grandparent->left;

                if (uncle != nullptr && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    ptr = grandparent;
                } else {
                    if (ptr == parent->left) {
                        rotateRight(parent);
                        ptr = parent;
                        parent = ptr->parent;
                    }
                    rotateLeft(grandparent);
                    swap(parent->color, grandparent->color);
                    ptr = parent;
                }
            }
        }
        root->color = BLACK;
    }

    void transplant(Node *u, Node *v) {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        if (v != nullptr)
            v->parent = u->parent;
    }

    Node* minimum(Node *node) {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    void fixDelete(Node *x) {
        while (x != root && (x == nullptr || x->color == BLACK)) {
            if (x == x->parent->left) {
                Node *sibling = x->parent->right;

                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    x->parent->color = RED;
                    rotateLeft(x->parent);
                    sibling = x->parent->right;
                }

                if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                    (sibling->right == nullptr || sibling->right->color == BLACK)) {
                    sibling->color = RED;
                    x = x->parent;
                } else {
                    if (sibling->right == nullptr || sibling->right->color == BLACK) {
                        if (sibling->left != nullptr)
                            sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotateRight(sibling);
                        sibling = x->parent->right;
                    }

                    sibling->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (sibling->right != nullptr)
                        sibling->right->color = BLACK;
                    rotateLeft(x->parent);
                    x = root;
                }
            } else {
                Node *sibling = x->parent->left;

                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    x->parent->color = RED;
                    rotateRight(x->parent);
                    sibling = x->parent->left;
                }

                if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                    (sibling->right == nullptr || sibling->right->color == BLACK)) {
                    sibling->color = RED;
                    x = x->parent;
                } else {
                    if (sibling->left == nullptr || sibling->left->color == BLACK) {
                        if (sibling->right != nullptr)
                            sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotateLeft(sibling);
                        sibling = x->parent->left;
                    }

                    sibling->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (sibling->left != nullptr)
                        sibling->left->color = BLACK;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }

        if (x != nullptr)
            x->color = BLACK;
    }

public:
    RedBlackTree() { root = nullptr; }

    void insert(int data) {
        Node *node = new Node(data);
        if (root == nullptr) {
            root = node;
            root->color = BLACK;
        } else {
            Node *temp = root;
            Node *parent = nullptr;

            while (temp != nullptr) {
                parent = temp;
                if (node->data < temp->data)
                    temp = temp->left;
                else
                    temp = temp->right;
            }

            node->parent = parent;

            if (node->data < parent->data)
                parent->left = node;
            else
                parent->right = node;

            fixInsert(node);
        }
    }

    void deleteNode(int data) {
        Node *z = root;
        Node *x, *y;

        while (z != nullptr && z->data != data) {
            if (data < z->data)
                z = z->left;
            else
                z = z->right;
        }

        if (z == nullptr)
            return;

        y = z;
        bool yOriginalColor = y->color;

        if (z->left == nullptr) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nullptr) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;

            if (y->parent == z) {
                if (x != nullptr)
                    x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        if (yOriginalColor == BLACK)
            fixDelete(x);
    }

    void inorder(Node *node) {
        if (node == nullptr)
            return;

        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
    }

    void display() {
        inorder(root);
        cout << endl;
    }
};

// Main function
int main() {
    RedBlackTree tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(15);
    tree.insert(25);

    cout << "Inorder Traversal after Insertion: ";
    tree.display();

    tree.deleteNode(25);
    cout << "Inorder Traversal after Deletion: ";
    tree.display();

    return 0;
}