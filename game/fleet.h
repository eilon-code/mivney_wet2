#ifndef FLEET_H_
#define FLEET_H_

#include "wet2util.h"

class Fleet {
    public:
    Fleet(int id) : m_id(id), m_ships(1), m_size(0), m_rankOffset(0) {}
    ~Fleet(){}

    StatusType joinIn(Fleet other) {
        if (size() < other.size()) {
            m_rankOffset += other.size();
        } else {
            m_rankOffset -= size();
        }
    }

    StatusType addIn(Fleet other, Fleet defaultFleet) {
        if (size() < other.size()) {
            m_rankOffset += other.size();
            m_id = other.getId(); // override id
        }
        else if (size() == other.size()) {
            m_id = defaultFleet.getId(); // override id
        }
        m_ships += other.getShipCount();
        m_size += other.size();
    }

    int getId() const {
        return m_id;
    }

    int size() const {
        return m_size;
    }

    bool isEmpty() const {
        return m_size == 0;
    }

    int getShipCount() const {
        return m_ships;
    }

    int rankOffset() const {
        return m_rankOffset;
    }

    void offsetRank(int change) {
        m_rankOffset += change;
    }

    void addToMembersCount() {
        ++m_size;
    }

    bool operator<(const Fleet& other) {
        return getShipCount() < other.getShipCount();
    }

    bool operator!=(const Fleet& other) {
        return getId() != other.getId();
    }

    private:
    int m_id;
    int m_ships;
    int m_size;
    int m_rankOffset;
};

#endif // FLEET_H_
