#include "SolverInterface.hpp"

using namespace std;

void SolverInterface::setPrevStep(vector<vector<float>> value_PrevStep)
{
    PrevStep=value_PrevStep;
}
void SolverInterface::setCurrentStep(vector<vector<float>> value_CurrentStep)
{
    CurrentStep=value_CurrentStep;
}
void SolverInterface::setCoord_step_length(float value_coord_step_length)
{
    coord_step_length=value_coord_step_length;
}
void SolverInterface::setTime_step_length(float value_time_step_length)
{
    time_step_length=value_time_step_length;
}
void SolverInterface::setWavespeed(float value_wavespeed)
{
    wavespeed=value_wavespeed;
}
vector<vector<float>> SolverInterface::getPrevStep()
{
    return PrevStep;
}
vector<vector<float>> SolverInterface::getCurrentStep()
{
    return CurrentStep;
}
float SolverInterface::getCoord_step_length()
{
    return coord_step_length;
}
float SolverInterface::getTime_step_length()
{
    return time_step_length;
}
float SolverInterface::getWavespeed()
{
    return wavespeed;
}
void SolverInterface::make_step()
{
    vector<vector<float>> NextStep;
    for(int i; i<PrevStep.size();i++)
    {
        for(int j; j<PrevStep[0].size();j++)
        {
            float gamma = (wavespeed*time_step_length/coord_step_length)^2;
            float lapl = CurrentStep[i+1][j]+CurrentStep[i][j+1]+CurrentStep[i-1][j]+CurrentStep[i][j-1]-4*CurrentStep[i][j];
            u=2*CurrentStep[i][j]-PrevStep[i][j]+gamma*lapl;
            NextStep[i].push_back(u);
        }
    }
    PrevStep=CurrentStep;
    CurrentStep=NextStep;
}
void SolverInterface::make_calculations(int number)
{
    for(int i; i<number;i++)
    {
        make_step();
    }
}
