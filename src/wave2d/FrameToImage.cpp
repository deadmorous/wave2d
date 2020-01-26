#include "FrameToImage.hpp"
#include "real_type.hpp"
#include <stdexcept>

#ifdef WAVE2D_USE_QT

#include <QImage>
#include <QColor>

void FrameToImage::save() const
{
    // QImage img(static_cast<int>(m_width), static_cast<int>(m_height), QImage::Format_Grayscale8);
    QImage img(static_cast<int>(m_width), static_cast<int>(m_height), QImage::Format_RGB32);
    auto d = m_data.begin();
    for (auto y=0u; y<m_height; ++y)
        for (auto x=0u; x<m_width; ++x, ++d)
        {
            auto lum = (*d + m_amplitude) / (2*m_amplitude)*make_real(255.999);
            if (lum < 0)
                lum = 0;
            else if (lum >= make_real(256))
                lum = make_real(255);
            auto ulum = static_cast<unsigned int>(lum);
            img.setPixel(static_cast<int>(x), static_cast<int>(y), QColor(ulum, ulum, ulum).toRgb().rgb());
        }
    static unsigned int frameNumber = 0;
    QString number = QString("%1").arg(frameNumber, 5, 10, QChar('0'));
    auto name = QString("images/%1.png").arg(number);
    img.save(name);
    ++frameNumber;
}

#else // WAVE2D_USE_QT

void FrameToImage::save() const
{
    throw std::runtime_error("FrameToImage::save() is not available, consider setting USE_QT=ON in CMake");
}

#endif // WAVE2D_USE_QT
