#pragma once
#include <vector>
#include "DataFrame.hpp"

using namespace std;

class ModelParameters
{
private:
    real_type m_waveSpeed = 1;
public:
    void setWaveSpeed(real_type waveSpeed) {
        m_waveSpeed = waveSpeed;
    }
    real_type getWaveSpeed() const {
        return m_waveSpeed;
    }
};
