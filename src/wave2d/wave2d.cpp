#define _USE_MATH_DEFINES
#include <iostream>
#include <stdexcept>
#include <cmath>
#include "SolverController.hpp"

#ifdef WAVES2D_USE_CUDA
#include "solver_cuda/CudaSolver.hpp"
using Solver = CudaSolver;
#else // WAVES2D_USE_CUDA
#include "solver_ref/RefSolver.hpp"
using Solver = RefSolver;
#endif // WAVES2D_USE_CUDA

#include "ascii_art.hpp"
#include "FrameToImage.hpp"

using namespace std;
using SolverDataFrame = Solver::DataFrame;

namespace {

void outputFrameAscii(real_type amplitude, const SolverDataFrame& frame)
{
    AsciiArt<SolverDataFrame> aa(amplitude, frame);
    cout << aa << endl;
}

void outputFrameImage(real_type amplitude, const SolverDataFrame& frame)
{
    FrameToImage f2i(amplitude, frame);
    f2i.save();
}

void outputFrame(real_type amplitude, const SolverDataFrame& frame)
{
#ifdef WAVE2D_USE_QT
    outputFrameImage(amplitude, frame);
#else // WAVE2D_USE_QT
    outputFrameAscii(amplitude, frame);
#endif // WAVE2D_USE_QT
}

void fillStubData(
        real_type *dst,
        unsigned int width,
        unsigned int height,
        unsigned int stepNumber)
{
    const auto nx = 2; // How many periods in width
    const auto ny = 2; // How many periods in height
    const auto nk = 21; // How many time frames in one period
    auto a = static_cast<real_type>(2*M_PI*nx/width);
    auto b = static_cast<real_type>(2*M_PI*ny/height);
    auto c = static_cast<real_type>(2*M_PI/nk);
    for (auto y=0u; y<height; ++y)
        for (auto x=0u; x<width; ++x, ++dst)
            // *dst = sin(a*x + b*y + c*stepNumber);
            *dst = sin(a*x + c*stepNumber) * sin(b*y + c*stepNumber);
}

SolverDataFrame stubDataFrame(
        unsigned int width,
        unsigned int height,
        unsigned int stepNumber)
{
    vector<real_type> data(width*height);
    fillStubData(data.data(), width, height, stepNumber);
    return SolverDataFrame(data.data(), width, height);
}

template<class F>
void makeInitState(
        SolverController<SolverDataFrame>& sc,
        const SolverParameters& sp,
        unsigned int width,
        unsigned int height,
        F makeFrameData)
{
    vector<real_type> data(width*height);

    makeFrameData(data, sc, sp, width, height, make_real(0));
    sc.setPrevStep(SolverDataFrame(data.data(), width, height));
    makeFrameData(data, sc, sp, width, height, sp.getTimeStepLength());
    sc.setCurrentStep(SolverDataFrame(data.data(), width, height));
}

void makeRunningWaveInitState(
        SolverController<SolverDataFrame>& sc,
        const SolverParameters& sp,
        unsigned int width,
        unsigned int height,
        int k)
{
    makeInitState(sc, sp, width, height, [&k](
                  vector<real_type>& data,
                  const SolverController<SolverDataFrame>& sc,
                  const SolverParameters& sp,
                  unsigned int width,
                  unsigned int height,
                  real_type t)
    {
        auto d = data.data();
        auto a = static_cast<real_type>(2*M_PI*k/(width*sp.getSpatialStepLength()));
        auto c = sc.getModelParameters().getWaveSpeed();
        for (auto y=0u; y<height; ++y)
            for (auto x=0u; x<width; ++x, ++d)
                *d = sin(a*(x-c*t));
    });
}

void makeXyWavesInitState(
        SolverController<SolverDataFrame>& sc,
        const SolverParameters& sp,
        unsigned int width,
        unsigned int height,
        int kx, int ky)
{
    makeInitState(sc, sp, width, height, [&kx, &ky](
                  vector<real_type>& data,
                  const SolverController<SolverDataFrame>& sc,
                  const SolverParameters& sp,
                  unsigned int width,
                  unsigned int height,
                  real_type t)
    {
        auto sqr = [](real_type x) { return x*x; };
        auto d = data.data();
        auto ax = static_cast<real_type>(2*M_PI*kx/(width*sp.getSpatialStepLength()));
        auto ay = static_cast<real_type>(2*M_PI*ky/(width*sp.getSpatialStepLength()));
        auto c = sc.getModelParameters().getWaveSpeed();
        for (auto y=0u; y<height; ++y)
            for (auto x=0u; x<width; ++x, ++d) {
                auto r2 = sqr(make_real(x)/width - make_real(0.5)) + sqr(make_real(y)/height - make_real(0.5));
                auto cutoff = exp(-r2*50);
                *d = cutoff * sin(ax*(x-c*t))*sin(ay*y);
            }
    });
}

ostream& operator<<(ostream& s, const SolverDataFrame& f)
{
    auto d = f.data();
    for (auto y=0u; y<f.height(); ++y) {
        for (auto x=0u; x<f.width(); ++x, ++d) {
            if (x > 0)
                cout << '\t';
            cout << *d;
        }
        cout << endl;
    }
    return s;
}

class StubSolver//  : public SolverInterface
{
public:
    void makeStep(
            const ModelParameters& modelParameters,
            const SolverParameters& solverParameters,
            const SolverDataFrame& fprev,
            const SolverDataFrame& fcur,
            SolverDataFrame& fnext)
    {
        fillStubData(
                    fnext.data(),
                    fnext.width(),
                    fnext.height(),
                    m_stepNumber++);
    }

private:
    unsigned int m_stepNumber = 2;
};

} // anonymous namespace

int main()
{
    try {
//         StubSolver solver;
        Solver solver;
//        CudaSolver solver;
        SolverController<SolverDataFrame> sc;
        SolverParameters sp;
        sp.setTimeStepLength(make_real(0.5));
        sc.setSolverParameters(sp);
//        const auto StepCount = 100;
//        const auto Width = 300u, Height = 300u;
        const auto StepCount = 100;
        const auto Width = 500u, Height = 500u;

//        sc.setPrevStep(stubDataFrame(Width, Height, 0));
//        sc.setCurrentStep(stubDataFrame(Width, Height, 1));
        // makeRunningWaveInitState(sc, sp, Width, Height, 3);
        makeXyWavesInitState(sc, sp, Width, Height, 3, 3);
        auto stepNumber = 0;
        sc.run(solver, [&stepNumber](const SolverDataFrame& f) {
            // cout << "Step " << stepNumber << endl;
            if (stepNumber % 10 == 0)
                outputFrame(2, f);
            return ++stepNumber < 10*StepCount;
        });
#ifdef WAVE2D_USE_QT
        cout << "Using Qt" << endl;
#else // WAVE2D_USE_QT
        cout << "Not using Qt" << endl;
#endif // WAVE2D_USE_QT
        return 0;
    }
    catch(const std::exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }
}
