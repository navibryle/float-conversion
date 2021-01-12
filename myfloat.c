#include "myfloat.h"
#include <limits.h>

MyFloat myfloat_zero(){
  return (MyFloat) {.sign = INIT, .exponent = INIT, .mantissa = INIT};
}
MyFloat myfloat_one(){
  return (MyFloat) {.sign = INIT, .exponent = BIAS, .mantissa = INIT};
}
int myfloat_negative(MyFloat aFloat){
  return (int) aFloat.sign;
}
int myfloat_exponent(MyFloat aFloat){
  //most significant bit of the exponent is its sign
  if ((aFloat.exponent) == 127){//2^7 - 1 all bits 7 are on
    return INT_MAX;
  }else if ((aFloat.exponent) == INIT){
    return INT_MIN;
  }
  return ((int) aFloat.exponent)- BIAS;
}
int myfloat_mantissa(MyFloat aFloat){
  //most significant bit of the exponent is its sign
  if ((aFloat.exponent) == INIT || (aFloat.exponent) == 127){//2^7 - 1 all 7 bits are on
    return INIT;
  }
  return IMPLIED_BIT + ((int)aFloat.mantissa);//128 is the implied bit 2^7
}

void myfloat_set_negative(MyFloat * aFloatPoint, int anInt){
  aFloatPoint->sign = (anInt<<31)>>31;
}
void myfloat_set_exponent(MyFloat * aFloatPoint, int anInt){
  if (anInt >= 63){
    aFloatPoint->exponent = (unsigned) 127;
  }else if (anInt <= -63){
    aFloatPoint->exponent = (unsigned) 0;
  }else{
    anInt =(anInt<<25)>>25;//clears out the first 25 bits
    aFloatPoint->exponent = (unsigned)anInt + 63;
  }
}

void myfloat_set_mantissa(MyFloat * aFloatPoint, int anInt){
  aFloatPoint->mantissa = (anInt<<24)>>24;//sets the first 24 bits to 0
}

bool myfloat_equals(MyFloat aFloat1, MyFloat aFloat2){
  return aFloat1.exponent == aFloat2.exponent && \
  aFloat1.sign == aFloat2.sign && aFloat1.mantissa == aFloat2.mantissa;
}
/*  */
