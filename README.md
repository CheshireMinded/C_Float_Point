Introduction
Your task for this lab exercise is to complete several function definitions which solve puzzles, using only a limited set of operators in the C language.  In addition, you must write a main() function which calls each of your solution functions to demonstrate that each function performs as specified.

In this exercise, you will write C functions to implement floating-point operations, operating directly on bit-level representations of floating-point numbers.  For this purpose, we define a data type float_bits to be equivalent to an unsigned int:

/* access bit-level representation of a floating-point number */
typedef unsigned float_bits;

Coding Rules
Replace the "return" statement in each function with one or more lines of C code that implements the function. Your code must conform to the following style:

  float_bits funct(arg1, arg2, ...) {

      /* brief description of how your implementation works */

      int var1 = Expr1;

      ...

      int varM = ExprM;

 

      varJ = ExprJ;

      ...

      varN = ExprN;

      return ExprR;

  }

 

Each "Expr" is an expression using ONLY the following:

Integer constants 0 through 255 (0xFF), inclusive. You are not allowed to use big constants such as 0xffffffff.
Function arguments and local variables (no global variables).
Unary integer operations ! ~
Binary integer operations & ^ | + << >>
Each "Expr" may consist of multiple operators. You are not restricted to one operator per line.

You are expressly forbidden to:

Use any control constructs such as if, do, while, for, switch, etc.
Define or use any macros.
Define any additional functions in this file.
Call any functions.
Use any other operations, such as &&, ||, -, or ?:
Use any form of casting.
Use any data type other than int. This implies that you cannot use arrays, structs, or unions.
You may assume that your machine:

Uses 2s complement, 32-bit representations of integers.
Performs right shifts arithmetically.
Has unpredictable behavior when shifting an integer by more than the word size.
The Functions
Write a function to identify the class of floating-point number represented by a float_bits parameter:
int float_class(float_bits f);

This function must return:

a negative integer, if the floating-point number is a “special” (±∞, or NaN).
zero, if the floating-point number is denormalized.
a positive integer, if the floating-point number is normalized.
Write a function to negate a floating-point number.
float_bits float_negate(float_bits f);

Given a parameter f, this function will compute –f.  If f is Nan, the function should simply return f.

Write a function to return the absolute value of a floating-point number.
float_bits float_absval(float_bits f);

Given a parameter f, this function will compute |f|.  If f is Nan, the function should simply return f.

Write a function to double a floating-point number.
float_bits float_twice(float_bits f);

Given a parameter f, this function will compute 2 * f.  If f is Nan, the function should simply return f.

Write a function to halve a floating-point number.
float_bits float_half(float_bits f);

Given a parameter f, this function will compute 0.5 * f.  If f is Nan, the function should simply return f.
