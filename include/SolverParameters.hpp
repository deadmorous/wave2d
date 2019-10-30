#pragma once
#include <vector>
#include "DataFrame.hpp"
#include "cuda_defs.h"

using namespace std;

class SolverParameters
{
private:
    real_type m_spatialStepLength = 1;
    real_type m_timeStepLength = 1;
public:
    void setSpatialStepLength(real_type spatialStepLength) {
        m_spatialStepLength = spatialStepLength;
    }
    void setTimeStepLength(real_type timeStepLength) {
        m_timeStepLength = timeStepLength;
    }
    CUDA_HOST_AND_DEVICE real_type getSpatialStepLength() const {
        return m_spatialStepLength;
    }
    CUDA_HOST_AND_DEVICE real_type getTimeStepLength() const {
        return m_timeStepLength;
    }
};
