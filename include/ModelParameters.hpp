#pragma once
#include <vector>
#include "DataFrame.hpp"
#include "cuda_defs.h"

using namespace std;

class ModelParameters
{
private:
    real_type m_waveSpeed = 1;
public:
    void setWaveSpeed(real_type waveSpeed) {
        m_waveSpeed = waveSpeed;
    }
    CUDA_HOST_AND_DEVICE real_type getWaveSpeed() const {
        return m_waveSpeed;
    }
};
