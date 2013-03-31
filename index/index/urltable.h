//
//  urltable.h
//  index
//
//  Created by Hanzhou Li on 3/2/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#ifndef __index__urltable__
#define __index__urltable__

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "Utility.h"
#include "GlobalConfig.h"

// Header is docID(4 bytes) + fileID(2 bytes) + startIndex(4 bytes) + pageRank(2 bytes) + urlLen(2 bytes)
static const int uHeaderSize = 4 + 2 + 4 + 2 + 2;
static const int uBlockSize = 32768;

typedef struct{
    uint32_t docID; // doc id
    uint32_t fileID; // the number of data file containing this page
    uint32_t startIndex;   // pointer to the start point in the data file
    uint32_t dl;    //number of words
    std::string url; // url
} URLItem;

typedef std::vector<URLItem*> URLVector;

class URLTable
{
private:
    uint32_t _totalURL;
    URLVector *_urlTable;
public:
    URLTable();
    ~URLTable();
    void Add(URLItem* urlitem);
    void Write(FILEMODE mode);
};

#endif /* defined(__index__urltable__) */
