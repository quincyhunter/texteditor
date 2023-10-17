// AVLSet.hpp
//
// ICS 46 Spring 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    ~AVLSet() noexcept override;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const noexcept;


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
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
    int iSize = 0;
    struct Node
        {
            ElementType elem;
            Node * left = nullptr;
            Node * right = nullptr;
        };
    Node * root = nullptr;
    void copyTreeRec( Node * &first, const Node * second);
    void deleteTreeRec(Node * treeroot);
    bool containshelper(const ElementType& element, Node* const&treeroot) const;
    int heighthelper(Node *treeroot) const;
    int needToBalance(Node * treeroot) const; 
    Node * addhelper(const ElementType& element, Node *& treeroot);
    void LL(Node *& treeroot); 
    void RR(Node *& treeroot); 
    void LR(Node *& treeroot); 
    void RL(Node *& treeroot); 
    void balancing(Node *& treeroot,const ElementType& element);
    bool shouldBalance = true;

    void preorderhelper(VisitFunction visit, Node *treeroot) const;
    void inorderhelper(VisitFunction visit, Node *treeroot) const;
    void postorderhelper(VisitFunction visit, Node *treeroot) const;
};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
{
    iSize = 0;
    root = nullptr;
    this->shouldBalance = shouldBalance;
}

template <typename ElementType>
void AVLSet<ElementType>::deleteTreeRec(Node* treeroot)
{
    if(treeroot) 
    {
        deleteTreeRec(treeroot->right);
        deleteTreeRec(treeroot->left);
        delete treeroot;
    }   
}

template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    deleteTreeRec(root);
}


template <typename ElementType>
void AVLSet<ElementType>::copyTreeRec(Node* &first, const Node* second) 
{
    //if original is not null
    if(second != nullptr) 
    {
        //create a new node for the current root node;
        first = new Node{second->elem};
        //if there is a left
        if (second->left)
        {
            //create a new node, link it with my copy and recurse down the tree
            Node * leftnode = new Node{second->left->elem};
            first->left = leftnode;
            copyTreeRec(first->left,second->left);
        }
        else
            //point to null othersise
            first->left = nullptr;
        
        //if right exists
        if (second -> right) 
        {
            //create a new node, link to copy and recurse down the tree
            Node * rightnode = new Node{second->right->elem};
            first->right = rightnode;
            copyTreeRec(first->right,second->right);
        }
        else 
            //else point to null
            first->right = nullptr;  
    }  
    else
    {
        //else point to null
        first = nullptr;
    }
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
    copyTreeRec(root,s.root);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
    : root{nullptr}
{
    iSize = s.iSize;
    root = s.root;
    s.iSize = 0;
    s.root = nullptr;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    deleteTreeRec(root);
    root = nullptr;
    copyTreeRec(root,s.root);
    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    if(this != &s)
    {
        deleteTreeRec(root);
        root = nullptr;
        copyTreeRec(root,s.root);
    }
    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
int AVLSet<ElementType>::needToBalance(Node * treeroot) const 
{
    int balval = heighthelper(treeroot->left) - heighthelper(treeroot->right);
    return balval;
}

template <typename ElementType>
void AVLSet<ElementType>::LL(Node *& treeroot) 
{
    Node * t = treeroot -> left;
    treeroot->left = t->right;
    t -> right = treeroot;
    treeroot = t;
}

template <typename ElementType>
void AVLSet<ElementType>::RR(Node *& treeroot) 
{
    Node * t = treeroot -> right;
    treeroot->right = t->left;
    t -> left = treeroot;
    treeroot = t;
}

template <typename ElementType>
void AVLSet<ElementType>::LR(Node *& treeroot) 
{
    LL(treeroot->left);
    RR(treeroot);
}

template <typename ElementType>
void AVLSet<ElementType>::RL(Node *& treeroot) 
{
    RR(treeroot->right);
    LL(treeroot);
}

template <typename ElementType>
void AVLSet<ElementType>::balancing(Node *& treeroot, const ElementType& element) 
{
    
    if (needToBalance(treeroot) > 1 && element < treeroot->left->elem)
        LL(treeroot);
    else if (needToBalance(treeroot) < -1 && element > treeroot->right->elem)
        RR(treeroot);
    else if (needToBalance(treeroot) > 1 && (element > treeroot->left->elem))
        {
        RR(treeroot->left);
        LL(treeroot);
        }
    else if(needToBalance(treeroot) < -1 &&  element < treeroot->right->elem)
    {
        LL(treeroot->right);
        RR(treeroot);
    }
}

template <typename ElementType>
typename AVLSet<ElementType>::Node * AVLSet<ElementType>::addhelper(const ElementType& element, Node *& treeroot)
{
    //add a value
    if (treeroot != nullptr)
    {
        if (element < treeroot->elem)
        {
            treeroot->left = addhelper(element, treeroot->left);
            if(shouldBalance ==true) 
            {
                if(needToBalance(treeroot))
                {
                    balancing(treeroot, element);
                }
            }
        }
        else if (element > treeroot->elem)
        {
            treeroot->right = addhelper(element, treeroot->right);
            if(shouldBalance == true) 
            {
                if(needToBalance(treeroot))
                {
                    balancing(treeroot, element);
                }
            }
        }
        return treeroot;
    }
    else 
    {
        iSize++;
        Node * newNode = new Node{element,nullptr,nullptr};
        treeroot = newNode;
        return treeroot;
    }

}




template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    //iSize++;
    addhelper(element,root);
    //balancing(root,element);
}

template <typename ElementType>
bool AVLSet<ElementType>::containshelper(const ElementType &element,  Node*const& treeroot) const
{
    if (treeroot == nullptr)
        return false;
    else
    {
        if(element == treeroot->elem)
            return true;
        else if (element > treeroot->elem)
            return containshelper(element,treeroot->right);
        else
            return containshelper(element, treeroot->left);

    }
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    return containshelper(element,root);
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return iSize;
}



template <typename ElementType>
int AVLSet<ElementType>::heighthelper(Node * treeroot) const
{
    if (treeroot != nullptr)
    {
        int l = heighthelper(treeroot -> left);
        int r = heighthelper(treeroot -> right);

        if (l > r)
            return (l+1);
        else    
            return (r + 1);
    }
    else
        return -1;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const noexcept
{
    return heighthelper(root);
}

template <typename ElementType>
void AVLSet<ElementType>::preorderhelper(VisitFunction visit, Node *treeroot) const
{
    if (treeroot)
    {
        visit(treeroot->elem);
        preorderhelper(visit,treeroot->left);
        preorderhelper(visit,treeroot->right);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    preorderhelper(visit,root);
}


template <typename ElementType>
void AVLSet<ElementType>::inorderhelper(VisitFunction visit, Node *treeroot) const
{
    if (treeroot)
    {
        inorderhelper(visit,treeroot->left);
        visit(treeroot->elem);
        inorderhelper(visit,treeroot->right);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    inorderhelper(visit,root);
}

template <typename ElementType>
void AVLSet<ElementType>::postorderhelper(VisitFunction visit, Node *treeroot) const
{
    if (treeroot)
    {
        postorderhelper(visit,treeroot->left);
        postorderhelper(visit,treeroot->right);
        visit(treeroot->elem);
    }
}


template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    postorderhelper(visit,root);
}



#endif

