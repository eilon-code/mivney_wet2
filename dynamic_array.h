#ifndef ARRAY_H_
#define ARRAY_H_

#include "wet2util.h"

template<typename T>
class Array {
    private:
        struct Cell {
            T* pointer;
            bool occupied;
        };
        Cell** m_data; // Pointer to an array of Cell pointers
        int m_size;
    
    public:
        Array() : m_data(nullptr), m_size(0) {}
        ~Array();

        StatusType set(int index, const T& value);
        output_t<T&> get(int index);
        
        StatusType remove(int index);
        StatusType insert(int index);
        StatusType incrementSize(int place);

        int size() const {
            return m_size;
        }

        bool empty() const {
            return size() == 0;
        }

        bool legal(int index) const {
            return index < size() && index >= 0;
        }
};

#endif // ARRAY_H_

template <typename T>
inline Array<T>::~Array()
{
    for (int i=0; i < size(); ++i) {
        Cell* cell = m_data[i];
        delete cell->pointer;
        delete cell;
    }
    delete[] m_data;
}

template <typename T>
inline StatusType Array<T>::set(int index, const T &value)
{
    if (!legal(index)) return StatusType::FAILURE;
    try {
        T* newValue = new T(value);
        Cell* cell = m_data[index];
        if (cell->occupied) {
            delete cell->pointer;
        }
        cell->pointer = newValue;
        cell->occupied = true;
    } catch(...) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

template <typename T>
inline output_t<T&> Array<T>::get(int index)
{
    if (!legal(index)) return StatusType::FAILURE;
    Cell* cell = m_data[index];
    if (!cell->occupied) return StatusType::FAILURE;
    return *(cell->pointer);
}

template <typename T>
inline StatusType Array<T>::remove(int index)
{
    if (!legal(index)) return StatusType::FAILURE;
    Cell* cell = m_data[index];
    if (!cell->occupied) return StatusType::FAILURE;

    try {
        delete cell->pointer;
        cell->pointer = nullptr;
        cell->occupied = false;
    } catch(...) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

template <typename T>
inline StatusType Array<T>::insert(int index)
{
    if (!legal(index)) return StatusType::FAILURE;
    StatusType result = incrementSize(1);
    if (result != StatusType::SUCCESS) return result;

    try {
        for (int i = index; i < m_size; ++i) {
            new_data[i + 1] = m_data[i];
        }
        Cell* cell = m_data[index];
    } catch(...) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

template <typename T>
inline StatusType Array<T>::incrementSize(int place)
{
    if (place < 0) return StatusType::FAILURE;
    try {
        // Create new array with increased size
        int newSize = m_size + place;
        Cell** new_data = new Cell*[newSize];
        for (int i = 0; i < m_size; ++i) {
            new_data[i] = m_data[i];
        }
        for (int i = m_size; i < newSize; ++i) {
            new_data[i] = new Cell(){nullptr, false};
        }

        delete[] m_data;
        m_data = new_data;
        m_size += place;
    } catch(...) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}
