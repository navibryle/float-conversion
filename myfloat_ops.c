#include "myfloat_ops.h"
#include <limits.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

MyFloat myfloat_add(MyFloat aFloat1, MyFloat aFloat2){
  //both floats should be the same sign otherwise it 
  //would be float substraction
  if (aFloat1.sign != aFloat2.sign){
    if (aFloat2.sign == 1){
      aFloat2.sign = 0;
      return myfloat_sub(aFloat1,aFloat2);
    }
    aFloat2.sign = 1;
    return myfloat_sub(aFloat2,aFloat1);
  }
  MyFloat new;
  new.sign = aFloat1.sign;
  unsigned exp1 = aFloat1.exponent - BIAS;
  unsigned exp2 = aFloat2.exponent - BIAS;
  unsigned trueMant1 = (unsigned) aFloat1.mantissa + IMPLIED_BIT;
  unsigned trueMant2 = (unsigned) aFloat2.mantissa + IMPLIED_BIT;
  int output = INIT;
  if (exp1>exp2){
    int to_move = exp1 - exp2;
    trueMant1 = trueMant1<<to_move;
    output = trueMant1 + trueMant2;
    new.exponent = (256U<<(1+to_move) & output)? (exp1+1)+BIAS:exp1+BIAS;
    while (!((1<<31)&output)){//this push out all the implied bits except for the last one
      output = output<<1;
    }
    output = (output<<1)>>24;//push out the implied bit and shift to the left
    //to get the 8 most significant mantissa bits.
  }else if (exp2>exp1){
    unsigned to_move = exp2 - exp1;
    trueMant2 = trueMant2<<to_move;
    output = trueMant1 + trueMant2;
    new.exponent = (256U<<(1+to_move) & output)? (exp2+1)+BIAS:exp2+BIAS;
    while (!((1<<31)&output)){//this push out all the implied bits except for the last one
      output = output<<1;
    }
    output = (output<<1)>>24;//push out the implied bit and shift to the left
    //to get the 8 most significant mantissa bits.
    
  }else{//exp1 == exp2
    output = trueMant1 + trueMant2;
    new.exponent = (256U<<1 & output)? (exp1+1)+BIAS:exp1+BIAS;
    while (!((1<<31)&output)){//this push out all the implied bits except for the last one
      output = output<<1;
    }
    output = (output<<1)>>24;//push out the implied bit and shift to the left
    //to get the 8 most significant mantissa bits.
  }
  new.mantissa = output;
  return new;
}

MyFloat myfloat_sub(MyFloat aFloat1, MyFloat aFloat2){
   if (aFloat1.sign != aFloat2.sign){
    if (aFloat2.sign == 1){
      aFloat2.sign = 0;
      return myfloat_add(aFloat1,aFloat2);
    }
    aFloat2.sign = 1;
    return myfloat_add(aFloat2,aFloat1);
  }
  MyFloat new;
  unsigned exp1 = aFloat1.exponent - BIAS;
  unsigned exp2 = aFloat2.exponent - BIAS;
  unsigned trueMant1 = (unsigned) aFloat1.mantissa + IMPLIED_BIT;
  unsigned trueMant2 = (unsigned) aFloat2.mantissa + IMPLIED_BIT;
  unsigned output = INIT;
  if (exp1>exp2){
    new.sign = 0;
    int to_move = exp1 - exp2;
    trueMant1 = trueMant1<<to_move;
    output = trueMant1-trueMant2;
    int exp = (int) exp2;
    if (output != 0){
      while (!((1<<31)&output)){//this push out all the implied bits except for the last one
      output = output<<1;
      exp--;
    }
    output = (output<<1)>>24;//push out the last implied bit and shift to the left
    exp = exp +23;
    new.exponent = (unsigned)exp+BIAS;
    }
    //to get the 8 most significant mantissa bits.
  }else if (exp2>exp1){
    new.sign = 1;
    int to_move = exp2 - exp1;
    trueMant2 = trueMant2<<to_move;
    output = trueMant2-trueMant1;
    int exp = (int) exp1;
    if (output != 0){
      while (!((1<<31)&output)){//this push out all the implied bits except for the last one
      output = output<<1;
      exp--;
    }
    output = (output<<1)>>24;//push out the last implied bit and shift to the left
    exp = exp +23;
    new.exponent = (unsigned)exp+BIAS;
    }
  }else{//exp1 == exp2
    new.sign = 0;
    output = trueMant1 - trueMant2;
    int exp = (int) exp1;
    if (output != 0){
      while (!((1<<31)&output)){//this push out all the implied bits except for the last one
      output = output<<1;
      exp--;
    }
    output = (output<<1)>>24;//push out the last implied bit and shift to the left
    exp = exp +23;
    new.exponent = (unsigned)exp+BIAS;
    }else{
      new.exponent = (unsigned) exp;
    }
  }
  new.mantissa = output;
  return new;
}

MyFloat myfloat_mul(MyFloat aFloat1, MyFloat aFloat2){
  MyFloat new;
  int exp =  ((int)aFloat2.exponent - BIAS) +  ((int)aFloat1.exponent - BIAS);
  unsigned output = ((unsigned) aFloat1.mantissa + IMPLIED_BIT)*((unsigned) aFloat2.mantissa + IMPLIED_BIT);
  //shift right 8 times to counter act the fact that multiplication naturally shifts the bits 8 to the left
  output = output>>8;
  while (!((1<<31) & output)){//this push out all the implied bits except for the last onemake
      output = output<<1;
      exp-=1;
    }
  output = (output<<1)>>24;//push out the last implied bit and shift to the left
  exp +=23;
  new.sign = (aFloat1.sign == aFloat2.sign) ? 0:1;
  new.mantissa = output;
  new.exponent = (int)exp+BIAS;
  return new;
}


MyFloat myfloat_inv(MyFloat aFloat){
  MyFloat new;
  new.exponent = BIAS - (aFloat.exponent - BIAS);
  unsigned output = (unsigned) (IMPLIED_BIT)/((unsigned) aFloat.mantissa + IMPLIED_BIT);
  while (!((1<<31) & output)){//this push out all the implied bits except for the last one
      output = output<<1;
    }
  output = (output<<1)>>24;//push out the last implied bit and shift to the left
  new.sign = aFloat.sign;
  new.mantissa = output;
  return new;
}

MyFloat myfloat_div(MyFloat aFloat1, MyFloat aFloat2){
  MyFloat new;
  int exp =  (((int)aFloat1.exponent - BIAS)- ((int)aFloat2.exponent - BIAS)) ;
  int numerator = (aFloat1.mantissa + IMPLIED_BIT)<<9;
  int output = numerator/(aFloat2.mantissa + IMPLIED_BIT);
  if (output != 0){
    while (!((1<<31) & output)){//this push out all the implied bits except for the last one
      output = output<<1;
      exp--;
    }
  output = (output<<1)>>24;//push out the last implied bit and shift to the left
  exp = exp +22;
  }
  new.sign = (aFloat1.sign == aFloat2.sign) ? 0:1;
  new.mantissa = output;
  new.exponent = (int)exp+BIAS;
  return new;
}