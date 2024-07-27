#ifndef UP_TREE_H_
#define UP_TREE_H_

#include "wet2util.h"
#include "dynamic_array.h"

template<typename T, typename S>
class UpTree {
    public:
    class Union {
        Union* root;
        S id;
        int size;
        Union(S unionId) : root(nullptr), id(unionId), size(0) {}
        bool isUnion() const {
            return root == nullptr;
        }

        void joinIn(Union* unionFather) {
            id.joinIn(unionFather);
            root = unionFather;
        }
    }
    class Node {
        Union* root;
        T value;
        Node(Union* union, const T& val) : root(union), value(val) {}
    };
    UpTree() : m_size(0) {}
    ~UpTree();

    StatusType makeSet(const S& unionId, int hashId);
    StatusType insertValue(Union* union, const T& value) {
        if (m_pointers.size() < m_size) {
            m_pointers.incrementSize(1); // maybe add a const
        }
        Node node(union, value);
        return m_pointers.set(m_size, node);
    }
    StatusType union2(Union* union1, Union* union2);
    Union* find(int valueId);

    private:
    Array<Node> m_pointers;
    int m_size;
};

#endif // UP_TREE_H_

template <typename T, typename S>
inline StatusType UpTree<T, S>::makeSet(const S& unionId, int hashId)
{
    Union newUnion(unionId);
    m_hash[hashId] = newUnion;
    return StatusType::SUCCESS;
}

template <typename T, typename S>
inline StatusType UpTree<T, S>::union2(Union* union1, Union* union2)
{
    if (*union1 < *union2) {
        union1->joinIn(union2);
    } else {
        union2->joinIn(union1);
    }
    return StatusType::SUCCESS;
}
