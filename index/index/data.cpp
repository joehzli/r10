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
    return buffer;
    
}

vector<IndexRecord *> *ParseIndex(char *indexBuf)
{
    IndexRecordVector *indexArray = new IndexRecordVector;
    char* indexPointer = indexBuf;
    while(1) {
        IndexRecord *indexRecord = new IndexRecord;
        //std::cout<<indexPointer<<std::endl;
        int nRead = 0;
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

void GenerateTmpIndex()
{
    StringVector *fileList = GetDataFileList("data");
    uint32_t docID = 0;
    RawPostingVector *postingVector = new RawPostingVector;
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
            for(int j=1;j<pages->size();j++) {
                char *page = (*pages)[j];
                int pageLength = (*indexArray)[j]->length;
                char *url = (*indexArray)[j]->url;
                GetPostingFromPage(postingVector, page, url, pageLength, docID);
                docID++;
                if(postingVector->size() > MAX_POSTING_PER_TMP_INDEX) {
                    char tmpIndexFileName[128];
                    sprintf(tmpIndexFileName, "tmp/%d.index.tmp", docID);
                    WriteRawPostingToFile(postingVector, tmpIndexFileName, FILEMODE_ASCII);
                    freeRawPostingVector(postingVector);
                }
            }
        }
    }
    
    if(postingVector->size() > 0) {
        char tmpIndexFileName[128];
        sprintf(tmpIndexFileName, "tmp/%d.index.tmp", docID);
        WriteRawPostingToFile(postingVector, tmpIndexFileName, FILEMODE_ASCII);
        freeRawPostingVector(postingVector);
    }
    
}

void MergeTmpIndex()
{
    //merge sort the index
    system(MERGE_SORT_COMMAND);
    system(CLEAR_TMP_COMMAND);
    cout<<"merge done.\n";
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
        posting->post = invertedPost;
        
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
            lexiconItem->invertedPointerEnd = count;
            lexiconItem->fileID = invertedTable.GetFileID();
            lexiconItem->num = invertedTable.GetDocNumLastWord();
            if(count < lastCounter) {
                lexiconItem->invertedPointer = 0;
            }
            
            lexiconItem = new LexiconItem;
            lexiconItem->word = posting->word;
            lastWord = posting->word;
            lexiconItem->invertedPointer = count+1;
            lexiconTable.push_back(lexiconItem);
            lastCounter = count;
        }
        
    }
    
    int count = invertedTable.WriteOutstanding();
    lexiconTable.back()->invertedPointerEnd = count;
    lexiconTable.back()->fileID = invertedTable.GetFileID();
    lexiconTable.back()->num = invertedTable.GetDocNumLastWord();
    if(count < lastCounter) {
        lexiconTable.back()->invertedPointer = 0;
    }
    
    WriteLexiconTable(&lexiconTable, FILEMODE_ASCII);
}