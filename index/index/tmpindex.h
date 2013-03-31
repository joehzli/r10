//
//  tmpindex.h
//  index
//
//  Created by Hanzhou Li on 3/31/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#ifndef __index__tmpindex__
#define __index__tmpindex__

#include "stdint.h"
#include <string>
#include <vector>
#include "Utility.h"

// Used in temporary index
typedef struct {
    uint32_t docID;
    std::string word;
    uint32_t pos;
    uint16_t context;
} RawPosting;

typedef std::vector<RawPosting *> RawPostingVector;

void GenerateTmpIndex();

/****************************************************
 Merge the generated temporary index files.
 ****************************************************/
void MergeTmpIndex();
int GetPostingFromPage(RawPostingVector *vector, char* page, const char* url, int length, uint32_t docID);
void freeRawPostingVector(RawPostingVector *vector);
void WriteRawPostingToFile(RawPostingVector *vector, const char* filePath, FILEMODE mode);
uint32_t WriteRawPostingToBuffer(char* buffer, RawPostingVector *vector, const char* filePath, FILEMODE mode);
bool CompareRawPostingDocID(RawPosting *posting1, RawPosting *posting2);
bool CompareRawPostingWord(RawPosting *posting1, RawPosting *posting2);
#endif /* defined(__index__tmpindex__) */
