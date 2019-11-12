#define _USE_MATH_DEFINES
#include <iostream>
#include <stdexcept>
#include <cmath>
#include "SolverInterface.hpp"
#include "SolverController.hpp"
#include "solver_ref/RefSolver.hpp"
#include "solver_cuda/CudaSolver.hpp"
#include "ascii_art.hpp"

using namespace std;

namespace {

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
            *dst = sin(a*x + b*y + c*stepNumber);
}

DataFrame stubDataFrame(
        unsigned int width,
        unsigned int height,
        unsigned int stepNumber)
{
    vector<real_type> data(width*height);
    fillStubData(data.data(), width, height, stepNumber);
    return DataFrame(data.data(), width, height);
}

ostream& operator<<(ostream& s, const DataFrame& f)
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
            const DataFrame& fprev,
            const DataFrame& fcur,
            DataFrame& fnext)
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
        RefSolver solver;
//        CudaSolver solver;
        SolverController sc;
//        const auto StepCount = 100;
//        const auto Width = 300u, Height = 300u;
        const auto StepCount = 10;
        const auto Width = 10u, Height = 10u;

        sc.setPrevStep(stubDataFrame(Width, Height, 0));
        sc.setCurrentStep(stubDataFrame(Width, Height, 1));
        auto stepNumber = 0;
        sc.run(solver, [&stepNumber](const DataFrame& f) {
            // cout << "Step " << stepNumber << endl;
            AsciiArt aa(2,f);
            cout << aa << endl;
            return ++stepNumber < StepCount;
        });
        return 0;
    }
    catch(const std::exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }
}
