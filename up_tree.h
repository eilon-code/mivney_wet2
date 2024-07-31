#ifndef UP_TREE_H_
#define UP_TREE_H_

#include "wet2util.h"
#include "dynamic_array.h"
#include "hash_table.h"

template<typename T, typename S>
class UpTree {
    public:
    class Union {
        Union* superUnion;
        S id;
        int size;
        Union(S unionId) : superUnion(nullptr), id(unionId), size(0) {}
        bool isUnion() const {
            return superUnion == nullptr;
        }

        StatusType joinIn(Union* root) {
            StatusType result = id.joinIn(root->id);
            if (result != StatusType::SUCCESS) {
                return result;
            }
            superUnion = root;
            return result;
        }
    }
    class Node {
        Union* root;
        T value;
        Node(Union* root, const T& val) : root(root), value(val) {}
    };
    UpTree() : m_size(0) {}
    ~UpTree();

    StatusType makeSet(const S& unionId, int hashId);
    StatusType insertValue(int unionId, const T& value);
    StatusType union2(Union* union1, Union* union2);
    output_t<Union*> find(int valueId);

    private:
    HashTable<Union> m_unionHash;
    HashTable<Node> m_nodeHash;
};

#endif // UP_TREE_H_

template <typename T, typename S>
inline StatusType UpTree<T, S>::makeSet(const S& unionId, int hashId)
{
    try {
        Union newUnion(unionId);
        return m_unionHash.insert(hashId, newUnion);
    } catch(...) {
        return StatusType::ALLOCATION_ERROR
    }
    
    return StatusType::SUCCESS;
}

template <typename T, typename S>
inline StatusType UpTree<T, S>::insertValue(int unionId, const T &value)
{
    output_t<Union*> search = m_unionHash.get(unionId);
    if (search.status() != StatusType::SUCCESS) {
        return search.status();
    }

    Union* root = search.ans();
    Node node(root, value);
    return m_nodeHash.insert(value.getId(), node);
}

template <typename T, typename S>
inline StatusType UpTree<T, S>::union2(Union* union1, Union* union2)
{
    Union superUion;
    Union subUion;
    if (*union1 < *union2) {
        superUion = union2;
        subUion = union1;
    } else {
        superUion = union1;
        subUion = union2;
    }
    return subUion->joinIn(superUion);;
}

template <typename T, typename S>
inline output_t<typename UpTree<T, S>::Union*> UpTree<T, S>::find(int valueId)
{
    output_t<Node*> search = m_nodeHash.get(valueId);
    if (search.status() != StatusType::SUCCESS) {
        return search.status();
    }
    Node* node = search.ans();
    Union* root = node->root;
    while (!root->superUnion) {
        root = root->superUnion
    }
    Union* subUnion = node->root;
    while (subUnion->superUnion != root) {
        Union* temp = subUnion->superUnion;
        subUnion->superUnion = root;
        subUnion = temp;
    }
    return root;
}
