#ifndef FLEET_H_
#define FLEET_H_

#include "wet2util.h"

class Fleet {
    public:
    Fleet(int id) : m_id(id), m_ships(1), m_size(0), m_rankOffset(0), m_overrideId(id) {}
    ~Fleet(){}

    StatusType joinIn(Fleet other, Fleet defaultFleet) {
        if (size() < other.size() || (size() == other.size() && defaultFleet != *this)) {
            m_rankOffset += other.size();
        } else {
            m_rankOffset -= size();
        }

        return StatusType::SUCCESS;
    }

    StatusType addIn(Fleet other, Fleet defaultFleet) {
        if (size() < other.size()) {
            m_rankOffset += other.size();
            m_overrideId = other.getOverrideId(); // override id
        }
        else if (size() == other.size()) {
            if (defaultFleet != *this) {
                m_rankOffset += other.size();
            }
            m_overrideId = defaultFleet.getOverrideId(); // override id
        }
        m_ships += other.getShipCount();
        m_size += other.size();

        return StatusType::SUCCESS;
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
        return getId() != other.getOverrideId() && getOverrideId() != other.getId();
    }

    int getOverrideId() const {
        return m_overrideId;
    }

    private:
    int m_id;
    int m_ships;
    int m_size;
    int m_rankOffset;
    int m_overrideId;
};

#endif // FLEET_H_
