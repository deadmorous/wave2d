#include "solver_cuda/CudaSolver.hpp"

__global__ void makeStepKernel(
        real_type *dst, const real_type *fprev, const real_type *fcur,
        unsigned int width, unsigned int height,
        ModelParameters modelParameters,
        SolverParameters solverParameters)
{
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i < width && j < height) {
        auto i_plus = i==width-1? 0: i+1;
        auto i_minus = i==0? width-1: i-1;
        auto j_plus = j==height-1? 0: j+1;
        auto j_minus = j==0? height-1: j-1;

        real_type gamma = modelParameters.getWaveSpeed()*
                solverParameters.getTimeStepLength()/solverParameters.getSpatialStepLength();
        real_type lapl =
                fcur[i_plus+j*width] +
                fcur[i+j_plus*width] +
                fcur[i_minus+j*width]+
                fcur[i+j_minus*width]-
                4*fcur[i+j*width];
        real_type u=2*fcur[i+j*width]-fprev[i+j*width]+gamma*gamma*lapl;
        dst[i+j*width] = u;
    }
}

class CudaSolver::Impl
{
public:
    void makeStep(
        const ModelParameters& modelParameters,
        const SolverParameters& solverParameters,
        const CudaSolverDataFrame& fprev,
        const CudaSolverDataFrame& fcur,
        CudaSolverDataFrame& fnext)
    {
        const unsigned int BLOCK_SIZE = 16;
        auto dst = fnext.data();
        dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE, 1);
        dim3 dimGrid((fprev.width()+dimBlock.x-1) / dimBlock.x, (fprev.height()+dimBlock.y-1) / dimBlock.y, 1);

        makeStepKernel<<<dimGrid, dimBlock>>> (
            thrust::raw_pointer_cast<real_type*>(fnext.deviceData().data()),
            thrust::raw_pointer_cast<const real_type*>(fprev.deviceData().data()),
            thrust::raw_pointer_cast<const real_type*>(fcur.deviceData().data()),
            fnext.width(), fnext.height(),
            modelParameters, solverParameters);
    }
};

CudaSolver::CudaSolver() :
    m_impl(std::make_shared<Impl>())
{
}

void CudaSolver::makeStep(
        const ModelParameters& modelParameters,
        const SolverParameters& solverParameters,
        const CudaSolverDataFrame& fprev,
        const CudaSolverDataFrame& fcur,
        CudaSolverDataFrame& fnext)
{
    m_impl->makeStep(
        modelParameters,
        solverParameters,
        fprev,
        fcur,
        fnext);
}
