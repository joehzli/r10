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
#include "lexiconTable.h"
#include "invertedTable.h"
#include "urltable.h"
#include "parser.h"

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
            if(de == NULL || de->d_name == NULL) {
                break;
            }
            if (de->d_type == DT_REG && strlen(de->d_name) > 0 && de->d_name[0] != '.') {
                char *fullPath = new char[128];
                sprintf(fullPath, "%s/%s", path, de->d_name);
                fileList->push_back(fullPath);
                delete fullPath;
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
    buffer[count] = '\0';
    gzclose(gzfile);
    return buffer;
    
}

vector<IndexRecord *> *ParseIndex(char *indexBuf)
{   
    IndexRecordVector *indexArray = new IndexRecordVector;
    char* indexPointer = indexBuf;
    while((*indexPointer) != 0) {
        IndexRecord *indexRecord = new IndexRecord;
        int i = 0;
        while((*indexPointer)!=' ') {
            indexRecord->url[i] = (*indexPointer);
            i++;
            indexPointer++;
        }
        indexRecord->url[i] = 0;
        
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

void GenerateTmpIndex()
{
    StringVector *fileList = GetDataFileList("data");
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
            delete dataFileBuf;
            delete indexFileBuf;
            // deal with each page
            uint32_t urlPointer = 0;
            for(int j=0;j<pages->size();j++) {
                char *page = (*pages)[j];
                int pageLength = (*indexArray)[j]->length;
                char *url = (*indexArray)[j]->url;
                GetPostingFromPage(postingVector, page, url, pageLength, docID);
                
                URLItem *urlItem = new URLItem;
                urlItem->docID = docID;
                urlItem->fileID= fileID;
                urlItem->startIndex=urlPointer;
                urlItem->url= (*indexArray)[j]->url;
                urlPointer+=(*indexArray)[j]->length;
                urlTable.Add(urlItem);
                docID++;
                if(postingVector->size() > MAX_POSTING_PER_TMP_INDEX) {
                    char tmpIndexFileName[128];
                    sprintf(tmpIndexFileName, "tmp/%d.index.tmp", docID);
                    WriteRawPostingToFile(postingVector, tmpIndexFileName, FILEMODE_ASCII);
                    freeRawPostingVector(postingVector);
                }
                delete page;
            }
            pages->clear();
            delete pages;
            for(int i=0;i<indexArray->size();i++) {
                IndexRecord *indexRecord = (*indexArray)[i];
                delete indexRecord;
            }
            indexArray->clear();
            delete indexArray;
            fileID++;
        }
    }
    fileList->clear();
    delete fileList;
    fileList = NULL;
    if(postingVector->size() > 0) {
        char tmpIndexFileName[128];
        sprintf(tmpIndexFileName, "tmp/%d.index.tmp", docID);
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

void GenerateInvertedIndexFile()
{
    //convert it into inverted index
    FILE *mergedIndex = fopen(TMP_INDEX_PATH, "r");
    RawPosting *posting = new RawPosting;
    char* invertedWord = new char[1024];
    uint32_t invertedContext;
    uint32_t invertedDocID;
    uint32_t invertedPost;
    InvertedTable invertedTable;
    LexiconTable lexiconTable;
    string lastWord = "";
    int lastCounter = 0;
    while(fscanf(mergedIndex, "%d %s %d %d\n", &invertedDocID, invertedWord, &invertedPost, &invertedContext) != EOF) {
        posting->docID = invertedDocID;
        posting->word=invertedWord;
        delete invertedWord;
        invertedWord = new char[1024];
        posting->context = invertedContext;
        posting->pos = invertedPost;
        
        // first time
        if(lastWord == "") {
            lastWord = posting->word;
            LexiconItem *lexiconItem = new LexiconItem;
            lexiconItem->word = posting->word;
            lexiconItem->invertedPointer = 0;
            lexiconTable.push_back(lexiconItem);
        }
        
        int count = invertedTable.Insert(posting);
        
        // count > 0 means meet new word and there is data written
        if(count > 0) {
            LexiconItem *lexiconItem = lexiconTable.back();
            lexiconItem->fileID = invertedTable.GetFileID();
            lexiconItem->num = invertedTable.GetDocNumLastWord();
            if(count < lastCounter) {
                lexiconItem->invertedPointer = 0;
            }
            
            lexiconItem = new LexiconItem;
            lexiconItem->word = posting->word;
            lastWord = posting->word;
            lexiconItem->invertedPointer = count;
            lexiconTable.push_back(lexiconItem);
            lastCounter = count;
        }
        
    }
    
    int count = invertedTable.WriteOutstanding();
    lexiconTable.back()->fileID = invertedTable.GetFileID();
    lexiconTable.back()->num = invertedTable.GetDocNumLastWord();
    if(count < lastCounter) {
        lexiconTable.back()->invertedPointer = 0;
    }
    
    WriteLexiconTable(&lexiconTable, CURRENT_FILEMODE);
}