//
//  gzfile.cpp
//  index
//
//  Created by Hanzhou Li on 3/31/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#include "gzfile.h"
#include "stdlib.h"
#include "GlobalConfig.h"
#include <zlib.h>

char *ReadGZFile(const char *filePath)
{
    gzFile gzfile = gzopen(filePath, "r");
    int size = INDEX_CHUNK;
    char *buffer=(char *)malloc(size);
    int oldSize=size;
    
    int count=0;             //The number of bytes that already read
    while (!gzeof(gzfile))
    {
        count+=gzread(gzfile,buffer+count,oldSize);
        if (count==size)                    // Reallocate when buffer is full
        {
            oldSize=size;
            size*=2;
            buffer=(char *)realloc(buffer,size);
        }
    }
    buffer[count] = '\0';
    gzclose(gzfile);
    return buffer;
    
}