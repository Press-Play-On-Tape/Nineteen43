#pragma once 
 
#include "../FixedPoints.h" 
 
BEGIN_FIXED_POINTS
using UQ4x4 = UFixedPoint<4, 4>;
using UQ8x8 = UFixedPoint<8, 8>;
using UQ16x16 = UFixedPoint<16, 16>;
using UQ32x32 = UFixedPoint<32, 32>;

using UQ1x7 = UFixedPoint<1, 7>;
using UQ1x15 = UFixedPoint<1, 15>;
using UQ1x31 = UFixedPoint<1, 31>;
using UQ1x63 = UFixedPoint<1, 63>;
END_FIXED_POINTS