#pragma once

#include <memory>

template <class T>
public ref class m_shared_ptr sealed
{
    std::shared_ptr<T>* pPtr;

public:
    m_shared_ptr()
        : pPtr(new std::shared_ptr<T>())
    {}

    m_shared_ptr(T* t) {
        pPtr = new std::shared_ptr<T>(t);
    }

    m_shared_ptr(std::shared_ptr<T> t) {
        pPtr = new std::shared_ptr<T>(t);
    }

    m_shared_ptr(const m_shared_ptr<T>% t) {
        pPtr = new std::shared_ptr<T>(*t.pPtr);
    }

    !m_shared_ptr() {
        delete pPtr;
    }

    ~m_shared_ptr() {
        delete pPtr;
    }

    operator std::shared_ptr<T>() {
        return *pPtr;
    }

    m_shared_ptr<T>% operator=(T* ptr) {
        delete pPtr;
        pPtr = new std::shared_ptr<T>(ptr);
        return *this;
    }

    T* operator->() {
        return (*pPtr).get();
    }

    void reset() {
        pPtr->reset();
    }
};