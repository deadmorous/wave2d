#include "real_type.hpp"
#include "solver_ref/RefSolverDataFrame.hpp"
#include "ModelParameters.hpp"
#include "SolverParameters.hpp"

class OmpSolver
{
public:
    OmpSolver();
    using DataFrame = RefSolverDataFrame;
    void makeStep(
            const ModelParameters& modelParameters,
            const SolverParameters& solverParameters,
            const RefSolverDataFrame& fprev,
            const RefSolverDataFrame& fcur,
            RefSolverDataFrame& fnext);
};
