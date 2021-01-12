#include "myfloat_ops.h"
#include <inttypes.h>

#include <limits.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define ONE 1U
#define EXPONENTBIT 256U
#define BITSHIFT24 24U
#define BITSHIFT31 31U
#define EXPONENT22 22U
#define EXPONENT23 23U
#define BITSHIFT8 8U
#define BITSHIFT9 9U
MyFloat addition_redirect(MyFloat aFloat1, MyFloat aFloat2){
  if (aFloat2.sign == ONE){
      aFloat2.sign = INIT;
      return myfloat_sub(aFloat1,aFloat2);
    }
    aFloat2.sign = ONE;
    return myfloat_sub(aFloat2,aFloat1);
}

MyFloat myfloat_add(MyFloat aFloat1, MyFloat aFloat2){
  //both floats should be the same sign otherwise it 
  //would be float substraction
  if (aFloat1.sign != aFloat2.sign){
    return addition_redirect(aFloat1,aFloat2);
  }
  MyFloat new;
  new.sign = aFloat1.sign;
  unsigned exp1 = aFloat1.exponent - BIAS;
  unsigned exp2 = aFloat2.exponent - BIAS;
  short unsigned trueMant1 =  aFloat1.mantissa + IMPLIED_BIT;
  short unsigned trueMant2 =  aFloat2.mantissa + IMPLIED_BIT;
  unsigned output = INIT;
  if (exp1>exp2){
    add_case1(aFloat1,aFloat2,&new);
  }else if (exp2>exp1){
    add_case2(aFloat1,aFloat2,&new);
  }else{//exp1 == exp2
    output = trueMant1 + trueMant2;
    int is_active = EXPONENTBIT<<ONE & output;
    new.exponent = (is_active)? (exp1+ONE)+BIAS:exp1+BIAS;
    normalize_float_add(&output);
    new.mantissa = output;
  }
  
  return new;
}
void add_case1(MyFloat aFloat1, MyFloat aFloat2,MyFloat * new){
  new->sign = aFloat1.sign;
  unsigned exp1 = aFloat1.exponent - BIAS;
  unsigned exp2 = aFloat2.exponent - BIAS;
  short unsigned trueMant1 =  aFloat1.mantissa + IMPLIED_BIT;
  short unsigned trueMant2 =  aFloat2.mantissa + IMPLIED_BIT;
  unsigned output = INIT;
  unsigned to_move = exp1 - exp2;
  trueMant1 = trueMant1<<to_move;
  output = trueMant1 + trueMant2;
  int is_active = EXPONENTBIT<<(ONE+to_move) & output;
  new->exponent = (is_active)? (exp1+ONE)+BIAS:exp1+BIAS;
  normalize_float_add(&output);
  new->mantissa = output;
}
void add_case2(MyFloat aFloat1, MyFloat aFloat2,MyFloat * new){
  new->sign = aFloat1.sign;
  unsigned exp1 = aFloat1.exponent - BIAS;
  unsigned exp2 = aFloat2.exponent - BIAS;
  short unsigned trueMant1 =  aFloat1.mantissa + IMPLIED_BIT;
  short unsigned trueMant2 =  aFloat2.mantissa + IMPLIED_BIT;
  unsigned output = INIT;
  unsigned to_move = exp2 - exp1;
  trueMant2 = trueMant2<<to_move;
  output = trueMant1 + trueMant2;
  int is_active = EXPONENTBIT<<(ONE+to_move) & output;
  new->exponent = (is_active)? (exp2+ONE)+BIAS:exp2+BIAS;
  normalize_float_add(&output);
  new->mantissa = output;
}
void normalize_float_add(unsigned *output){
  int is_active = !((ONE<<BITSHIFT31)&*output);
  while (is_active){//this push out all the implied bits except for the last one
      *output = *output<<ONE;
      is_active = !((ONE<<BITSHIFT31)&*output);
    }
    *output = (*output<<ONE)>>BITSHIFT24;//push out the implied bit and shift to the left
}
MyFloat myfloat_sub_redirect(MyFloat aFloat1, MyFloat aFloat2){
  if (aFloat2.sign == ONE){
      aFloat2.sign = INIT;
      return myfloat_add(aFloat1,aFloat2);
    }
    aFloat2.sign = ONE;
    return myfloat_add(aFloat2,aFloat1);
}

