#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "vbyte.h"
#include <iostream>

using namespace std;
/*
int main(int argc, char **argv)
{
    uint32_t n = (uint32_t)4294967295;
    uint32_t *np = &n;
    char buf[5];
    char *p = buf;
    ZVBCODE(np, p);
    cout<<(int)(p-buf)<<" "<<n<<endl;
    
    n = 0;
    np = &n;
    int x = 0;
    p = buf;
    UZVBCODE(p, x, np);
    cout<<(int)(p-buf)<<" "<<x<<" "<<n<<endl;
    return 0;
}
*/