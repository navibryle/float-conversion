#include "myfloat_conv.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
//============================str-float======================================
bool check_exponent(const char * aString){
  for (int i = 0;aString[i] != '\0';i++){
    if (aString[i] == 'e'){
      return true;
    }
  }
  return false;
}
void to_dec(const char * aString,double * aDouble_pointer){
  char temp_string_fraction[1024] = {'\0'};
  char temp_string_exponent[1024] = {'\0'};
  int string_index = 0;
  double exponent = 0;
  for (int i = 0; aString[i] != 'e';i++,string_index++){
    temp_string_fraction[i] = aString[i];
  }
  string_index += 1;//this will be the index of the first character of the exponent
  sscanf(temp_string_fraction,"%lf",aDouble_pointer);
  for (int j = string_index,k = 0; aString[j]!= '\0';j++,k++){
    temp_string_exponent[k] = aString[j];
  }
  exponent = pow(10,atoi(temp_string_exponent));
  //printf("the exponent on conversion: %d\n",exponent);
  *aDouble_pointer *= exponent;
}
MyFloat to64(double aDouble){
    MyFloat new;
    DoublePun64 the_double;
    the_double.f = aDouble;
    new.exponent = the_double.parts.exponent-1023+BIAS;
    new.mantissa = the_double.parts.mantisa>>44;
    new.sign = the_double.parts.sign;
    return new;
}
MyFloat to32(double aDouble){
    MyFloat new;
    DoublePun32 the_double;
    the_double.f = aDouble;
    new.exponent = the_double.parts.exponent-127+BIAS;
    new.mantissa = the_double.parts.mantisa>>15;
    new.sign = the_double.parts.sign;
    return new;
}
MyFloat str_to_myfloat(const char * aString){
  if (strcmp(aString,"-inf")==0){
    return (MyFloat) {.sign = 1, .mantissa = 0, .exponent = 127};
  }else if (strcmp(aString,"inf") == 0){
    return (MyFloat) {.sign = 0, .mantissa = 0, .exponent = 127};
  }else if (strcmp(aString,"-0") == 0){
    return (MyFloat) {.sign = 1, .mantissa = 0, .exponent = 0};
  }else if (strcmp(aString,"0") == 0){
    return (MyFloat) {.sign = 0, .mantissa = 0, .exponent = 0};
  }else{
    /*
    check if number is in scientific notation
    if its not in scientific notation turn the number in a double and transfer the corresponding bits
    mantisa of a double precision number is 52 bits and exponent is 11 bit sign is 1 bit. if mantisa.
    if its in scientific notion turn it into a normal number.
    */
    double aDouble = 0;
    char new_string[1024] = {'\0'};
    strcpy(new_string,aString);
    if (check_exponent(aString)){
      to_dec(new_string,&aDouble);
    }else{
      sscanf(new_string,"%lf",&aDouble);
    }

      return to64(aDouble);
  }
}
//============================str-float======================================
//============================float-str======================================
DoublePun64 from64(MyFloat aFloat){
  DoublePun64 the_double;
  the_double.parts.exponent = aFloat.exponent-BIAS+1023;
  the_double.parts.mantisa = ((unsigned long long) aFloat.mantissa)<<44;

  the_double.parts.sign = aFloat.sign;
  return the_double;
}
DoublePun32 from32(MyFloat aFloat){
  DoublePun32 the_double;
  the_double.parts.exponent = aFloat.exponent-BIAS+127;
  the_double.parts.mantisa = ((unsigned long) aFloat.mantissa)<<15;
  the_double.parts.sign = aFloat.sign;
  return the_double;
}
char * myfloat_to_str(MyFloat aFloat){
  /*
    typedef DoublePun32 DoublePun;
  if (sizeof(double)==8){
    
  }*/
  char *string_rep = calloc(1024,sizeof(char));
  if (aFloat.exponent == 127 && aFloat.sign == 1){
    strcpy(string_rep,"-inf");
  }else if (aFloat.exponent == 127 && aFloat.sign == 0){
    strcpy(string_rep,"inf");
  }else if (aFloat.exponent == 0 && aFloat.sign == 1 && aFloat.mantissa == 0){
    strcpy(string_rep,"-0");
  }else if (aFloat.exponent == 0 && aFloat.sign == 0 && aFloat.mantissa == 0){
    strcpy(string_rep,"0");
  }else{
      sprintf(string_rep,"%.2e",from64(aFloat).f);
  }
  return string_rep;
}
//============================float-str======================================

