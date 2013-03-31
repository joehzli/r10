//
//  data.cpp
//  index
//
//  Created by Hanzhou Li on 3/2/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#include <queue>
#include "datasource.h"
#include "lexiconTable.h"
#include "invertedTable.h"
#include "urltable.h"
#include "parser.h"

using namespace std;

vector<string> *GetDataFileList(const char *path)
{
    vector<string> *fileList = new vector<string>();
    queue<char*> folderQueue = queue<char*>();
    char *fullPath = new char[MAX_PATH_LENGTH];
    sprintf(fullPath, "%s", path);
    folderQueue.push(fullPath);
    struct dirent *de;
    // Loop while not NULL
    while(folderQueue.size() > 0) {
        char* path = folderQueue.front();
        DIR *dir = opendir(path);
        if (dir == NULL)
            continue;
        while((de = readdir(dir))) {
            if(de == NULL || de->d_name == NULL) {
                break;
            }
            if (de->d_type == DT_REG && strlen(de->d_name) > 0 && de->d_name[0] != '.') {
                char *fullPath = new char[MAX_PATH_LENGTH];
                sprintf(fullPath, "%s/%s", path, de->d_name);
                fileList->push_back(fullPath);
                delete []fullPath;
                fullPath = NULL;
            }
            else if(de->d_type == DT_DIR && strlen(de->d_name) > 0 && de->d_name[0] != '.') {
                char *fullPath = new char[MAX_PATH_LENGTH];
                sprintf(fullPath, "%s/%s", path, de->d_name);
                folderQueue.push(fullPath);
            }
        }
        
        // release the queue
        delete []path;
        folderQueue.front() = NULL;
        folderQueue.pop();
        closedir(dir);
    }
    
    return fileList;
}

vector<IndexRecord *> *ParseIndex(char *indexBuf)
{   
    IndexRecordVector *indexArray = new IndexRecordVector;
    char* indexPointer = indexBuf;
    char urlBuff[MAX_URL_LENGTH];
    while((*indexPointer) != 0) {
        IndexRecord *indexRecord = new IndexRecord;
        int i = 0;
        while((*indexPointer)!=' ') {
            urlBuff[i] = (*indexPointer);
            i++;
            indexPointer++;
        }
        urlBuff[i] = 0;
        indexRecord->url = urlBuff;
        
        while((*indexPointer) == ' ') {
            indexPointer++;
        }
        
        indexRecord->depth = atoi(indexPointer);
        while((*indexPointer) != ' ') {
            indexPointer++;
        }
        while((*indexPointer) == ' ') {
            indexPointer++;
        }
        
        indexRecord->unknown = atoi(indexPointer);
        while((*indexPointer) != ' ') {
            indexPointer++;
        }
        while((*indexPointer) == ' ') {
            indexPointer++;
        }
        
        indexRecord->length = atoi(indexPointer);
        while((*indexPointer) != ' ') {
            indexPointer++;
        }
        while((*indexPointer) == ' ') {
            indexPointer++;
        }
        
        i=0;
        while((*indexPointer)!=' ') {
            indexRecord->IP[i] = (*indexPointer);
            i++;
            indexPointer++;
        }
        indexRecord->IP[i] = 0;
        
        while((*indexPointer) == ' ') {
            indexPointer++;
        }
        
        indexRecord->port = atoi(indexPointer);
        while((*indexPointer) != ' ') {
            indexPointer++;
        }
        while((*indexPointer) == ' ') {
            indexPointer++;
        }
        
        i=0;
        while((*indexPointer)!=' ' && (*indexPointer) != '\n'&& (*indexPointer) != 0) {
            indexRecord->status[i] = (*indexPointer);
            i++;
            indexPointer++;
        }
        indexRecord->status[i] = '\0';
        indexArray->push_back(indexRecord);
        while((*indexPointer) != '\n' && (*indexPointer) != 0) {
            indexPointer++;
        }
        if((*indexPointer) == 0) {
            break;
        } else {
            indexPointer++;
        }
    }
    
    return indexArray;
}

CStringVector *GetPages(char* dataBuf, IndexRecordVector* indexArray)
{
    CStringVector *pages = new CStringVector;
    char *dataPointer = dataBuf;
    for(int i=0;i<indexArray->size();i++) {
        IndexRecord *indexRecord = (*indexArray)[i];
        int pageLength= indexRecord->length;
        if(strcmp(indexRecord->status, "ok") != 0)
            continue;
        else {
            int pageLength= indexRecord->length;
            char *page = new char[pageLength+1];
            strncpy(page, dataPointer, pageLength);
            page[pageLength] = '\0';
            pages->push_back(page);
        }
        
        dataPointer += pageLength;
    }
    return pages;
}

