#include <stdio.h>

typedef unsigned float_bits;

/* Function to check if a floating-point number is NaN */

int is_nan(float_bits f) {
    int exp_mask = 0x7F << 24; // 0x7F800000
    int frac_mask = 0xFF | (0xFF << 8) | (0x7F << 16); 
    // 0x007FFFFF
    int exp = f & exp_mask;
    int frac = f & frac_mask;

    // NaN is represented by all exponent bits set to 1 and a non-zero fraction
    int exp_is_max = !(exp ^ exp_mask); 
    // Check if exponent is all 1s
    int frac_is_nonzero = !!frac;       
    // Check if fraction is not zero

    // If both conditions are true, the number is NaN
    return exp_is_max & frac_is_nonzero;
}


/* Classifies the type of a floating-point number */

int float_class(float_bits f) {
    int exp_mask = 0x7F << 24; // 0x7F800000
    int frac_mask = 0xFF | (0xFF << 8) | (0x7F << 16); // 0x007FFFFF
    int exp = f & exp_mask;
    int frac = f & frac_mask;

    // Check for NaN or infinity (all exponent bits are 1)
    int is_special = !(exp ^ exp_mask);
    int is_nan = ((exp == exp_mask) << 31) >> 31 & (frac | -frac);

    // Check for denormalized (all exponent bits are 0)
    int is_denormalized = !(exp | (exp >> 16));

    // Return -1 for special, 0 for denormalized, 1 for normalized
    return (is_nan | is_special) & -1 | is_denormalized & 0 | ~is_nan & ~is_special & ~is_denormalized & 1;
}

/* Negates a floating-point number */

float_bits float_negate(float_bits f) {
    int exp_mask = 0x7F << 24; // 0x7F800000
    int frac_mask = 0xFF | (0xFF << 8) | (0x7F << 16); // 0x007FFFFF
    int sign_mask = 0x80 << 24; // 0x80000000
    int exp = f & exp_mask;
    int frac = f & frac_mask;

    // Check for NaN (exp bits all 1s and frac not 0)
    int is_nan = ((exp == exp_mask) << 31) >> 31 & (frac != 0);

    // Negate the sign bit
    int negated_f = f ^ sign_mask;

    // Use bitwise operations to select the original or negated value
    return is_nan & f | ~is_nan & negated_f;
}



/* Computes the absolute value of a floating-point number */

float_bits float_absval(float_bits f) {
    int exp_mask = 0x7F << 24; // 0x7F800000
    int frac_mask = 0xFF | (0xFF << 8) | (0x7F << 16); // 0x007FFFFF
    int sign_mask = 0x80 << 24; // 0x80000000
    int exp = f & exp_mask;
    int frac = f & frac_mask;

    // Check for NaN
    int is_nan = ((exp == exp_mask) << 31) >> 31 & (frac != 0);

    // Clear the sign bit
    int absval_f = f & ~sign_mask;

    // Use bitwise operations to select the original or absolute value
    return is_nan & f | ~is_nan & absval_f;
}







/* Doubles a floating-point number */

float_bits float_twice(float_bits f) {
    int exp_mask = (0x7F << 16) << 8; // 0x7F800000
    int frac_mask = (0xFF << 8) | 0xFF | (0x7F << 16); // 0x007FFFFF
    int sign_mask = 0x80 << 24; // 0x80000000

    int exp = f & exp_mask;
    int frac = f & frac_mask;
    int sign = f & sign_mask;

    // Check if f is NaN or infinity
    int is_special = !(exp ^ exp_mask);
    int is_zero = !(exp | frac);

    // Increment the exponent for normalized numbers
    int increment = 0x80 << 16; // For incrementing exponent by 1
    int new_exp = exp + ((increment & ~is_special & ~is_zero) | (is_special | is_zero));

    // Handle overflow to infinity
    int overflow = !(new_exp & exp_mask);
    new_exp = (overflow & exp) | (~overflow & new_exp);

    // Handling denormalized numbers
    int new_frac = frac << 1;
    int frac_overflow = new_frac & ((0x80 << 16) << 1); 
    // 0x00800000 using shifts
    new_exp = ((frac_overflow & ~exp) & increment) | (~(frac_overflow & ~exp) & new_exp);
    new_frac = (frac_overflow & (new_frac & frac_mask)) | (~frac_overflow & new_frac);

    // Combine sign, new exponent, and new fraction
    return sign | new_exp | (new_frac & frac_mask);
}





