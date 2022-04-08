// AVLBag.hpp
#ifndef AVLBAG_HPP
#define AVLBAG_HPP

#include <functional>
#include <algorithm>
#include <iostream>

template <typename ElementType>
class AVLBag
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLBag to be empty.
    explicit AVLBag();

    // Cleans up the AVLBag so that it leaks no memory.
    ~AVLBag() noexcept;

    // Initializes a new AVLBag to be a copy of ana existing one.
    AVLBag(const AVLBag& s);

    // Initializes a new AVLBag whose contents are moved from an
    // expiring one.
    AVLBag(AVLBag&& s) noexcept;

    // Assigns an existing AVLBag into another.
    AVLBag& operator=(const AVLBag& s);

    // Assigns an expiring AVLBag into another.
    AVLBag& operator=(AVLBag&& s) noexcept;

    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    void add(const ElementType& element);


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    bool contains(const ElementType& element) const;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const noexcept;


    // amount() returns the amount of objects in that "bag". 
    // Duplicate objecs will get added to their respective bag.
    int amount(const ElementType& element) const;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;


private:
    struct Node
    {
        ElementType data;
        Node* left;
        Node* right;
        unsigned int amount;
    };

    Node* origin;
    unsigned int nodes;
    bool isBalanced;

    void cleanupRecursive(const Node* node);
    int heightOfTree(const Node* node) const noexcept;
    Node* RotateRight(Node* node);
    Node* RotateLeft(Node* node);
    Node* addRecursive(Node* node, const ElementType& element);
    bool containsRecursive(const Node* node, const ElementType& element) const;
    int amountRecursive(const Node* node, const ElementType& element) const;
    void preorderRecursive(VisitFunction visit, const Node* node) const;
    void inorderRecursive(VisitFunction visit, const Node* node) const;
    void postorderRecursive(VisitFunction visit, const Node* node) const;
    void copyTree(const Node* node);
};


template <typename ElementType>
AVLBag<ElementType>::AVLBag()
    : origin{nullptr}, nodes{0}, isBalanced{false}
{
}


template <typename ElementType>
void AVLBag<ElementType>::cleanupRecursive(const Node* node)
{
    if (node == nullptr)
    {
        return;
    }
    if (node->left != nullptr)
    {
        cleanupRecursive(node->left);
    }
    if (node->right != nullptr)
    {
        cleanupRecursive(node->right);
    }
    delete node;
    return;
}


template <typename ElementType>
AVLBag<ElementType>::~AVLBag() noexcept
{
    cleanupRecursive(origin);
}


template <typename ElementType>
void AVLBag<ElementType>::copyTree(const Node* node)
{
    // base case
    if (node == nullptr)
    {
        return;
    }
    // add itself
    add(ElementType{node->data});
    // add its left child
    if (node->left != nullptr)
    {
        copyTree(node->left);
    }
    // add its right child
    if (node->right != nullptr)
    {
        copyTree(node->right);
    }
}


template <typename ElementType>
AVLBag<ElementType>::AVLBag(const AVLBag& s)
    : origin{nullptr}, nodes{0}, isBalanced{false}
{
    copyTree(s.origin);
}


template <typename ElementType>
AVLBag<ElementType>::AVLBag(AVLBag&& s) noexcept
    : origin{nullptr}, nodes{0}, isBalanced{false}
{
    std::swap(origin, s.origin);
    std::swap(nodes, s.nodes);
}


template <typename ElementType>
AVLBag<ElementType>& AVLBag<ElementType>::operator=(const AVLBag& s)
{
    if (this != &s)
    {
        // cleanup old stuff
        cleanupRecursive(origin);
        origin = nullptr;
        nodes = 0;
        isBalanced = false;
        // copy it over
        copyTree(s.origin);
    }
    return *this;
}


template <typename ElementType>
AVLBag<ElementType>& AVLBag<ElementType>::operator=(AVLBag&& s) noexcept
{
    if (this != &s)
    {
        // cleanup old stuff
        cleanupRecursive(origin);
        origin = nullptr;
        nodes = 0;
        isBalanced = false;
        // swap it
        std::swap(nodes, s.nodes);
        std::swap(origin, s.origin);
    }
    return *this;
}


template <typename ElementType>
int AVLBag<ElementType>::heightOfTree(const Node* node) const noexcept
{
    if (node == nullptr)
    {
        return -1;
    }
    else if (node->left == nullptr && node->right == nullptr)
    {
        return 0;
    }
    return 1 + std::max(heightOfTree(node->left), heightOfTree(node->right));
}


template <typename ElementType>
typename AVLBag<ElementType>::Node* AVLBag<ElementType>::RotateRight(Node* node)
{
    // a is b's (node) left child
    Node* a = node->left;
    // t2 is a's right child
    Node* t2 = a->right;
    // a's right child is now b
    a->right = node;
    // b's left child is now t2
    node->left = t2;
    // return new "origin"
    return a;
}


