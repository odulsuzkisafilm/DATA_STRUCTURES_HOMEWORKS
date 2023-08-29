//
// Created by Efe Cinar on 03.08.2023.
//

#ifndef CS300HW3_AVLSEARCHTREE_H
#define CS300HW3_AVLSEARCHTREE_H
#include <iostream>

template <class Comparable>
class AvlSearchTree;

template <class Comparable>
class AvlNode
{
    Comparable 	element;
    AvlNode   		*left;
    AvlNode   		*right;
    int        		height;

    AvlNode(const Comparable & theElement, AvlNode *lt, AvlNode *rt, int h = 0)
            : element( theElement ), left( lt ), right( rt ), height( h ) { }

    friend class AvlSearchTree<Comparable>;
};

template <class Comparable>
class AvlSearchTree
{
public:
    explicit AvlSearchTree(const Comparable & notFound);
    AvlSearchTree(const AvlSearchTree & rhs );
    ~AvlSearchTree( );

    const Comparable & findMin( ) const;
    const Comparable & findMax( ) const;
    Comparable & find(const Comparable & x );
    bool isEmpty( ) const;
    void printTree( ) const;

    void makeEmpty( );
    void insert(const Comparable & x);
    void remove(const Comparable & x);

    const AvlSearchTree & operator=(const AvlSearchTree & rhs );

private:
    AvlNode<Comparable> *root;
    Comparable ITEM_NOT_FOUND;

    Comparable & elementAt(AvlNode<Comparable> *t );

    void insert(const Comparable & x, AvlNode<Comparable> * & t) const;
    void remove(const Comparable & x, AvlNode<Comparable> * & t);

    AvlNode<Comparable> * findMin( AvlNode<Comparable> *t ) const;
    AvlNode<Comparable> * findMax( AvlNode<Comparable> *t ) const;
    AvlNode<Comparable> * find(const Comparable & x, AvlNode<Comparable> *t);
    void makeEmpty( AvlNode<Comparable> * & t ) const;
    void printTree( AvlNode<Comparable> *t ) const;
    AvlNode<Comparable> * clone( AvlNode<Comparable> *t ) const;

    // Avl manipulations
    int height( AvlNode<Comparable> *t ) const;
    int max( int lhs, int rhs ) const;
    void rotateWithLeftChild( AvlNode<Comparable> * & k2 ) const;
    void rotateWithRightChild( AvlNode<Comparable> * & k1 ) const;
    void doubleWithLeftChild( AvlNode<Comparable> * & k3 ) const;
    void doubleWithRightChild( AvlNode<Comparable> * & k1 ) const;
};

template <class Comparable>
void AvlSearchTree<Comparable>::remove(const Comparable & x, AvlNode<Comparable> * & t)
{
    if (t == nullptr) {
        return; // The comparable object is not here so no need to do something.
    }

    if (x < t->element) {
        remove(x, t->left); // Element is in the left subtree so focusing on the left branch.
    } else if (x > t->element) {
        remove(x, t->right); // Element is in the right subtree so focusing on the left branch.
    } else if (t->left != nullptr && t->right != nullptr) {
        // Node with two children
        t->element = findMin(t->right)->element; // Replace with the inorder successor
        remove(t->element, t->right); // Remove the inorder successor
    } else {
        // Node with one or no children
        AvlNode<Comparable> *oldNode = t;
        t = (t->left != nullptr) ? t->left : t->right; // carrying the content of the grandchild to oldNode's node
        delete oldNode;
    }

    // Fixing the tree's balance after deletion
    if (t != nullptr) {
        int leftHeight = height(t->left);
        int rightHeight = height(t->right);
        t->height = 1 + max(leftHeight, rightHeight);

        // Checking if the node is unbalanced
        int heightDifference = leftHeight - rightHeight;

        if (heightDifference > 1) {
            // Left subtree is higher
            if (height(t->left->left) >= height(t->left->right)) {
                rotateWithLeftChild(t);
            } else {
                doubleWithLeftChild(t);
            }
        } else if (heightDifference < -1) {
            // Right subtree is higher
            if (height(t->right->right) >= height(t->right->left)) {
                rotateWithRightChild(t);
            } else {
                doubleWithRightChild(t);
            }
        }
    }
}


template<class Comparable>
bool AvlSearchTree<Comparable>::isEmpty() const {
    return root == nullptr;
}

template<class Comparable>
AvlSearchTree<Comparable>::AvlSearchTree(const Comparable &notFound): root(nullptr), ITEM_NOT_FOUND(notFound) {}

template <class Comparable>
int AvlSearchTree<Comparable>::height(AvlNode<Comparable> *t ) const
{
    if (t == nullptr)
        return -1;
    return t->height;
}

template <class Comparable>
int AvlSearchTree<Comparable>::max(int lhs, int rhs )
const
{
    if (lhs > rhs)
        return lhs;

    return rhs;
}

template <class Comparable>
void AvlSearchTree<Comparable>::insert(const Comparable & x, AvlNode<Comparable> * & t) const
{
    if ( t == nullptr)
        t = new AvlNode<Comparable>(x, nullptr, nullptr);

    else if (x < t->element) {
        // X should be inserted to the left tree!
        insert( x, t->left );

        // Check if the left tree is out of balance (left subtree grew in height!)
        if (height( t->left ) - height( t->right ) == 2)
            if ( x < t->left->element )  // X was inserted to the left-left subtree!
                rotateWithLeftChild( t );
            else			     // X was inserted to the left-right subtree!
                doubleWithLeftChild( t );
    }
    else if(t->element < x)
    {    // Otherwise X is inserted to the right subtree
        insert( x, t->right );
        if (height( t->right ) - height( t->left ) == 2)
            // height of the right subtree increased
            if (t->right->element < x)
                // X was inserted to right-right subtree
                rotateWithRightChild(t);
            else // X was inserted to right-left subtree
                doubleWithRightChild(t);
    }
    else
        ;  // Duplicate; do nothing

    // update the height of the node
    t->height = max(height(t->left), height(t->right)) + 1;
}

