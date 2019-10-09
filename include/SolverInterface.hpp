#pragma once

#include "DataFrame.hpp"
#include "ModelParameters.hpp"
#include "SolverParameters.hpp"

using namespace std;

class SolverInterface
{
public:
    void makeStep(
            const ModelParameters& modelParameters,
            const SolverParameters& solverParameters,
            const DataFrame& fprev,
            const DataFrame& fcur,
            DataFrame& fnext);
};
