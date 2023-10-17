// HashSet.hpp
//
// ICS 46 Spring 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    ~HashSet() noexcept override;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8, in which case
    // the new capacity should be determined by this formula:
    //
    //     capacity * 2 + 1
    //
    // In the case where the array is resized, this function runs in linear
    // time (with respect to the number of elements, assuming a good hash
    // function); otherwise, it runs in constant time (again, assuming a good
    // hash function).  The amortized running time is also constant.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns false.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    HashFunction hashFunction;
    int cap = 0;
    struct Node
        {
            ElementType elem;
            Node * next = nullptr;
        };
    int iSize = 0;
    Node *hasharr;

    // You'll no doubt want to add member variables and "helper" member
    // functions here.
};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{
    cap = DEFAULT_CAPACITY;
    hasharr = new Node[cap];
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    for (int i = 0; i<cap; i++) 
    {
        for(int j = 0; hasharr[i].next != nullptr; j++ ) 
        {
            Node * pt = hasharr[i].next->next;
            delete hasharr[i].next;
            hasharr[i].next = pt;
        }
    }
    if (cap) 
        delete[] hasharr;
    else
        delete hasharr;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{s.hashFunction}
{
    cap = s.cap;
    iSize = 0;
    hasharr = new Node[cap];
    for (int i = 0; i < cap; i++) 
    {
        Node * tmp;
        tmp = s.hasharr[i].next;
        for (int j = 0; tmp != nullptr; tmp = s.hasharr[i].next->next) 
        {
            iSize += 1;
            int hashval = hashFunction(s.hasharr[i].next->elem) % cap;
            Node* newNode = new Node{s.hasharr[i].next->elem,nullptr};
            Node * tmp;
            if (hasharr[hashval].next) 
            {
                tmp = hasharr[hashval].next;
                for( int i = 0; tmp->next!=nullptr;tmp = tmp->next) { i++;}
                tmp->next = newNode;
            }
            else 
            {
                hasharr[hashval].next = newNode;
            }
            j++;
        }
        
    }
    
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{s.hashFunction}
{
    cap = s.cap;
    iSize = s.iSize;
    hasharr = s.hasharr;
    s.hasharr = nullptr;
    s.cap = 0;
    s.iSize = 0;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    
    //delete this
    for (int i = 0; i<cap; i++) 
    {
        for(int j = 0; hasharr[i].next != nullptr; j++ ) 
        {
            Node * pt = hasharr[i].next->next;
            delete hasharr[i].next;
            hasharr[i].next = pt;
        }
    }
    if (cap) 
        delete[] hasharr;
    else
        delete hasharr;

    //copy construct this
    cap = s.cap;
    iSize = 0;
    hasharr = new Node[cap];
    for (int i = 0; i < cap; i++) 
    {
        Node * tmp;
        tmp = s.hasharr[i].next;
        for (int j = 0; tmp != nullptr; tmp = s.hasharr[i].next->next) 
        {
            iSize += 1;
            int hashval = hashFunction(s.hasharr[i].next->elem) % cap;
            Node* newNode = new Node{s.hasharr[i].next->elem,nullptr};
            Node * tmp;
            if (hasharr[hashval].next) 
            {
                tmp = hasharr[hashval].next;
                for( int i = 0; tmp->next!=nullptr;tmp = tmp->next) { i++;}
                tmp->next = newNode;
            }
            else 
            {
                hasharr[hashval].next = newNode;
            }
            j++;
        }
        
    }
    return * this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    //delete this
    if (this != &s)
    {
        for (int i = 0; i<cap; i++) 
        {
            for(int j = 0; hasharr[i].next != nullptr; j++ ) 
            {
                Node * pt = hasharr[i].next->next;
                delete hasharr[i].next;
                hasharr[i].next = pt;
            }
        }
        if (cap) 
            delete[] hasharr;
        else
            delete hasharr;

        //move this;
        cap = s.cap;
        iSize = s.iSize;
        hasharr = s.hasharr;
        hashFunction = s.hashFunction;
        s.hasharr = nullptr;
        s.cap = 0;
        s.iSize = 0;


    }
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    if (this->contains(element))
        return;
    else
    {
        iSize += 1;
        int hashval = hashFunction(element) % cap;
        Node* newNode = new Node{element,nullptr};
        Node * tmp;
        if (hasharr[hashval].next) 
        {
            tmp = hasharr[hashval].next;
            for( int i = 0; tmp->next!=nullptr;tmp = tmp->next) { i++;}
            tmp->next = newNode;
        }
        else 
        {
            hasharr[hashval].next = newNode;
        }
    }
    if ((cap*0.8) < iSize) 
    {
        int oldcap = cap;
        cap = oldcap * 2 + 1;
        Node * tmp = hasharr;
        hasharr = new Node[cap];
        iSize = 0;
        for (int i =0; i < oldcap;i++) 
        {
            Node * pt;
            while(tmp[i].next != nullptr)
            {

                iSize++;
                int hashval = hashFunction(tmp[i].next->elem) % cap;
                Node* newNode = new Node{tmp[i].next->elem,nullptr};
               
                if (hasharr[hashval].next) 
                {
                    Node * tmp;
                    tmp = hasharr[hashval].next;
                    for( int i = 0; tmp->next!=nullptr;tmp = tmp->next) {i++;}
                    tmp->next = newNode;
                }
                else 
                {
                    hasharr[hashval].next = newNode;
                } 

                pt = tmp[i].next->next;
                delete tmp[i].next;
                tmp[i].next = pt;
            }
            delete tmp[i].next;

        }
        delete[] tmp;
    }
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    int hashval = hashFunction(element) % cap;
    Node* tmp = hasharr[hashval].next;
    for (int i = 0; tmp != nullptr; i++) {
        if (tmp->elem == element) 
        {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return iSize;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    if (index > cap) 
        return 0;
    else 
    {
        Node * tmp = hasharr[index].next;
        int cnt = 0;
        for(cnt = 0; tmp != nullptr; cnt++)
        {
            tmp = tmp->next;
        }
        return cnt;
    }
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    if (index > cap) 
        return 0;
    else 
    {
        Node * tmp = hasharr[index].next;
        for(int cnt = 0; tmp != nullptr; cnt++)
        {
            if (tmp->elem == element)
                return true;
            tmp = tmp->next;
        }
        return false;
    }
}



#endif

