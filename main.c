#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

// From https://stackoverflow.com/a/3437484
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

typedef enum invocation_type {
    NTIMES,
    NHOURS,
    INVALID
} invocation_type;

long calculate_lambda(long s) {
    long lambda = 0;
    if(s <= 0) {
        lambda = floor(abs((double)s/2.0)) + 1;
    }
    return lambda;
}

void train_n_times(long int s, long int n) {
    long lambda = calculate_lambda(s);
    printf("%ld\n", n + (n > lambda)*(n-lambda)*(n + s + lambda - 2));
}

long calculate_train_n_hours(long int s, long int h) {
    long n = -1;
    if(s < 1) {
        long lambda = calculate_lambda(s);
        long h_minus = min(h, lambda);
        long h_plus = max(0, h - lambda);
        long n_minus = h_minus;
        long s_plus = s + 2*n_minus;
        long n_plus = 0;
        if(h_plus > 0) {
             n_plus = calculate_train_n_hours(s_plus, h_plus);
        }
        n = n_minus + n_plus;
    } else if(s == 1) {
        n = floor(sqrt((long double)h/s));
    } else { // s > 1
        n = floor((-1.0 + sqrt(1 + 4.0*h/(powl(s-1.0, 2.0))))*(s-1.0)/2.0);
    }
    return n;
}

void train_n_hours(long int s, long int h) {
    long n = calculate_train_n_hours(s, h);
    printf("%ld\n", n);
}

int main(int argc, char** argv) {
    if(argc != 4) {
        fprintf(stderr, "Usage: rqt <current skill percentage> <number> <times|hours>\n");
        exit(1);
    }
    char* arg_iter;
    int minus_found = 0;
    if(*argv[1] == '-') {
        minus_found = 1;
    }
    for(arg_iter = argv[1] + minus_found; *arg_iter != '\0'; ++arg_iter) {
        if(!isdigit(*arg_iter)) {
            fprintf(stderr, "Current skill percentage must be an integer.\n");
            exit(1);
        }
    }
    long s = strtol(argv[1], NULL, 10);
    if((errno == ERANGE && (s == LONG_MAX || s == LONG_MIN))
        || (errno != 0 && s == 0)) {
        printf("Error %d (%s) converting string '%s' to number.\n", errno, strerror(errno), argv[1]);
        return 1;
    }

    for(arg_iter = argv[2]; *arg_iter != '\0'; ++arg_iter) {
        if(!isdigit(*arg_iter)) {
            fprintf(stderr, "The second argument must be a non-negative integer, either number of times to train, or number of hours.\n");
            exit(1);
        }
    }
    long n = strtol(argv[2], NULL, 10);
    if((errno == ERANGE && (n == LONG_MAX || n == LONG_MIN))
        || (errno != 0 && n == 0)) {
        printf("Error %d (%s) converting string '%s' to number.\n", errno, strerror(errno), argv[2]);
        return 1;
    }

    invocation_type t = INVALID;
    if(strncmp(argv[3], "times", 8) == 0) {
        t = NTIMES;
    } else if(strncmp(argv[3], "hours", 8) == 0) {
        t = NHOURS;
    }
    if(t == INVALID) {
    }

    switch(t) {
        case NTIMES:
            {
                train_n_times(s, n);
            }
            break;
        case NHOURS:
            {
                train_n_hours(s, n);
            }
            break;
        case INVALID:
            {
                fprintf(stderr, "The third argument needs to be either 'times' or 'hours'.\n");
                exit(1);
            }
            break;
    }
    return 0;
}
