//
// Created by Efe Cinar on 16.08.2023.
//

#ifndef CS300HW3_HASHTABLE_H
#define CS300HW3_HASHTABLE_H

#include <vector>
#include <iostream>
#include <string>

/* Begin: data structures and member functions taken from the Hashtables and Hashtables2 slides and updated */
template <class HashedObj>
class HashTable
{
public:
    explicit HashTable( const HashedObj & notFound, int size = 53 );
    HashTable( const HashTable & rhs )
            : ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
              array( rhs.array ), currentSize( rhs.currentSize ) { }

    HashedObj & find( const HashedObj & x );

    void makeEmpty( );
    void insert( const HashedObj & x );
    void remove( const HashedObj & x );
    int getCurrentSize() const;
    int getTableSize() const;

    const HashTable & operator=( const HashTable & rhs );

    enum EntryType { ACTIVE, EMPTY, DELETED };
private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry( const HashedObj & e = HashedObj( ),
                   EntryType i = EMPTY )
                : element( e ), info( i ) { }
    };

    std::vector<HashEntry> array;
    int currentSize;
    HashedObj ITEM_NOT_FOUND;

    bool isActive( int currentPos ) const;
    int findPos( const HashedObj & x ) const;
    void rehash();
};

/* End: data structures and member functions taken from the Hashtables and Hashtables2 slides and updated */

template<class HashedObj>
int HashTable<HashedObj>::getTableSize() const{
    return array.size();
}

template<class HashedObj>
int HashTable<HashedObj>::getCurrentSize() const {
    return currentSize;
}

int hash(const std::string & key, int tableSize)
{
    int hashVal = 0;

    for (int i = 0; i < key.length();i++)
        hashVal = 37 * hashVal + key[ i ];

    hashVal = hashVal % tableSize;

    if (hashVal < 0)
        hashVal = hashVal + tableSize;

    return(hashVal);
}


template<class HashedObj>
void HashTable<HashedObj>::makeEmpty() {
    for(HashEntry he: array){
        he.info = EMPTY;
    }
    currentSize = 0;
}

template<class HashedObj>
const HashTable<HashedObj> &HashTable<HashedObj>::operator=(const HashTable &rhs) {
    if (this != &rhs){
        makeEmpty();
        if (rhs.isEmpty()) return *this;
        ITEM_NOT_FOUND = rhs.ITEM_NOT_FOUND;
        array = rhs.array;
        currentSize = rhs.currentSize;
    }
    return *this;
}

int pow(int num, int power){
    int result = 1;
    for (int i = 0; i < power; i++){
        result *= num;
    }
    return result;
}

bool isPrime( int n )
{
    if ( n == 2 || n == 3 )
        return true;

    if ( n == 1 || n % 2 == 0 )
        return false;

    for ( int i = 3; i * i <= n; i += 2 )
        if ( n % i == 0 )
            return false;

    return true;
}


int nextPrime( int n )
{
    if ( n % 2 == 0 )
        n++;

    for ( ; ! isPrime( n ); n += 2 );

    return n;
}

template <class HashedObj>
HashTable<HashedObj>::HashTable(const HashedObj & notFound,
                                int size)
        : ITEM_NOT_FOUND( notFound ), array(nextPrime(size))
{
    makeEmpty();
}

template <class HashedObj>
int HashTable<HashedObj>::findPos( const HashedObj & x ) const
{
    int collisionNum = 0;
    int firstPos = hash( x.word, array.size( ) );
    int currentPos = firstPos;

    while ( array[ currentPos ].info != EMPTY &&
            array[ currentPos ].element != x )
    {
        currentPos = firstPos + pow(++collisionNum, 2) ;  //add the difference
        if ( currentPos >= array.size( ) )              // perform the mod
            currentPos %= array.size( );                // if necessary
    }
    return currentPos;
}

template <class HashedObj>
bool HashTable<HashedObj>::isActive( int currentPos ) const
{
    return array[ currentPos ].info == ACTIVE;
}

template <class HashedObj>
void HashTable<HashedObj>::remove( const HashedObj & x )
{
    int currentPos = findPos( x );
    if ( isActive( currentPos ) )
        array[ currentPos ].info = DELETED;
}

template <class HashedObj>
HashedObj & HashTable<HashedObj>::find( const HashedObj & x )
{
    int currentPos = findPos( x );
    if (isActive( currentPos ))
        return array[ currentPos ].element;

    return ITEM_NOT_FOUND;
}

template <class HashedObj>
void HashTable<HashedObj>::insert( const HashedObj & x )
{
    // Insert x as active
    int currentPos = findPos( x );
    if ( isActive( currentPos ) )
        return;
    array[ currentPos ] = HashEntry( x, ACTIVE );

    // enlarge the hash table if necessary
    double threshold = array.size() * 0.9;
    if ( ++currentSize >= threshold)
        rehash( );
}

template <class HashedObj>
void HashTable<HashedObj>::rehash( )
{
    std::vector<HashEntry> oldArray = array;

    // Create new double-sized, empty table
    array.resize( nextPrime( 2 * oldArray.size( ) ) );
    for ( int j = 0; j < array.size( ); j++ )
        array[ j ].info = EMPTY;

    // Copy table over
    currentSize = 0;
    for ( int i = 0; i < oldArray.size( ); i++ )
        if ( oldArray[ i ].info == ACTIVE )
            insert( oldArray[ i ].element );

    std::cout << "rehashed..." << std::endl;
    std::cout << "previous table size:" << oldArray.size() << ", new table size: " << array.size() << ", current unique word count " << currentSize << std::endl;
    std::cout << "current load factor: " << currentSize * 1.0 / array.size() << std::endl;
}

#endif //CS300HW3_HASHTABLE_H
