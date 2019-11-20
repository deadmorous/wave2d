#pragma once

#include "real_type.hpp"
#include <vector>

class FrameToImage
{
public:
    template<class DataFrame>
    FrameToImage(real_type amplitude, const DataFrame& frame) :
        m_amplitude(amplitude),
        m_width(frame.width()),
        m_height(frame.height()),
        m_data(frame.begin(), frame.end())
    {}

    void save() const;

private:
    real_type m_amplitude;
    unsigned int m_width;
    unsigned int m_height;
    std::vector<real_type> m_data;
};
