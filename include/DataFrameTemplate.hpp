#pragma once

#include "real_type.hpp"

template<class Container>
class DataFrameTemplate
{
    Container m_data;
    unsigned int m_width;
    unsigned int m_height;
public:
    using pointer = typename Container::pointer;
    using const_pointer = typename Container::const_pointer;

    DataFrameTemplate() : m_width(0), m_height(0)
    {}
    DataFrameTemplate(const_pointer data, unsigned int width, unsigned int height) :
        m_data(data, data+width*height),
        m_width(width),
        m_height(height)
    {}
    DataFrameTemplate(unsigned int width, unsigned int height) :
        m_data(width*height),
        m_width(width),
        m_height(height)
    {}
    const_pointer data() const {
        return m_data.data();
    }
    const_pointer cdata() const {
        return m_data.data();
    }
    pointer data() {
        return m_data.data();
    }
    unsigned int size() const {
        return m_width*m_height;
    }
    const_pointer begin() const {
        return m_data.data();
    }
    pointer begin() {
        return m_data.data();
    }
    const_pointer end() const {
        return m_data.data() + size();
    }
    pointer end() {
        return m_data.data() + size();
    }

    unsigned int width() const {
        return m_width;
    }
    unsigned int height() const {
        return m_height;
    }
    unsigned int indexOf(unsigned int x, unsigned int y) const {
        return x+y*m_width;
    }
    real_type& at(unsigned int x, unsigned int y) {
        return m_data.at(indexOf(x, y));
    }
    const real_type& at(unsigned int x, unsigned int y) const {
        return m_data.at(indexOf(x, y));
    }
};
