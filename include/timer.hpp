#pragma once
#include <vector>
#include <chrono>


class Timer
{
private:
    std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
    std::vector<int> v_diff;
public:
    void startTimer()
    {
        startTime=std::chrono::system_clock::now();
    }
    void stopTimer()
    {
        endTime = std::chrono::system_clock::now();
        int diff = std::chrono::duration_cast<std::chrono::nanoseconds> (endTime-startTime).count();
        v_diff.push_back(diff);
    }
    int getDifference()
    {
        int summ = 0;
        for(int i =0; i<v_diff.size();i++)
        {
            summ+= v_diff[i];
        }
        float aver = summ/v_diff.size();
        return aver;
    }

};
