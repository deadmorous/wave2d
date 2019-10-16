#pragma once

// #include "SolverInterface.hpp"
#include "ModelParameters.hpp"
#include "SolverParameters.hpp"
#include "DataFrame.hpp"
#include <functional>

using namespace std;

class SolverController
{
private:
    DataFrame m_prevFrame;
    DataFrame m_currentFrame;
    ModelParameters m_modelParameters;
    SolverParameters m_solverParameters;

public:
    void setPrevStep(const DataFrame& prevStep) {
        m_prevFrame = prevStep;
    }
    void setCurrentStep(const DataFrame& currentStep) {
        m_currentFrame = currentStep;
    }
    void setModelParameters(const ModelParameters& modelParameters) {
        m_modelParameters = modelParameters;
    }
    void setSolverParameters(const SolverParameters& solverParameters) {
        m_solverParameters = solverParameters;
    }

    const DataFrame& getPrevStep() const {
        return m_prevFrame;
    }
    const DataFrame& getCurrentStep() const {
        return m_currentFrame;
    }
    ModelParameters getModelParameters() const {
        return m_modelParameters;
    }
    SolverParameters getSolverParameters()const {
        return m_solverParameters;
    }

    template<class Solver>
    void run(
            Solver& solver,
            std::function<bool(const DataFrame&)> cb)
    {
        DataFrame nextFrameData = m_prevFrame;
        DataFrame *prevFrame = &m_prevFrame;
        DataFrame *currentFrame = &m_currentFrame;
        DataFrame *nextFrame = &nextFrameData;
        while(true)
        {
            solver.makeStep(
                        m_modelParameters,
                        m_solverParameters,
                        *prevFrame,
                        *currentFrame,
                        *nextFrame);
            std::swap(prevFrame, currentFrame);
            std::swap(currentFrame, nextFrame);
            if (!cb(*nextFrame))
                break;
        }
        if (prevFrame != &m_prevFrame)
            swap(m_prevFrame, m_currentFrame);
    }
};
