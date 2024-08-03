#ifndef FLEET_H_
#define FLEET_H_

#include "wet2util.h"

class Fleet {
    public:
    Fleet(int id) : m_id(id), m_ships(1), m_size(0), m_rankOffset(0) {}
    ~Fleet(){}

    StatusType joinIn(Fleet other) {
        m_rankOffset = other.size();
    }

    StatusType addIn(Fleet other) {
        m_ships += other.ships();
        m_size += other.size();
    }

    int getId() const {
        return m_id;
    }

    int size() const {
        return m_size;
    }

    int ships() const {
        return m_ships;
    }

    int rankOffset() const {
        return m_rankOffset;
    }

    void addToMembersCount() {
        ++m_size;
    }

    bool operator<(const Fleet& other) {
        return size() < other.size();
    }

    private:
    int m_id;
    int m_ships;
    int m_size;
    int m_rankOffset;
};

#endif // FLEET_H_
