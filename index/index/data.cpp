//
//  data.cpp
//  index
//
//  Created by Hanzhou Li on 3/2/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#include <queue>
#include <zlib.h>
#include "data.h"

using namespace std;

vector<string> *GetDataFileList(const char *path)
{
    vector<string> *fileList = new vector<string>();
    queue<char*> folderQueue = queue<char*>();
    char *fullPath = new char[128];
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
            if (de->d_type == DT_REG && strlen(de->d_name) > 0 && de->d_name[0] != '.') {
                char *fullPath = new char[128];
                sprintf(fullPath, "%s/%s", path, de->d_name);
                fileList->push_back(fullPath);
            }
            else if(de->d_type == DT_DIR && strlen(de->d_name) > 0 && de->d_name[0] != '.') {
                char *fullPath = new char[128];
                sprintf(fullPath, "%s/%s", path, de->d_name);
                folderQueue.push(fullPath);
            }
        }
        
        // release the queue
        delete path;
        folderQueue.pop();
        closedir(dir);
    }
    
    return fileList;
}

char *ReadGZFile(const char *filePath)
{
    gzFile gzfile = gzopen(filePath, "r");
    int size = INDEX_CHUNK;
    char *buffer=(char *)malloc(size);
    int oldSize=size;
    
    int count=0;             //The number of bytes that already read
    while (!gzeof(gzfile))
    {
        count+=gzread(gzfile,buffer+count,oldSize);
        if (count==size)                    // Reallocate when buffer is full
        {
            oldSize=size;
            size*=2;
            buffer=(char *)realloc(buffer,size);
        }
    }
    return buffer;
    
}

vector<IndexRecord *> *ParseIndex(char *indexBuf)
{
    IndexRecordVector *indexArray = new IndexRecordVector;
    char* indexPointer = indexBuf;
    while(1) {
        IndexRecord *indexRecord = new IndexRecord;
        int nRead;
        int ret = sscanf(indexPointer, "%s %d %d %d %s %d %s%n", indexRecord->url, &indexRecord->depth, &indexRecord->unknown, &indexRecord->length, indexRecord->IP, &indexRecord->port, indexRecord->status, &nRead);
        if(ret == 7) {
            indexArray->push_back(indexRecord);
            indexPointer +=nRead;
        }
        else {
            delete indexRecord;
            break;
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