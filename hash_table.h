#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "wet2util.h"
#include "dynamic_array.h"
#include "linked_list.h"

template<typename T>
class HashTable {
    public:
    HashTable();
    ~HashTable();

    StatusType insert(int key, const T& value);
    output_t<T*> get(int key);
    output_t<T*> del(int key);// not necessary

    private:
    Array<List<T>> m_array;
};

#endif // HASH_TABLE_H_
