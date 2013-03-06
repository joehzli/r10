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
#define TMP_INDEX_PATH "./tmp/merge.index"
#define MERGE_SORT_COMMAND "sort -m -k 2f,2f -k 1n,1n ./tmp/*.tmp -o  ./tmp/merge.index"
#define CLEAR_TMP_COMMAND "rm -rf ./tmp/*.tmp"
#define MAX_POSTING_PER_TMP_INDEX 10000000 // 10 million

typedef struct {
    char url[2083]; // max url lenth in IE
    int depth;
    int unknown;
    int length;
    char IP[18];
    int port;
    char status[8];
    
} IndexRecord;

typedef std::vector<IndexRecord *> IndexRecordVector;

/****************************************************
 Return a vector containing the data files' names.
 This method will get all the files in the path recursively.
 @path: The path of the folder containing the files.
 ***************************************************/
StringVector *GetDataFileList(const char *path);

/****************************************************
 Return the uncompress content of the GZipped file.
 @filePath: The path of the compressed file.
 ****************************************************/
char *ReadGZFile(const char *filePath);

/****************************************************
 Return a vector of the index file of the data set.
 @indexBuf: The content of the index file.
 ****************************************************/
IndexRecordVector *ParseIndex(char *indexBuf);

/****************************************************
 Return a vector of the pages' content parsed from 
 the data file.
 @dataBuf: The content of the data file to be parsed.
 @indexArray: The vector array contain the responding
 indice of the data file.
 ****************************************************/
CStringVector *GetPages(char* dataBuf, IndexRecordVector* indexArray);

void GenerateTmpIndex();

/****************************************************
 Merge the generated temporary index files.
 ****************************************************/
void MergeTmpIndex();

/****************************************************
 Generate the final inverted index files from the
 temporary index files.
 ****************************************************/
void GenerateInvertedIndexFile();

#endif /* defined(__index__data__) */
