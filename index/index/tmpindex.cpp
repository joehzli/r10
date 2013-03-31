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
            char *indexFileBuf = ReadGZFile(indexFilePath.data());
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
                GetPostingFromPage(postingVector, page, url, pageLength, docID);
                
                URLItem *urlItem = new URLItem;
                urlItem->docID = docID;
                urlItem->fileID= fileID;
                urlItem->startIndex=urlPointer;
                urlItem->url= (*indexArray)[j]->url;
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
    char *parsedBuf = new char[length*2];
    bzero(parsedBuf, length*2);
    int ret = parser(url, page, parsedBuf, length*2);
    if(ret == 0||parsedBuf == NULL || strlen(parsedBuf) == 0) {
        delete []parsedBuf;
        parsedBuf = NULL;
        return 0;
    }
    if(ret < 0) {
        std::cout<<"ret < 0:"<<docID<<std::endl;
        delete []parsedBuf;
        parsedBuf = NULL;
        return 0;
    }
    
    size_t parsedBufLength = strlen(parsedBuf);
    char* pagePointer = parsedBuf;
    int counter = 1;
    while((*pagePointer) != 0) {
        RawPosting *posting = new RawPosting;
        char context;
        char* word = new char[parsedBufLength];
        int i = 0;
        while((*pagePointer)!=' ') {
            word[i] = (*pagePointer);
            i++;
            pagePointer++;
        }
        word[i] = 0;
        
        while((*pagePointer) == ' ') {
            pagePointer++;
        }
        
        context = (*pagePointer);
        posting->docID = docID;
        posting->word=word;
        std::transform(posting->word.begin(), posting->word.end(), posting->word.begin(), ::tolower);
        switch(context) {
            case 'P':
                posting->context = 0;
                break;
            case 'B':
                posting->context = 1;
                break;
            case 'H':
                posting->context = 2;
                break;
            case 'I':
                posting->context = 3;
                break;
            case 'T':
                posting->context = 4;
                break;
            case 'U':
                posting->context = 5;
                break;
            default:
                posting->context = 10;
                break;
        }
        posting->pos = counter;
        counter++;
        pagePointer++;
        vector->push_back(posting);
        delete word;
        word= NULL;
        
        while((*pagePointer) != '\n' && (*pagePointer) != 0) {
            pagePointer++;
        }
        if((*pagePointer) == 0) {
            break;
        } else {
            pagePointer++;
        }
    }
    delete []parsedBuf;
    parsedBuf = NULL;
    return 1;
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
            fprintf(fp, "%d %s %d %d\n", posting->docID, posting->word.c_str(), posting->pos, posting->context);
        }
        fclose(fp);
    }
}

uint32_t WriteRawPostingToBuffer(char* buffer, RawPostingVector *vector, const char* filePath, FILEMODE mode)
{
    uint32_t count = 0;
    std::sort(vector->begin(), vector->end(), CompareRawPostingDocID);
    // must use stable sort in the second round
    std::stable_sort(vector->begin(), vector->end(), CompareRawPostingWord);
    for(int i=0; i<vector->size();i++) {
        RawPosting *posting = (*vector)[i];
        int nRead = 0;
        nRead = sprintf(buffer, "%d %s %d %d\n", posting->docID, posting->word.c_str(), posting->pos, posting->context);
        count +=nRead;
        buffer += nRead;
    }
    return count;
}

bool CompareRawPostingDocID(RawPosting *posting1, RawPosting *posting2)
{
    return posting1->docID < posting2->docID;
}

bool CompareRawPostingWord(RawPosting *posting1, RawPosting *posting2)
{
    return posting1->word < posting2->word;
}