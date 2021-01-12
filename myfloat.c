#include "myfloat.h"
#include <limits.h>
#define BIAS32 127U
#define BITSHIFT31 31U
#define BITSHIFT24 24U
#define BITSHIFT25 25U
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
  if ((aFloat.exponent) == BIAS32){//2^7 - 1 all bits 7 are on
    return INT_MAX;
  }
  if ((aFloat.exponent) == INIT){
    return INT_MIN;
  }
  return (aFloat.exponent)- BIAS;
}
unsigned myfloat_mantissa(MyFloat aFloat){
  //most significant bit of the exponent is its sign
  if ((aFloat.exponent) == INIT || (aFloat.exponent) == BIAS32){//2^7 - 1 all 7 bits are on
    return INIT;
  }
  return IMPLIED_BIT + ((int)aFloat.mantissa);//128 is the implied bit 2^7
}

void myfloat_set_negative(MyFloat * aFloatPoint, int anInt){
  aFloatPoint->sign = ((unsigned) anInt<<BITSHIFT31)>>BITSHIFT31;
}
void myfloat_set_exponent(MyFloat * aFloatPoint, int anInt){
  if (anInt >= BIAS){
    aFloatPoint->exponent = BIAS32;
  }else if (anInt <= -(BIAS)){
    aFloatPoint->exponent =  INIT;
  }else{
    anInt =((unsigned short) anInt<<BITSHIFT25);//clears out the first 25 bits
    aFloatPoint->exponent = ((unsigned)anInt>>BITSHIFT25) + BIAS;
  }
}

void myfloat_set_mantissa(MyFloat * aFloatPoint, int anInt){
  aFloatPoint->mantissa = ((unsigned)anInt<<BITSHIFT24)>>BITSHIFT24;//sets the first 24 bits to 0
}

bool myfloat_equals(MyFloat aFloat1, MyFloat aFloat2){
  return aFloat1.exponent == aFloat2.exponent && \
  aFloat1.sign == aFloat2.sign && aFloat1.mantissa == aFloat2.mantissa;
}
/*  */
