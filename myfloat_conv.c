#include "myfloat_conv.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define BUFFSIZE 1024U
#define ONE 1U
#define TEN 10U
#define DOUBLEBIAS 1023U
#define BITSHIFT44 44U
#define BITSHIFT15 15U
#define FLOATBIAS 127U
#define FIVE 5 
#define FOUR 4
#define THREE 3
#define TWO 2
//============================str-float======================================
bool check_exponent(const char * aString){
  for (int i = INIT;aString[i] != '\0';i++){
    if (aString[i] == 'e'){
      return true;
    }
  }
  return false;
}
void to_dec(const char * aString,double * aDouble_pointer){
  char temp_string_fraction[BUFFSIZE] = {'\0'};
  char temp_string_exponent[BUFFSIZE] = {'\0'};
  int string_index = INIT;
  double exponent = INIT;
  for (int i = INIT; aString[i] != 'e';i++,string_index++){
    temp_string_fraction[i] = aString[i];
  }
  string_index += ONE;//this will be the index of the first character of the exponent
  sscanf(temp_string_fraction,"%lf",aDouble_pointer);
  for (int j = string_index,k = INIT; aString[j]!= '\0';j++,k++){
    temp_string_exponent[k] = aString[j];
  }
  exponent = pow(TEN,atoi(temp_string_exponent));
  //printf("the exponent on conversion: %d\n",exponent);
  *aDouble_pointer *= exponent;
}
MyFloat to64(double aDouble){
    MyFloat new;
    DoublePun64 the_double;
    the_double.f = aDouble;
    new.exponent = the_double.parts.exponent-DOUBLEBIAS+BIAS;
    new.mantissa = the_double.parts.mantisa>>BITSHIFT44;
    new.sign = the_double.parts.sign;
    return new;
}
MyFloat str_to_myfloat(const char * aString){
  MyFloat output;
  if (strcmp(aString,"-inf")==INIT){
    output =  (MyFloat) {.sign = ONE, .mantissa = INIT, .exponent = FLOATBIAS};
  }else if (strcmp(aString,"inf") == INIT){
    output = (MyFloat) {.sign = INIT, .mantissa = INIT, .exponent = FLOATBIAS};
  }else if (strcmp(aString,"-0") == INIT){
    output = (MyFloat) {.sign = ONE, .mantissa = INIT, .exponent = INIT};
  }else if (strcmp(aString,"0") == INIT){
    output = (MyFloat) {.sign = INIT, .mantissa = INIT, .exponent = INIT};
  }else{
    /*
    check if number is in scientific notation
    if its not in scientific notation turn the number in a double 
    and transfer the corresponding bits
    mantisa of a double precision number is 52 bits and 
    exponent is 11 bit sign is 1 bit. if mantisa.
    if its in scientific notion turn it into a normal number.
    */
    double aDouble = 0;
    char new_string[BUFFSIZE] = {'\0'};
    strncpy(new_string,aString,strlen(aString)+1);
    if (check_exponent(aString)){
      to_dec(new_string,&aDouble);
    }else{
      sscanf(new_string,"%lf",&aDouble);
    }

      output = to64(aDouble);
  }
  return output;
}
//============================str-float======================================
//============================float-str======================================
DoublePun64 from64(MyFloat aFloat){
  DoublePun64 the_double;
  the_double.parts.exponent = aFloat.exponent-BIAS+DOUBLEBIAS;
  the_double.parts.mantisa = ((unsigned long long) aFloat.mantissa)<<BITSHIFT44;

  the_double.parts.sign = aFloat.sign;
  return the_double;
}
DoublePun32 from32(MyFloat aFloat){
  DoublePun32 the_double;
  the_double.parts.exponent = aFloat.exponent-BIAS+FLOATBIAS;
  the_double.parts.mantisa = ((unsigned long) aFloat.mantissa)<<BITSHIFT15;
  the_double.parts.sign = aFloat.sign;
  return the_double;
}
char * myfloat_to_str(MyFloat aFloat){
  /*
    typedef DoublePun32 DoublePun;
  if (sizeof(double)==8){
    
  }*/
  char *string_rep = calloc(BUFFSIZE,sizeof(char));
  if (aFloat.exponent == FLOATBIAS && aFloat.sign == ONE){
    strncpy(string_rep,"-inf",FIVE);
  }else if (aFloat.exponent == FLOATBIAS && aFloat.sign == INIT){
    strncpy(string_rep,"inf",FOUR);
  }else if (aFloat.exponent == INIT && aFloat.sign == ONE && aFloat.mantissa == INIT){
    strncpy(string_rep,"-0",THREE);
  }else{
    myfloat_to_str_cont(aFloat,string_rep);
  }
  return string_rep;
}
void myfloat_to_str_cont(MyFloat aFloat,char * string_rep){
  if (aFloat.exponent == INIT && aFloat.sign == INIT && aFloat.mantissa == INIT){
    strncpy(string_rep,"0",TWO);
  }else{
    sprintf(string_rep,"%.2e",from64(aFloat).f);
  }
}
//============================float-str======================================

