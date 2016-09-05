// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "../../env/_crtdef.h"
#include "_fpu.h"

static inline long double fpu_asin(long double x){
	return __MCFCRT_fpatan(x, __MCFCRT_fsqrt(1.0l - x * x));
}

float asinf(float x){
	return (float)fpu_asin(x);
}
double asin(double x){
	return (double)fpu_asin(x);
}
long double asinl(long double x){
	return fpu_asin(x);
}
