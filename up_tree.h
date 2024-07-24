#ifndef UP_TREE_H_
#define UP_TREE_H_

#include "wet2util.h"
#include "dynamic_array.h"

template<typename T>
class UpTree {
    public:
    struct Group {
        int id;
    }
    class Node {
        Node* m_root;
        Group* m_group;
        T* m_key;
        int m_size;
        Node(T* key) : m_root(nullptr), m_group(nullptr), m_key(key) {}
    };
    UpTree() : m_size(0), m_groups(0) {}
    ~UpTree();

    output_t<T*> makeSet(const T& key);
    StatusType union2(Node* node1, Node* node2);
    find()

    private:
    int m_size;
    int m_groups;
    Array<Node> m_pointers;
};

#endif // UP_TREE_H_

template <typename T>
inline output_t<T *> UpTree<T>::makeSet(const T &key)
{
    
    return output_t<T *>();
}
