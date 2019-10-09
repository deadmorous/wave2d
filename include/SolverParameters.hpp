#pragma once
#include <vector>
#include "DataFrame.hpp"

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
        m_timeStepLength;
    }
    real_type getSpatialStepLength() const {
        return m_spatialStepLength;
    }
    real_type getTimeStepLength() const {
        return m_timeStepLength;
    }
};
