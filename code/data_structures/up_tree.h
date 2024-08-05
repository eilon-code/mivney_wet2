#ifndef UP_TREE_H_
#define UP_TREE_H_

#include "../../wet2util.h"
#include "hash_table.h"

template<typename T, typename S>
class UpTree {
    public:
    class Set {
        public:
        Set(const S& setId) : superSet(nullptr), id(setId) {}
        ~Set()=default;
        StatusType joinIn(Set* root, Set* defaultSet) {
            if (superSet!=nullptr) return StatusType::FAILURE;
            StatusType result = id.joinIn(&(root->id), defaultSet->id);
            if (result != StatusType::SUCCESS) return result;

            superSet = root;
            return result;
        }
        bool isRoot() const {
            return superSet == nullptr;
        }

        int getId() const {
            return id.getId();
        }
        
        Set* superSet;
        S id;
    };
    class Node {
        public:
        Set* root;
        T value;
        Node(Set* root, const T& val) : root(root), value(val) {}
        ~Node()=default;
        int getId() const {
            return value.getId();
        }
    };
    UpTree() = default;
    ~UpTree() = default;

    StatusType makeSet(const S& setId);
    StatusType insertValue(int setId, const T& value);
    StatusType union2Sets(int set1Id, int set2Id);
    output_t<Set*> findSet(int setId);
    output_t<Set*> fetchSetOf(int valueId);
    output_t<T*> fetch(int valueId);

    private:
    HashTable<Set> m_setHash;
    HashTable<Node> m_nodeHash;
};

#endif // UP_TREE_H_

template <typename T, typename S>
inline StatusType UpTree<T, S>::makeSet(const S& SetId)
{
    Set newSet(SetId);
    return m_setHash.insert(SetId.getId(), newSet).status();
}

template <typename T, typename S>
inline StatusType UpTree<T, S>::insertValue(int setId, const T &value)
{
    output_t<Set*> search = findSet(setId);
    if (search.status() != StatusType::SUCCESS) return search.status();
    Set* root = search.ans();

    Node node(root, value);
    output_t<Node*> result = m_nodeHash.insert(value.getId(), node);
    if (result.status() != StatusType::SUCCESS) return result.status();
    Node* pointer = result.ans();
    pointer->value.attach(root->id);
    root->id.addToMembersCount();
    return result.status();
}

template <typename T, typename S>
inline StatusType UpTree<T, S>::union2Sets(int set1Id, int set2Id)
{
    output_t<Set*> search1 = findSet(set1Id);
    output_t<Set*> search2 = findSet(set2Id);
    if (search1.status() != StatusType::SUCCESS) return search1.status();
    if (search2.status() != StatusType::SUCCESS) return search2.status();
    Set* set1 = search1.ans();
    Set* set2 = search2.ans();

    if (set1->id.isEmpty() || set2->id.isEmpty()) return StatusType::FAILURE;

    Set* superSet;
    Set* subSet;

    if (set1->id < set2->id) {
        superSet = set2;
        subSet = set1;
    } else {
        superSet = set1;
        subSet = set2;
    }
    return subSet->joinIn(superSet, set1);
}

template <typename T, typename S>
inline output_t<typename UpTree<T, S>::Set*> UpTree<T, S>::findSet(int setId)
{
    output_t<Set*> search = m_setHash.get(setId);
    if (search.status() != StatusType::SUCCESS) {
        return search.status();
    }

    Set* searchResult = search.ans();
    Set* root = searchResult;
    int totalRankOffset = root->id.rankOffset();
    while (!root->isRoot()) {
        root = root->superSet;
        totalRankOffset += root->id.rankOffset();
    }
    Set* subSet = search.ans();// from now on, the hight optimization for all Sets in the way up
    while (subSet->superSet && subSet->superSet != root) {
        Set* temp = subSet->superSet;

        totalRankOffset -= subSet->id.rankOffset();
        subSet->id.offsetRank(totalRankOffset);
        subSet->superSet = root;
        
        subSet = temp;
    }

    if (root->id != searchResult->id) return StatusType::FAILURE;
    return root;
}

template <typename T, typename S>
inline output_t<typename UpTree<T, S>::Set*> UpTree<T, S>::fetchSetOf(int valueId)
{
    output_t<Node*> search = m_nodeHash.get(valueId);
    if (search.status() != StatusType::SUCCESS) return search.status();
    Node* node = search.ans();
    return node->root;
}

template <typename T, typename S>
inline output_t<T*> UpTree<T, S>::fetch(int valueId)
{
    output_t<Node*> search = m_nodeHash.get(valueId);
    if (search.status() != StatusType::SUCCESS) return search.status();
    Node* node = search.ans();
    return &(node->value);
};
