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
    ~Impl() {
         maybeFree();
    }
    void makeStep(
        const ModelParameters& modelParameters,
        const SolverParameters& solverParameters,
        const DataFrame& fprev,
        const DataFrame& fcur,
        DataFrame& fnext)
{
    const unsigned int BLOCK_SIZE = 16;
    auto dst = fnext.data();
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE, 1);
    dim3 dimGrid((fprev.width()+dimBlock.x-1) / dimBlock.x, (fprev.height()+dimBlock.y-1) / dimBlock.y, 1);

    auto size = fnext.width()*fnext.height()*sizeof(real_type);
    maybeAlloc(size);

    cudaMemcpy(m_fprev, fprev.data(), size, cudaMemcpyHostToDevice);
    cudaMemcpy(m_fcur, fcur.data(), size, cudaMemcpyHostToDevice);

    makeStepKernel<<<dimGrid, dimBlock>>> (
        m_fnext, m_fprev, m_fcur,
        fnext.width(), fnext.height(),
        modelParameters, solverParameters);

    cudaMemcpy(fnext.data(), m_fnext, size, cudaMemcpyDeviceToHost);
}

private:
    real_type *m_fprev = nullptr;
    real_type *m_fcur = nullptr;
    real_type *m_fnext = nullptr;

    void maybeAlloc(size_t size) {
        if (!m_fprev) {
            cudaMalloc(&m_fprev, size);
            cudaMalloc(&m_fcur, size);
            cudaMalloc(&m_fnext, size);
        }
    }

    void maybeFree()
    {
        if (m_fprev) {
            cudaFree(m_fprev);
            cudaFree(m_fcur);
            cudaFree(m_fnext);
            m_fprev = nullptr;
        }
    }

};

CudaSolver::CudaSolver() :
    m_impl(std::make_shared<Impl>())
{
}

void CudaSolver::makeStep(
        const ModelParameters& modelParameters,
        const SolverParameters& solverParameters,
        const DataFrame& fprev,
        const DataFrame& fcur,
        DataFrame& fnext)
{
    m_impl->makeStep(
        modelParameters,
        solverParameters,
        fprev,
        fcur,
        fnext);
}
