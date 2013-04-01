//
//  tmpindex.cpp
//  index
//
//  Created by Hanzhou Li on 3/31/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#include "tmpindex.h"
#include "GlobalConfig.h"
#include "Utility.h"
#include "datasource.h"
#include "urltable.h"
#include "gzfile.h"
#include "parser.h"

using namespace std;

void GenerateTmpIndex()
{
    StringVector *fileList = GetDataFileList(DATASOURCE_FILE);
    uint32_t docID = 0;
    uint32_t fileID = 0;
    RawPostingVector *postingVector = new RawPostingVector;
    URLTable urlTable = URLTable();
    for(int i = 0; i < fileList->size(); i++) {
        string fileName = (*fileList)[i];
        if(fileName.substr(fileName.length()-5, 5) == "index")
        {
            string indexFilePath = fileName;
            string dataFilePath = fileName.substr(0, fileName.length()-5)+"data";
            char *dataFileBuf = ReadGZFile(dataFilePath.data());
            if (dataFileBuf == NULL) {
                fileID++;
                continue;
            }
            char *indexFileBuf = ReadGZFile(indexFilePath.data());
            if (dataFileBuf == NULL) {
                free(dataFileBuf);
                dataFileBuf = NULL;
                fileID++;
                continue;
            }
            IndexRecordVector *indexArray = ParseIndex(indexFileBuf);
            CStringVector *pages = GetPages(dataFileBuf, indexArray);
            // free the datafile buffer
            free(dataFileBuf);
            dataFileBuf = NULL;
            free(indexFileBuf);
            indexFileBuf = NULL;
            
            // deal with each page
            uint32_t urlPointer = 0;
            for(int j=0;j<pages->size();j++) {
                char *page = (*pages)[j];
                int pageLength = (*indexArray)[j]->length;
                const char *url = (*indexArray)[j]->url.c_str();
                uint32_t wordCount = GetPostingFromPage(postingVector, page, url, pageLength, docID);
                
                URLItem *urlItem = new URLItem;
                urlItem->docID = docID;
                urlItem->fileID= fileID;
                urlItem->startIndex=urlPointer;
                urlItem->url= (*indexArray)[j]->url.c_str();
                urlItem->dl = wordCount;
                urlPointer+=(*indexArray)[j]->length;
                urlTable.Add(urlItem);
                docID++;
                char tmpIndexFileName[MAX_PATH_LENGTH];
                if(postingVector->size() > MAX_POSTING_PER_TMP_INDEX) {
                    sprintf(tmpIndexFileName, TMPINDEX_FILE, docID);
                    WriteRawPostingToFile(postingVector, tmpIndexFileName, FILEMODE_ASCII);
                    freeRawPostingVector(postingVector);
                }
                delete []page;
                page = NULL;
            }
            pages->clear();
            delete pages;
            pages = NULL;
            for(int i=0;i<indexArray->size();i++) {
                IndexRecord *indexRecord = (*indexArray)[i];
                delete indexRecord;
                (*indexArray)[i] = NULL;
            }
            indexArray->clear();
            delete indexArray;
            indexArray = NULL;
            fileID++;
        }
    }
    fileList->clear();
    delete fileList;
    fileList = NULL;
    char tmpIndexFileName[MAX_PATH_LENGTH];
    if(postingVector->size() > 0) {
        sprintf(tmpIndexFileName, TMPINDEX_FILE, docID);
        WriteRawPostingToFile(postingVector, tmpIndexFileName, FILEMODE_ASCII);
        freeRawPostingVector(postingVector);
    }
    delete postingVector;
    urlTable.Write(CURRENT_FILEMODE);
}

void MergeTmpIndex()
{
    //merge sort the index
    system(MERGE_SORT_COMMAND);
    system(CLEAR_TMP_COMMAND);
}

int GetPostingFromPage(RawPostingVector *vector, char* page, const char* url, int length, uint32_t docID)
{
    // call parser
    int ret = parser(url, page, vector, docID);
    if(ret < 0) {
        // page has errors
        return 0;
    }
    
    return ret; //it's also the word count
}

void freeRawPostingVector(RawPostingVector *vector)
{
    for(int i=0;i<vector->size();i++) {
        RawPosting *posting = (*vector)[i];
        delete posting;
        (*vector)[i] = NULL;
    }
    
    vector->clear();
}

void WriteRawPostingToFile(RawPostingVector *vector, const char* filePath, FILEMODE mode)
{
    FILE *fp = fopen(filePath, "w+");
    if(fp != NULL) {
        std::stable_sort(vector->begin(), vector->end(), CompareRawPostingDocID);
        // must use stable sort in the second round
        std::stable_sort(vector->begin(), vector->end(), CompareRawPostingWord);
        for(int i=0; i<vector->size();i++) {
            RawPosting *posting = (*vector)[i];
            fprintf(fp, "%d %s %d\n", posting->docID, posting->word.c_str(), posting->pos);
        }
        fclose(fp);
    }
}

bool CompareRawPostingDocID(RawPosting *posting1, RawPosting *posting2)
{
    return posting1->docID < posting2->docID;
}

bool CompareRawPostingWord(RawPosting *posting1, RawPosting *posting2)
{
    return posting1->word < posting2->word;
}