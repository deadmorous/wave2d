#include "real_type.hpp"
#include "DataFrame.hpp"
#include "ModelParameters.hpp"
#include "SolverParameters.hpp"
#include "SolverInterface.hpp"

class RefSolver : public SolverInterface
{
public:
    void makeStep(
            const ModelParameters& modelParameters,
            const SolverParameters& solverParameters,
            const DataFrame& fprev,
            const DataFrame& fcur,
            DataFrame& fnext)
    {
        auto dst = fnext.data();
        for(auto j=0u; j<fprev.height();j++)
        {
            for(auto i=0u; i<fprev.width();i++)
            {
                auto i_plus = i==fprev.width()-1? 0: i+1;
                auto i_minus = i==0? fprev.width()-1: i-1;
                auto j_plus = j==fprev.width()-1? 0: j+1;
                auto j_minus = j==0? fprev.width()-1: j-1;
                real_type gamma = modelParameters.getWaveSpeed()*
                               solverParameters.getTimeStepLength()/solverParameters.getSpatialStepLength();
                real_type lapl = fcur.at(i_plus,j)+fcur.at(i,j_plus)+fcur.at(i_minus,j)+fcur.at(i,j_minus)-4*fcur.at(i,j);
                real_type u=2*fcur.at(i,j)-fprev.at(i,j)+gamma*gamma*lapl;
                *dst++ = u;
            }
        }
    }
};
