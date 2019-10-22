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
            for(unsigned int i; i<fprev.width();i++)
            {
                for(unsigned int j; j<fprev.height();j++)
                {
                    unsigned int i_plus;
                    unsigned int i_minus;
                    unsigned int j_plus;
                    unsigned int j_minus;
                    if (i==fprev.width()-1)
                        i_plus=0;
                    else
                        i_plus=i+1;
                    if (i==0)
                        i_minus=fprev.width()-1;
                    else
                        i_minus=i-1;
                    if (j==fprev.width()-1)
                        j_plus=0;
                    else
                        j_plus=j+1;
                    if (j==0)
                        j_minus=fprev.width()-1;
                    else
                       j_minus=j-1;
                    real_type gamma = modelParameters.getWaveSpeed()*
                                   solverParameters.getTimeStepLength()/solverParameters.getSpatialStepLength();
                    real_type lapl = fcur.at(i_plus,j)+fcur.at(i,j_plus)+fcur.at(i_minus,j)+fcur.at(i,j_minus)-4*fcur.at(i,j);
                    real_type u=2*fcur.at(i,j)-fprev.at(i,j)+gamma*gamma*lapl;

                }
            }
    }
};
