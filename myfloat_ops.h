#ifndef _MY_FLOAT_OPS
#define _MY_FLOAT_OPS
#include "myfloat.h"

MyFloat myfloat_add(MyFloat, MyFloat);

MyFloat myfloat_sub(MyFloat, MyFloat);

MyFloat myfloat_mul(MyFloat, MyFloat);

MyFloat myfloat_inv(MyFloat);

MyFloat myfloat_div(MyFloat, MyFloat);
MyFloat addition_redirect(MyFloat aFloat1, MyFloat aFloat2);
void normalize_float_sub(MyFloat * new,unsigned output ,long int exp);
void normalize_float_add(unsigned *output);
void add_case1(MyFloat aFloat1, MyFloat aFloat2,MyFloat * new);
void add_case2(MyFloat aFloat1, MyFloat aFloat2,MyFloat * new);
void add_case3(MyFloat aFloat1, MyFloat aFloat2,MyFloat * new);
#endif
/*  */
