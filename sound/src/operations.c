#include <stdint.h>   /* Declarations of uint_32 and the like */

char findmaxAvg(int *av){
        char temp = 0;
        int maxval = av[0];
        char curr;
        for (curr = 1; curr < 8; curr++) {
                if (av[curr] > maxval) temp = curr;
        }
        return temp;
}

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


int absolute(int val) {
        if (val < 0) return (-1)*val;
        else return val;
}


void convertit(int value, char *s) {
        if(value < 0) value *= -1;
        int i;
        int other = 1;
        for (i = 7; i >= 0; i--) {
                s[i] = (value / other) % 10 + 48;
                other *= 10;
        }
}

void calcAverages(char size, int *av, int *re) {
        if(size > 16) return;
        int j;
        av[0] = 0;
        for(j = 1; j < size; j++) {
                av[0] += re[j];
        }
        av[0] /= (size - 1);
        int i;

        for(i = 1; i < 8; i++) {
                av[i] = 0;
                for (j = 0; j < size; j++) {
                        av[i] += re[i*size + j];
                }
                av[i] /= size;
        }
}
