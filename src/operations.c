/* operations.c
   Written 2016 by Felix Brober and Julia Duong

   For copyright and licensing, see file COPYING
 */

#include <stdint.h>   /* Declarations of uint_32 and the like */

unsigned int isqrt(unsigned int a) {
        unsigned int rem = 0;
        int root = 0;
        int i;

        for (i = 0; i < 16; i++) {
                root <<= 1;
                rem <<= 2;
                rem += a >> 30;
                a <<= 2;

                if (root < rem) {
                        root++;
                        rem -= root;
                        root++;
                }
        }

        return (unsigned int) (root >> 1);
}
