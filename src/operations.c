/* operations.c
 *   Written 2016 by Felix Brober and Julia Duong
 *
 *  For copyright and licensing, see file COPYING
 *
 * File containing an integer square-root function
 * and an absolute value function.
 */

#include <stdint.h>   /* Declarations of uint_32 and the like */

/**
 * Fast integer square-root function.
 *
 * See: http://www.codecodex.com/wiki/Calculate_an_integer_square_root
 * Uses figure 2: http://www.embedded.com/electronics-blogs/programmer-s-toolbox/4219659/Integer-Square-Roots
 * by Jack W. Crenshaw (Integer Square Roots).
 *
 * @param n Squared values
 * @return  Square-root of n
 **/
uint32_t isqrt32 (uint32_t n) {
        register uint32_t root, remainder, place;

        root = 0;
        remainder = n;
        place = 0x40000000;

        while (place > remainder)
                place = place >> 2;
        while (place)
        {
                if (remainder >= root + place)
                {
                        remainder = remainder - root - place;
                        root = root + (place << 1);
                }
                root = root >> 1;
                place = place >> 2;
        }
        return root;
}

/**
 * Calculates the absolute value of the given value
 *
 * @param a Input value
 * @return  absolute value of a
 **/
uint32_t absolute(const int a) {
        if(a < 0) return -1*a;
        else return a;
}
