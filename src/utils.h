
#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>

#include "memory.h"
#include "exception.h"
#include "buffer.h"
#include "netfilter.h"

long get_time();
long get_time_sec();
const char *ltime();

#endif /* UTILS_H */
