#ifndef _MY_FLOAT_
#define _MY_FLOAT_
#include <stdbool.h>

#define SIGN_BITS 1UL
#define EXPONENT_BITS 7UL
#define MANTISSA_BITS 8UL
#define MYFLOAT_BITS 16UL
#define MYFLOAT_BYTES 2
#define BIAS 63
#define INIT 0U
#define IMPLIED_BIT 256

/*  */


typedef struct my_float MyFloat;

struct __attribute__((packed)) my_float {
  short unsigned  mantissa:MANTISSA_BITS;
  short unsigned  exponent:EXPONENT_BITS;
  short unsigned  sign:SIGN_BITS;
};
MyFloat myfloat_zero();

MyFloat myfloat_one();

int myfloat_negative(MyFloat);

int myfloat_exponent(MyFloat);

unsigned myfloat_mantissa(MyFloat);

void myfloat_set_negative(MyFloat *, int);

void myfloat_set_exponent(MyFloat *, int);

void myfloat_set_mantissa(MyFloat *, int);

bool myfloat_equals(MyFloat, MyFloat);
#endif
/*  */
