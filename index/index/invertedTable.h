//
//  invertedTable.h
//  index
//
//  Created by Hanzhou Li on 3/2/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#ifndef __index__invertedTable__
#define __index__invertedTable__

#include <iostream>
#include <string>
#include <vector>

typedef struct {
    uint32_t pos;   // 12-bit is enough
    uint16_t context;   //3-bit is enough
} PostingTuple;

typedef struct {
    uint32_t docID;
    std::string word;
    uint32_t post;
    uint16_t context;
} RawPosting;

typedef struct {
    uint32_t docID; //27-bit is enough
    uint32_t TF;
    uint32_t length;    //5-bit??
    PostingTuple *posArray;
} DocTuple;

typedef std::vector<RawPosting *> RawPostingVector;

#endif /* defined(__index__invertedTable__) */
