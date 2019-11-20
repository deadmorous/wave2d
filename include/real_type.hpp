#pragma once

using real_type = WAVE2D_REAL_TYPE;

template <class T>
inline real_type make_real(const T& x) {
    return static_cast<real_type>(x);
}
