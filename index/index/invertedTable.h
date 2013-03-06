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
#include "Utility.h"

#define MAX_FILE_SIZE 4*1000*1000 // about 4MB

typedef struct {
    uint32_t pos;   // 12-bit is enough
    uint16_t context;   //3-bit is enough
} Posting;

typedef struct {
    uint32_t docID;
    std::string word;
    uint32_t post;
    uint16_t context;
} RawPosting;

typedef std::vector<RawPosting *> RawPostingVector;
typedef std::vector<Posting *> PostingVector;

typedef struct {
    uint32_t docID; //27-bit is enough
    PostingVector posArray;
} DocTuple;


class InvertedTable
{
private:
    FILEMODE _mode;
    std::string _word;
    char _outputPath[128];
    std::vector<DocTuple *> _invertedList;
    uint32_t _lastDocID;
    uint32_t _counter;
    uint16_t _fileID;
    uint32_t _DocNumLastWord;
    void write();
public:
    InvertedTable();
    ~InvertedTable();
    void SetFileMode(FILEMODE mode);
    int Insert(RawPosting *rawPosting);
    int WriteOutstanding();
    uint16_t GetFileID();
    uint32_t GetDocNumLastWord();
};

void freeRawPostingVector(RawPostingVector *vector);
void WriteRawPostingToFile(RawPostingVector *vector, const char* filePath, FILEMODE mode);
int WriteRawPostingToBuffer(char* buffer, RawPostingVector *vector, const char* filePath, FILEMODE mode);
bool CompareRawPostingDocID(RawPosting *posting1, RawPosting *posting2);
bool CompareRawPostingWord(RawPosting *posting1, RawPosting *posting2);

#endif /* defined(__index__invertedTable__) */
