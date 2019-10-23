#include "real_type.hpp"
#include "DataFrame.hpp"
#include "ModelParameters.hpp"
#include "SolverParameters.hpp"
#include "SolverInterface.hpp"

__global__ void makeStepKernel()
{
}

class CudaSolver : public SolverInterface
{
public:
    void makeStep(
            const ModelParameters& modelParameters,
            const SolverParameters& solverParameters,
            const DataFrame& fprev,
            const DataFrame& fcur,
            DataFrame& fnext)
    {
	// TODO
	const unsigned int BLOCK_SIZE = 16;
        auto dst = fnext.data();
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE, 1);
	dim3 dimGrid((fprev.width()+dimBlock.x-1) / dimBlock.x, (fprev.height()+dimBlock.y-1) / dimBlock.y, 1);
    
	makeStepKernel<<<dimGrid, dimBlock>>> ();
    }
};
