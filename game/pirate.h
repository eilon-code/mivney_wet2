#ifndef PIRATE_H_
#define PIRATE_H_

#include "fleet.h"

class Pirate {
    public:
    Pirate(int id) :  m_id(id), m_coins(0), m_rank(-1) {}
    ~Pirate() = default;

    void attach(Fleet fleet) {
        m_rank = fleet.size()+1 - fleet.rankOffset();
    }

    void changeCoins(int change) {
        m_coins += change;
    }

    int getId() const {
        return m_id;
    }

    int getCoins() const {
        return m_coins;
    }

    int getRank() const {
        return m_rank;
    }

    private:
    int m_id;
    int m_coins;
    int m_rank;
};

#endif // PIRATE_H_
