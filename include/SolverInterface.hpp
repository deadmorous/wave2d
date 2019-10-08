#pragma once
#include <vector>

using namespace std;

class SolverInterface{
private:
    vector<vector<float>> PrevStep;
    vector<vector<float>> CurrentStep;
    float coord_step_length;
    float time_step_length;
    float wavespeed;
public:
    void setPrevStep(vector<vector<float>> value_PrevStep);
    void setCurrentStep(vector<vector<float>> value_CurrentStep);
    void setCoord_step_length(float value_coord_step_length);
    void setTime_step_length(float value_time_step_length);
    void setWavespeed(float value_wavespeed);
    vector<vector<float>> getPrevStep();
    vector<vector<float>> getCurrentStep();
    float getCoord_step_length();
    float getTime_step_length();
    float getWavespeed();
    void make_step();
    void make_calculations(int number);
};
