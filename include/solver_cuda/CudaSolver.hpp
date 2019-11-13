#include "real_type.hpp"
#include "CudaSolverDataFrame.hpp"
#include "ModelParameters.hpp"
#include "SolverParameters.hpp"
#include <memory>

class CudaSolver
{
public:
    using DataFrame = CudaSolverDataFrame;
    CudaSolver();

    void makeStep(
            const ModelParameters& modelParameters,
            const SolverParameters& solverParameters,
            const CudaSolverDataFrame& fprev,
            const CudaSolverDataFrame& fcur,
            CudaSolverDataFrame& fnext);
private:
    class Impl;
    std::shared_ptr<Impl> m_impl;
};
