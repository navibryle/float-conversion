#ifndef _FLOAT_CONV_
#define _FLOAT_CONV_
#include "myfloat.h"
#include "myfloat_ops.h"
#include <stdlib.h>
  typedef union {
  double f;
  long i;
  struct {
    long long unsigned mantisa : 52;
    unsigned exponent : 11;
    unsigned sign : 1;
  } parts;    
} DoublePun64;
  typedef union {
  double f;
  long i;
  struct {
    long unsigned mantisa : 23;
    unsigned exponent : 8;
    unsigned sign : 1;
  } parts;    
} DoublePun32;




MyFloat str_to_myfloat(const char *);
char * myfloat_to_str(MyFloat);
bool check_exponent(const char * aString);
void to_dec(const char * aString,double * aDouble_pointer);
unsigned get_mantissa(double aDouble);
int get_exponent(double aDouble);
unsigned get_sign(double aDouble);
#endif
/*  */
