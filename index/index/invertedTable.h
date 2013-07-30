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
#include "GlobalConfig.h"
#include "tmpindex.h"

// Used in inverted index
typedef struct {
    uint32_t docID;
    uint32_t actualDocID;
    uint32_t freq;
    uint32_t pos;
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
    FILE *_fp;
    void write();
public:
    InvertedTable();
    ~InvertedTable();
    void SetFileMode(FILEMODE mode);
    uint32_t Insert(const RawPosting *rawPosting);
    uint32_t WriteOutstanding();
    uint16_t GetFileID();
    uint32_t GetDocNumLastWord();
};

/****************************************************
 Generate the final inverted index files from the
 temporary index files.
 ****************************************************/
void GenerateInvertedIndexFile();

#endif /* defined(__index__invertedTable__) */
