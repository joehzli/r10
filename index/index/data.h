//
//  data.h
//  index
//
//  Created by Hanzhou Li on 3/2/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#ifndef __index__data__
#define __index__data__

#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include "Utility.h"

#define INDEX_CHUNK 409600 //50KB
#define DATA_CHUNK 20971520 //2.5MB

typedef struct {
    char url[2083]; // max url lenth in IE
    int depth;
    int unknown;
    int length;
    char IP[17];
    int port;
    char status[8];
    
} IndexRecord;

typedef std::vector<IndexRecord *> IndexRecordVector;
/*
 Return the a vector containing the data files' names.
 */
StringVector *GetDataFileList(const char *path);
char *ReadGZFile(const char *filePath);
IndexRecordVector *ParseIndex(char *indexBuf);
CStringVector *GetPages(char* dataBuf, IndexRecordVector* indexArray);


#endif /* defined(__index__data__) */
