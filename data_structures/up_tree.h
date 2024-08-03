#ifndef UP_TREE_H_
#define UP_TREE_H_

#include "wet2util.h"
#include "dynamic_array.h"
#include "hash_table.h"

template<typename T, typename S>
class UpTree {
    public:
    class Set {
        Set(const S& setId) : superSet(nullptr), id(setId) {}
        bool isRoot() const {
            return superSet == nullptr;
        }

        StatusType joinIn(Set* root) {
            if (superSet!=nullptr) return StatusType::FAILURE
            StatusType result = id.joinIn(root->id);
            if (result != StatusType::SUCCESS) {
                return result;
            }
            result = root->id.addIn(id);
            if (result != StatusType::SUCCESS) {
                return result;
            }
            superSet = root;
            return result;
        }

        public:
        Set* superSet;
        S id;
    }
    class Node {
        Set* root;
        T value;
        Node(Set* root, const T& val) : root(root), value(val) {}
    };
    UpTree(){}
    ~UpTree() = default;

    StatusType makeSet(const S& setId);
    StatusType insertValue(int setId, const T& value);
    StatusType union2Set(int set1Id, int set2Id);
    output_t<Set*> findSet(int setId);
    output_t<Set*> findSetOf(int valueId);
    output_t<T*> fetch(int valueId);

    private:
    HashTable<Set> m_setHash;
    HashTable<Node> m_nodeHash;
};

#endif // UP_TREE_H_

template <typename T, typename S>
inline StatusType UpTree<T, S>::makeSet(const S& SetId)
{
    try {
        Set newSet(SetId);
        return m_setHash.insert(SetId.getId(), newSet);
    } catch(...) {
        return StatusType::ALLOCATION_ERROR
    }
    
    return StatusType::SUCCESS;
}

template <typename T, typename S>
inline StatusType UpTree<T, S>::insertValue(int setId, const T &value)
{
    output_t<Set*> search = m_setHash.get(setId);
    if (search.status() != StatusType::SUCCESS) {
        return search.status();
    }
    Set* root = search.ans();

    Node node(root, value);
    StatusType result = m_nodeHash.insert(value.getId(), node);
    if (result != StatusType::SUCCESS) return result.status();

    value.attach(root->id);
    root.id.addToMembersCount();
    return result;
}

template <typename T, typename S>
inline StatusType UpTree<T, S>::union2Set(int set1Id, int set2Id)
{
    output_t<Set*> search1 = m_setHash.get(set1Id);
    output_t<Set*> search2 = m_setHash.get(set2Id);
    if (search1.status() != StatusType::SUCCESS) return search1.status();
    if (search2.status() != StatusType::SUCCESS) return search2.status();
    Set* set1 = search1.ans();
    Set* set2 = search2.ans();

    Set* superset;
    Set* subSet;

    if (set1->id < set2->id) {
        superSet = set2;
        subSet = set1;
    } else {
        superSet = set1;
        subSet = set2;
    }
    return subSet->joinIn(superSet);;
}

template <typename T, typename S>
inline output_t<typename UpTree<T, S>::Set*> UpTree<T, S>::findSet(int setId)
{
    output_t<Set*> search = m_setHash.get(setId);
    if (search.status() != StatusType::SUCCESS) {
        return search.status();
    }
    return search.ans();
}

template <typename T, typename S>
inline output_t<typename UpTree<T, S>::Set*> UpTree<T, S>::findSetOf(int valueId)
{
    output_t<Node*> search = m_nodeHash.get(valueId);
    if (search.status() != StatusType::SUCCESS) {
        return search.status();
    }
    Node* node = search.ans();
    Set* root = node->root;
    while (root->superSet != nullptr) {
        root = root->superSet
    }
    Set* subSet = node->root;// from now on, the hight optimization for all Sets in the way up
    while (subSet->superSet != root) {
        Set* temp = subSet->superSet;
        subSet->superSet = root;
        subSet = temp;
    }
    return root;
}

template <typename T, typename S>
inline output_t<T*> UpTree<T, S>::fetch(int valueId)
{
    output_t<Node*> search = m_nodeHash.get(valueId);
    if (search.status() != StatusType::SUCCESS) {
        return search.status();
    }
    Node* node = search.ans();
    return &(node->value);
}
