#include "real_type.hpp"
#include "DataFrame.hpp"
#include "ModelParameters.hpp"
#include "SolverParameters.hpp"
#include "SolverInterface.hpp"

class CudaSolver : public SolverInterface
{
public:
    void makeStep(
            const ModelParameters& modelParameters,
            const SolverParameters& solverParameters,
            const DataFrame& fprev,
            const DataFrame& fcur,
            DataFrame& fnext);
};
