#pragma once

#include "real_type.hpp"

template<class Container>
class DataFrameTemplate
{
    Container m_data;
    unsigned int m_width;
    unsigned int m_height;
public:
    DataFrameTemplate() : m_width(0), m_height(0)
    {}
    DataFrameTemplate(const real_type *data, unsigned int width, unsigned int height) :
        m_data(data, data+width*height),
        m_width(width),
        m_height(height)
    {}
    DataFrameTemplate(unsigned int width, unsigned int height) :
        m_data(width*height),
        m_width(width),
        m_height(height)
    {}
    const real_type *data() const {
        return m_data.data();
    }
    const real_type *cdata() const {
        return m_data.data();
    }
    real_type *data() {
        return m_data.data();
    }
    unsigned int size() const {
        return m_width*m_height;
    }
    const real_type *begin() const {
        return m_data.data();
    }
    real_type *begin() {
        return m_data.data();
    }
    const real_type *end() const {
        return m_data.data() + size();
    }
    real_type *end() {
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
