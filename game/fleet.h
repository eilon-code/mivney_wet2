class Fleet {
    public:
    Fleet(int id) : m_id(id), m_ships(1), m_size(0) {}
    ~Fleet();

    int getId() const {
        return m_id;
    }

    int size() const {
        return m_size;
    }

    void addToMembersCount() {
        ++m_size;
    }

    private:
    int m_id;
    int m_ships;
    int m_size;
};
