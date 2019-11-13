#pragma once
#include <iostream>
#include <vector>
#include "real_type.hpp"


template<class DataFrame>
class AsciiArt
{
private:
    std::vector<std::string> m_palette = {" ",".",",","-",":",";","|","C","O","B"};
    unsigned int m_width;
    unsigned int m_height;
    real_type m_amplitude;
    std::vector<std::string> canvas;
public:
    AsciiArt()
    {

    }

    AsciiArt(real_type amplitude)
    {
        setAmplitude(amplitude);
    }
    AsciiArt(real_type amplitude,const DataFrame&frame)
    {
        setAmplitude(amplitude);
        generateCanvas(frame);
    }
    void setAmplitude(real_type amplitude)
    {
        m_amplitude=amplitude;
    }
    real_type getAmplitude()
    {
        return m_amplitude;
    }

    void setPalette(std::vector<std::string> palette)
    {
        m_palette=palette;
    }
    std::vector<std::string> getPalette()
    {
        return m_palette;
    }

    void generateCanvas(const DataFrame& frame)
    {
        m_width=frame.width();
        m_height=frame.height();
        for(auto j=0u; j<frame.height();j++)
        {
            for(auto i=0u; i<frame.width();i++)
            {
                auto k=1;
                bool h = true;
                while (h)
                {
                    if (frame.at(i,j)<-m_amplitude+k*2*m_amplitude/m_palette.size())
                    {
                        canvas.push_back(m_palette[k-1]);
                        h=false;
                    }
                    k+=1;
                }
            }
        }
    }
    template<class DataFrame2>
    friend
    std::ostream& operator << (std::ostream &out, const AsciiArt<DataFrame2>& aa);
};

template<class DataFrame>
inline std::ostream& operator << (std::ostream &out, const AsciiArt<DataFrame>& aa)
{
    for(auto j=0u; j<aa.m_height;j++)
    {
        for(auto i=0u; i<aa.m_width;i++)
        {
            out<<aa.canvas[i+j*aa.m_width];
        }
        out<<std::endl;
    }

    return out;
}