MyFloat myfloat_sub(MyFloat aFloat1, MyFloat aFloat2){
   if (aFloat1.sign != aFloat2.sign){
    return myfloat_sub_redirect(aFloat1,aFloat2);
  }
  MyFloat new;
  short unsigned exp1 = aFloat1.exponent - BIAS;
  short unsigned exp2 = aFloat2.exponent - BIAS;
  short unsigned trueMant1 = aFloat1.mantissa + IMPLIED_BIT;
  short unsigned trueMant2 = aFloat2.mantissa + IMPLIED_BIT;
  unsigned output = INIT;
  if (exp1>exp2){
    new.sign = INIT;
    unsigned to_move = exp1 - exp2;
    trueMant1 = trueMant1<<to_move;
    output = trueMant1-trueMant2;
    long int exp = exp2;
    normalize_float_sub(&new,output,exp);
    //to get the 8 most significant mantissa bits.
  }else if (exp2>exp1){
    new.sign = ONE;
    unsigned to_move = exp2 - exp1;
    trueMant2 = trueMant2<<to_move;
    output = trueMant2-trueMant1;
    long int exp =  exp1;
    normalize_float_sub(&new,output,exp);
  }else{//exp1 == exp2
    new.sign = INIT;
    output = trueMant1 - trueMant2;
    long int exp =  exp1;
    normalize_float_sub(&new,output,exp);
  } 
  return new;
}
void normalize_float_sub(MyFloat * new,unsigned output ,long int exp){
  unsigned new_output = output;
  long int new_exp = exp;
  if (new_output == INIT){
      new->exponent = (unsigned) new_exp;
    }else{
      int is_active = !((ONE<<BITSHIFT31)&new_output);
      while (is_active){//this push out 
      //all the implied bits except for the last one
      new_output = new_output<<ONE;
      new_exp--;
      is_active = !((ONE<<BITSHIFT31)&new_output);
      }
    new_output = (new_output<<ONE)>>BITSHIFT24;//push out the last implied bit and shift to the left
    new_exp = new_exp +EXPONENT23;
    new->exponent = (unsigned)new_exp+BIAS;
      }
    new->mantissa = new_output;
  }


MyFloat myfloat_mul(MyFloat aFloat1, MyFloat aFloat2){
  MyFloat new;
  long int exp =  ((int)aFloat2.exponent - BIAS) +  \
  ((int)aFloat1.exponent - BIAS);
  unsigned output = ((unsigned) aFloat1.mantissa + \
  IMPLIED_BIT)*((unsigned) aFloat2.mantissa + IMPLIED_BIT);
  //shift right 8 times to counter act the fact that
  // multiplication naturally shifts the bits 8 to the left
  output = output>>BITSHIFT8;
  int is_active = !((ONE<<BITSHIFT31) & output);
  while (is_active){
    //this push out all the implied bits except for the last onemake
      output = output<<ONE;
      exp-=ONE;
      is_active = !((ONE<<BITSHIFT31) & output);
    }
  output = (output<<ONE)>>BITSHIFT24;//push out the last implied bit and shift to the left
  exp +=EXPONENT23;
  new.sign = (aFloat1.sign == aFloat2.sign) ? INIT:ONE;
  new.mantissa = output;
  new.exponent = (int)exp+BIAS;
  return new;
}


MyFloat myfloat_inv(MyFloat aFloat){
  MyFloat new;
  new.exponent = BIAS - (aFloat.exponent - BIAS);
  unsigned output = (unsigned) (IMPLIED_BIT)/((unsigned) aFloat.mantissa + IMPLIED_BIT);
  int is_active = !((ONE<<BITSHIFT31) & output);
  while (is_active){//this push out all the implied bits except for the last one
      output = output<<ONE;
      is_active = !((ONE<<BITSHIFT31) & output);
    }
  output = (output<<ONE)>>BITSHIFT24;//push out the last implied bit and shift to the left
  new.sign = aFloat.sign;
  new.mantissa = output;
  return new;
}

MyFloat myfloat_div(MyFloat aFloat1, MyFloat aFloat2){
  MyFloat new;
  long int exp =  (((int)aFloat1.exponent - BIAS)- ((int)aFloat2.exponent - BIAS)) ;
  long int numerator = (aFloat1.mantissa + (unsigned) IMPLIED_BIT)<<BITSHIFT9;
  long unsigned output = numerator/(aFloat2.mantissa + IMPLIED_BIT);
  if (output != INIT){
    int is_active = !((ONE<<BITSHIFT31) & output);
    while (is_active){
      //this push out all the implied bits except for the last one
      output = output<<ONE;
      exp--;
      is_active = !((ONE<<BITSHIFT31) & output);
    }
  output = (output<<ONE)>>BITSHIFT24;//push out the last implied bit and shift to the left
  exp = exp +EXPONENT22;
  }
  new.sign = (aFloat1.sign == aFloat2.sign) ? INIT:ONE;
  new.mantissa = output;
  new.exponent = (int)exp+BIAS;
  return new;
}
