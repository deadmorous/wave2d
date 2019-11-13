#pragma once

#include "DataFrameTemplate.hpp"
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/copy.h>
#include <cstdint>

class CudaSolverDataFrame
{
    using CudaSolverHostDataFrame = DataFrameTemplate<thrust::host_vector<real_type>>;
    using CudaSolverDeviceDataFrame = DataFrameTemplate<thrust::device_vector<real_type>>;

    mutable CudaSolverHostDataFrame m_hostData;
    mutable CudaSolverDeviceDataFrame m_deviceData;
    std::uint64_t m_tsHost = 0;
    std::uint64_t m_tsDevice = 0;
    mutable std::uint64_t m_ts = 0;
    std::uint64_t newTimestamp() const {
        return ++m_ts;
    }

    void syncHost() const {
        if (m_tsHost < m_tsDevice) {
            thrust::copy(m_deviceData.begin(), m_deviceData.end(), m_hostData.begin());
            m_tsHost = m_tsDevice;
        }
    }

    void syncDevice() const {
        if (m_tsDevice < m_tsHost) {
            thrust::copy(m_hostData.begin(), m_hostData.end(), m_deviceData.begin());
            m_tsDevice = m_tsHost;
        }
    }

public:
    CudaSolverDataFrame() :
        m_hostData(m_width, m_height),
        m_deviceData(m_width, m_height)
    {}
    CudaSolverDataFrame(const real_type *data, unsigned int width, unsigned int height) :
        m_hostData(data, width, height),
        m_deviceData(width, height)
    {}
    const real_type *data() const {
        syncHost();
        return m_hostData.data();
    }
    const real_type *cdata() const {
        syncHost();
        return m_hostData.cdata();
    }
    real_type *data() {
        m_tsHost = newTimestamp();
        return m_hostData.data();
    }
    unsigned int size() const {
        return m_hostData.size();
    }
    const real_type *begin() const {
        syncHost();
        return m_hostData.begin();
    }
    real_type *begin() {
        m_tsHost = newTimestamp();
        return m_hostData.begin();
    }
    const real_type *end() const {
        syncHost();
        return m_hostData.end();
    }
    real_type *end() {
        m_tsHost = newTimestamp();
        return m_hostData.end();
    }

    unsigned int width() const {
        return m_width;
    }
    unsigned int height() const {
        return m_height;
    }
    unsigned int indexOf(unsigned int x, unsigned int y) const {
        return x+y*m_width;
    }
    real_type& at(unsigned int x, unsigned int y) {
        m_tsHost = newTimestamp();
        return m_data.at(indexOf(x, y));
    }
    const real_type& at(unsigned int x, unsigned int y) const {
        syncHost();
        return m_data.at(indexOf(x, y));
    }

    CudaSolverDeviceDataFrame& deviceData() {
        m_tsDevice = newTimestamp();
        return m_deviceData;
    };
    const CudaSolverDeviceDataFrame& deviceData() const {
        return m_deviceData;
    };
    const CudaSolverDeviceDataFrame& constDeviceData() const {
        syncDevice();
        return m_deviceData;
    };
};
