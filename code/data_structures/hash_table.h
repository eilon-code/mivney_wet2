#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "../wet2util.h"
#include "linked_list.h"

template<typename T>
class HashTable {
    public:
    HashTable():m_table(new List<T>[10]),m_size(0),m_buckets(10) {};
    ~HashTable();

    StatusType insert(int key, const T& value);
    output_t<T*> get(int key) const;
    void print() const;

    private:
    List<T>* m_table;
    int m_size;
    int m_buckets;
    void resize();
    int hash(const T& value) const;
    int hash(int key) const;
};

template<typename T>
HashTable<T>::~HashTable(){
    delete[] m_table;
}

template<typename T>
output_t<T*> HashTable<T>::get(int key) const{
    int index = hash(key);
    typename List<T>::Node *current = m_table[index].firstNode();
    while(current != nullptr){
        if(current->data.getId() == key){
            return &current->data;
        }
        current=current->next;
    }
    return StatusType::FAILURE;
}

template<typename T>
StatusType HashTable<T>::insert(int key, const T& value){
    if(get(key).status()==StatusType::SUCCESS){
        return StatusType::FAILURE;
    }
    if(m_size == m_buckets){
        resize();
    }
    int index = hash(key);
    m_table[index].add(value);
    m_size++;
    return StatusType::SUCCESS;
}

template <typename T>
void HashTable<T>::resize(){
    int old_buckets = m_buckets;
    m_buckets *= 2;
    List<T>* newTable = new List<T>[m_buckets];
    for(int i=0; i<old_buckets; i++){
        typename List<T>::Node *current = m_table[i].firstNode();
        while(current != nullptr){
            int newIndex = hash(current->data);
            newTable[newIndex].add(current->data);
            current = current->next;
        }
    }
    delete[] m_table;
    m_table = newTable;
}
template <typename T>
int HashTable<T>::hash(const T& value)const{
    return value.getId()%m_buckets;
}
template <typename T>
int HashTable<T>::hash(int key)const{
    return key%m_buckets;
}

//for testing
template <typename T>
void HashTable<T>::print()const{
    for(int i=0;i<m_buckets;i++){
        typename List<T>::Node *current = m_table[i].firstNode();
        std::cout << "[]-> ";
        while(current != nullptr){
            std::cout << current->data << " -> ";
            current = current->next;
        }
        std::cout << std::endl;
    }
}

#endif // HASH_TABLE_H_