template <class Comparable>
void AvlSearchTree<Comparable>::rotateWithLeftChild(AvlNode<Comparable> * & k2 ) const
{
    AvlNode<Comparable> *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
    k1->height = max( height( k1->left ), k2->height ) + 1;
    k2 = k1;
}

template <class Comparable>
void AvlSearchTree<Comparable>::rotateWithRightChild( AvlNode<Comparable> * & k1 ) const
{
    AvlNode<Comparable> *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
    k2->height = max( height( k2->right ), k1->height ) + 1;
    k1 = k2;
}

template <class Comparable>
void AvlSearchTree<Comparable>::doubleWithLeftChild( AvlNode<Comparable> * & k3 ) const
{
    rotateWithRightChild( k3->left );
    rotateWithLeftChild( k3 );
}

template <class Comparable>
void AvlSearchTree<Comparable>::doubleWithRightChild( AvlNode<Comparable> * & k1 ) const
{
    rotateWithLeftChild( k1->right );
    rotateWithRightChild( k1 );
}

template <class Comparable>
Comparable & AvlSearchTree<Comparable>::elementAt( AvlNode<Comparable> *t )
{
    return t == nullptr ? ITEM_NOT_FOUND : t->element;
}

template <class Comparable>
Comparable & AvlSearchTree<Comparable>::find(const Comparable & x)
{
    return elementAt(find(x, root));
}

template <class Comparable>
AvlNode<Comparable> * AvlSearchTree<Comparable>::find(const Comparable & x, AvlNode<Comparable> * t )
{
    if (t == nullptr)
        return nullptr;
    else if(x < t->element)
        return find(x, t->left);
    else if(t->element < x)
        return find(x, t->right);
    else
        return t;    // Match
}

template <class Comparable>
const Comparable & AvlSearchTree<Comparable>::findMin() const
{
    return elementAt( findMin( root ) );
}

template <class Comparable>
AvlNode<Comparable> * AvlSearchTree<Comparable>::findMin(AvlNode<Comparable> *t )
const
{
    if(t == nullptr)
        return nullptr;
    if(t->left == nullptr)
        return t;
    return findMin(t->left);
}

template <class Comparable>
const Comparable & AvlSearchTree<Comparable>::findMax() const
{
    return elementAt(findMax(root));
}

template <class Comparable> AvlNode<Comparable> * AvlSearchTree<Comparable>::findMax(AvlNode<Comparable> *t)
const
{
    if(t != nullptr)
        while(t->right != nullptr)
            t = t->right;
    return t;
}

template <class Comparable>
void AvlSearchTree<Comparable>::insert(const Comparable & x)
{
    insert(x, root);
}

template <class Comparable>
void AvlSearchTree<Comparable>::remove(const Comparable & x)
{
    remove(x, root);
}


template <class Comparable>
void AvlSearchTree<Comparable>::makeEmpty()
{
    makeEmpty(root);
}

template <class Comparable>
void AvlSearchTree<Comparable>::
makeEmpty(AvlNode<Comparable> * & t) const
{
    if(t != nullptr)
    {
        makeEmpty( t->left );
        makeEmpty( t->right );
        delete t;
    }
    t = nullptr;
}

template <class Comparable>
void AvlSearchTree<Comparable>::printTree( ) const
{
    if(isEmpty())
        std::cout << "Empty tree" << std::endl;
    else
        printTree(root);
}

template <class Comparable>
AvlSearchTree<Comparable>::~AvlSearchTree( )
{
    makeEmpty();
}

template<class Comparable>
AvlSearchTree<Comparable>::AvlSearchTree(const AvlSearchTree &rhs): root(NULL), ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND){
    *this = rhs;
}

template<class Comparable>
void recursiveCloner(AvlNode<Comparable>* & lhsPtr, AvlNode<Comparable>* & ptr, const AvlSearchTree<Comparable> & rhs){
    if(ptr->left != nullptr){
        lhsPtr->left = rhs.clone(ptr->left);
        recursiveCloner(lhsPtr->left, ptr->left, rhs);
    }
    if(ptr->right != nullptr){
        lhsPtr->right = rhs.clone(ptr->right);
        recursiveCloner(lhsPtr->right, ptr->right, rhs);
    }
}

template<class Comparable>
const AvlSearchTree<Comparable> & AvlSearchTree<Comparable>::operator=(const AvlSearchTree &rhs) {
    if (this != &rhs){
        makeEmpty();
        if (rhs.isEmpty()) return *this;

        root = rhs.clone(rhs.root);
        recursiveCloner(root, rhs.root, rhs);
    }
    return *this;
}

template <class Comparable>
void AvlSearchTree<Comparable>::printTree( AvlNode<Comparable> * t ) const
{
    if ( t != NULL )
    {
        printTree(t->left);
        std::cout << t->element << std::endl;
        printTree(t->right);
    }
}

template <class Comparable>
AvlNode<Comparable> * AvlSearchTree<Comparable>::clone(AvlNode<Comparable> * t) const
{
    if ( t == NULL )
        return NULL;
    else
        return new AvlNode<Comparable> (t->element, clone(t->left), clone(t->right));
}


#endif //CS300HW3_AVLSEARCHTREE_H
