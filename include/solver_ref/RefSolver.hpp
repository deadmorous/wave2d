#include "real_type.hpp"
#include "RefSolverDataFrame.hpp"
#include "ModelParameters.hpp"
#include "SolverParameters.hpp"

class RefSolver
{
public:
    RefSolver();
    using DataFrame = RefSolverDataFrame;
    void makeStep(
            const ModelParameters& modelParameters,
            const SolverParameters& solverParameters,
            const RefSolverDataFrame& fprev,
            const RefSolverDataFrame& fcur,
            RefSolverDataFrame& fnext);
};
