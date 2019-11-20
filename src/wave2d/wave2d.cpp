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
        sp.setTimeStepLength(make_real(0.01));
        sc.setSolverParameters(sp);
//        const auto StepCount = 100;
//        const auto Width = 300u, Height = 300u;
        const auto StepCount = 100;
        const auto Width = 100u, Height = 100u;

        sc.setPrevStep(stubDataFrame(Width, Height, 0));
        sc.setCurrentStep(stubDataFrame(Width, Height, 1));
        auto stepNumber = 0;
        sc.run(solver, [&stepNumber](const SolverDataFrame& f) {
            // cout << "Step " << stepNumber << endl;
            outputFrame(2, f);
            return ++stepNumber < StepCount;
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