/* Halves a floating-point number */

float_bits float_half(float_bits f) {
    int exp_mask = 0x7F << 24; // 0x7F800000
    int frac_mask = 0xFF | (0xFF << 8) | (0x7F << 16); // 0x007FFFFF
    int sign_mask = 0x80 << 24; // 0x80000000
    int exp = f & exp_mask;
    int frac = f & frac_mask;
    int sign = f & sign_mask;

    // Normalized numbers
    int is_normalized = !((exp == 0) | (exp == exp_mask));
    int new_exp = (exp >> 23) - 1;
    int new_frac = frac | (exp & ~exp_mask);

    // Rounding (round to even)
    int add_one = ((new_frac & 0x3) == 0x3) & is_normalized;
    new_frac = (new_frac >> 1) + add_one;

    // Adjust for underflow into denormalized range
int is_underflow = (new_exp <= 0) & is_normalized;
new_exp = (is_underflow << 23); // Becomes 0 or stays as is
new_frac = ((is_underflow & ((frac >> 1) + add_one)) | (~is_underflow & new_frac)) & frac_mask;

    // Combine sign, exponent, and fraction
    return sign | (new_exp << 23) | (new_frac & frac_mask);
}






void print_binary(float_bits fb) {
    printf("Binary representation: ");
    putchar('0' + ((fb >> 31) & 1)); putchar(' ');
    putchar('0' + ((fb >> 30) & 1)); putchar('0' + ((fb >> 29) & 1)); putchar('0' + ((fb >> 28) & 1)); putchar('0' + ((fb >> 27) & 1)); putchar(' ');
    putchar('0' + ((fb >> 26) & 1)); putchar('0' + ((fb >> 25) & 1)); putchar('0' + ((fb >> 24) & 1)); putchar('0' + ((fb >> 23) & 1)); putchar(' ');
    putchar('0' + ((fb >> 22) & 1)); putchar('0' + ((fb >> 21) & 1)); putchar('0' + ((fb >> 20) & 1)); putchar('0' + ((fb >> 19) & 1)); putchar(' ');
    putchar('0' + ((fb >> 18) & 1)); putchar('0' + ((fb >> 17) & 1)); putchar('0' + ((fb >> 16) & 1)); putchar('0' + ((fb >> 15) & 1)); putchar(' ');
    putchar('0' + ((fb >> 14) & 1)); putchar('0' + ((fb >> 13) & 1)); putchar('0' + ((fb >> 12) & 1)); putchar('0' + ((fb >> 11) & 1)); putchar(' ');
    putchar('0' + ((fb >> 10) & 1)); putchar('0' + ((fb >> 9) & 1));  putchar('0' + ((fb >> 8) & 1));  putchar('0' + ((fb >> 7) & 1));  putchar(' ');
    putchar('0' + ((fb >> 6) & 1));  putchar('0' + ((fb >> 5) & 1));  putchar('0' + ((fb >> 4) & 1));  putchar('0' + ((fb >> 3) & 1));  putchar(' ');
    putchar('0' + ((fb >> 2) & 1));  putchar('0' + ((fb >> 1) & 1));  putchar('0' + (fb & 1));
    putchar('\n');
}

int main() {
    union {
        float input;
        float_bits fb;
    } u;

    printf("Enter a floating-point number: ");
    scanf("%f", &u.input);

    float original_float = u.input;

    printf("Bit-level representation of the entered number: 0x%X\n", u.fb);
    print_binary(u.fb);
    printf("Class: %d\n", float_class(u.fb));
    printf("Negate: 0x%X\n", float_negate(u.fb));
    printf("Absval: 0x%X\n", float_absval(u.fb));
    printf("Twice: 0x%X\n", float_twice(u.fb));
    printf("Half: 0x%X\n", float_half(u.fb));

    return 0;
}


