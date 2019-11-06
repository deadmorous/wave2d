#include "real_type.hpp"
#include "DataFrame.hpp"
#include "ModelParameters.hpp"
#include "SolverParameters.hpp"
#include "SolverInterface.hpp"
#include <memory>

class CudaSolver : public SolverInterface
{
public:
    CudaSolver();

    void makeStep(
            const ModelParameters& modelParameters,
            const SolverParameters& solverParameters,
            const DataFrame& fprev,
            const DataFrame& fcur,
            DataFrame& fnext);
private:
    class Impl;
    std::shared_ptr<Impl> m_impl;
};