template <typename ElementType>
typename AVLBag<ElementType>::Node* AVLBag<ElementType>::RotateLeft(Node* node)
{
    // b is a's (node) right child
    Node* b = node->right;
    // t2 is b's left child
    Node* t2 = b->left;
    // b's left child is now a
    b->left = node;
    // a's right child is now t2
    node->right = t2;
    // return new "origin"
    return b;
}


template <typename ElementType>
typename AVLBag<ElementType>::Node* AVLBag<ElementType>::addRecursive(Node* node, const ElementType& element)
{
    // base case
    if (node == nullptr)
    {
        nodes += 1;
        return new Node{element, nullptr, nullptr, 1};
    }
    if (element > node->data)
    {
        node->right = addRecursive(node->right, element);
    }
    else if (element < node->data)
    {
        node->left = addRecursive(node->left, element);
    }
    else
    {
        // if we run into a duplicate
        node->amount += 1;
        return node;
    }
    // This is here to save time. Because we will only ever have to rebalance once,
    // we can ensure that once we rebalance, we don't spend time checking if it is unbalanced
    // and instead recursively unwind.
    if (isBalanced == false)
    {
        // if you image the tree like a number line, where left is negative
        // and right is positive, this represents the bais towards one side.
        // if its skewed left it will be positive, if its skewed right it will
        // be negative.
        int skew = heightOfTree(node->left) - heightOfTree(node->right);
        // Skewed Right
        if (skew <= -2)
        {
            // Skewed Right
            if (element > node->right->data)
            {
                // rotate it left to fix
                isBalanced = true;
                return RotateLeft(node);
            }
            // Skewed Left
            if (element < node->right->data)
            {
                // rotate it right then left to fix
                isBalanced = true;
                node->right = RotateRight(node->right);
                return RotateLeft(node);
            }
        }
        // Skewed Left
        if (skew >= 2)
        {
            // Skewed Right
            if (element > node->left->data)
            {
                // rotate it left then right to fix it
                isBalanced = true;
                node->left = RotateLeft(node->left);
                return RotateRight(node);
            }
            // Skewed Left
            if (element < node->left->data)
            {
                // rotate it right to fix
                isBalanced = true;
                return RotateRight(node);
            }
        }
    }
    // if not we aren't balancing we just
    // return the node
    return node;
}


template <typename ElementType>
void AVLBag<ElementType>::add(const ElementType& element)
{
    isBalanced = false;
    origin = addRecursive(origin, element);
}


template <typename ElementType>
bool AVLBag<ElementType>::containsRecursive(const Node* node, const ElementType& element) const
{
    if (node == nullptr)
    {
        return false;
    }
    if (element < node->data)
    {
        return containsRecursive(node->left, element);
    }
    if (element > node->data)
    {
        return containsRecursive(node->right, element);
    }
    return element == node->data;
    
}


template <typename ElementType>
bool AVLBag<ElementType>::contains(const ElementType& element) const
{
    return containsRecursive(origin, element);
}


template <typename ElementType>
unsigned int AVLBag<ElementType>::size() const noexcept
{
    return nodes;
}


template <typename ElementType>
int AVLBag<ElementType>::height() const noexcept
{
    return heightOfTree(origin);
}


template <typename ElementType>
int AVLBag<ElementType>::amountRecursive(const Node* node, const ElementType& element) const
{
    if (node == nullptr)
    {
        return 0;
    }
    if (element < node->data)
    {
        return amountRecursive(node->left, element);
    }
    if (element > node->data)
    {
        return amountRecursive(node->right, element);
    }
    return node->amount;
}


template <typename ElementType>
int AVLBag<ElementType>::amount(const ElementType& element) const
{
    return amountRecursive(origin, element);
}


template <typename ElementType>
void AVLBag<ElementType>::preorderRecursive(VisitFunction visit, const Node* node) const
{
    visit(node->data);
    if (node->left != nullptr)
    {
        preorderRecursive(visit, node->left);
    }
    if (node->right != nullptr)
    {
        preorderRecursive(visit, node->right);
    }
    return;
}


template <typename ElementType>
void AVLBag<ElementType>::preorder(VisitFunction visit) const
{
    preorderRecursive(visit, origin);
}


template <typename ElementType>
void AVLBag<ElementType>::inorderRecursive(VisitFunction visit, const Node* node) const
{
    if (node->left != nullptr)
    {
        inorderRecursive(visit, node->left);
    }
    visit(node->data);
    if (node->right != nullptr)
    {
        inorderRecursive(visit, node->right);
    }
    return;
}


template <typename ElementType>
void AVLBag<ElementType>::inorder(VisitFunction visit) const
{
    inorderRecursive(visit, origin);
}


template <typename ElementType>
void AVLBag<ElementType>::postorderRecursive(VisitFunction visit, const Node* node) const
{
    if (node->left != nullptr)
    {
        postorderRecursive(visit, node->left);
    }
    if (node->right != nullptr)
    {
        postorderRecursive(visit, node->right);
    }
    visit(node->data);
    return;
}


template <typename ElementType>
void AVLBag<ElementType>::postorder(VisitFunction visit) const
{
    postorderRecursive(visit, origin);
}



#endif

