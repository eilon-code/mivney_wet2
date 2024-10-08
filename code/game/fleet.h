#ifndef FLEET_H_
#define FLEET_H_

#include "wet2util.h"

class Fleet {
    public:
    Fleet(int id) : m_id(id), m_ships(1), m_size(0), m_rankOffset(0), m_overrideId(id) {}
    ~Fleet(){}

    StatusType joinIn(Fleet* other, const Fleet& defaultFleet) {
        // std::cout << "low: " << size() << ", up: " << other->size() << std::endl;
        bool defaultChoice = size() == other->size();
        bool defaultIsUp = other->getId() == defaultFleet.getId();
        bool correctOrder = size() < other->size() || (defaultChoice && defaultIsUp);
        if (correctOrder) {
            m_rankOffset += other->size();
        } else {
            m_rankOffset -= size();
        }
        m_rankOffset -= other->rankOffset();
        // std::cout << "lowR: " << rankOffset() << ", upR: " << other->rankOffset() << std::endl;
        StatusType result = other->addIn(*this, correctOrder);

        return result;
    }

    StatusType addIn(const Fleet& other, bool correctOrder) {
        if (!correctOrder) {
            m_rankOffset += other.size();
            m_overrideId = other.getOverrideId(); // override id
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
        return !(*this == other);
    }

    bool operator==(const Fleet& other) {
        return getId() == other.getOverrideId() || getOverrideId() == other.getId();
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
