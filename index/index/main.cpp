//
//  main.cpp
//  index
//
//  Created by Hanzhou Li on 3/2/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include "invertedTable.h"
#include "urltable.h"
#include "data.h"
#include "parser.h"
#include "Utility.h"

using namespace std;

int main(int argc, const char * argv[])
{
    StringVector *fileList = GetDataFileList("data");
    uint32_t docID = 0;
    for(int i = 0; i < fileList->size(); i++) {
        
        string dataFilePath = (*fileList)[0];
        string indexFilePath = (*fileList)[1];
        char *dataFileBuf = ReadGZFile(dataFilePath.data());
        char *indexFileBuf = ReadGZFile(indexFilePath.data());
        IndexRecordVector *indexArray = ParseIndex(indexFileBuf);
        CStringVector *pages = GetPages(dataFileBuf, indexArray);
        // free the datafile buffer
        delete dataFileBuf;
        
        // deal with each page
        for(int j=1;j<pages->size();j++) {
            char *page = (*pages)[j];
            int pageLength = (*indexArray)[j]->length;
            char *url = (*indexArray)[j]->url;
            RawPostingVector *postingVector = GetPostingFromPage(page, url, pageLength, docID);
            docID++;
            for(int k =0;k<postingVector->size();k++) {
                cout<<(*postingVector)[k]->word<<' '<<(*postingVector)[k]->post<<endl;
            }
            
            break;
        }
        break;
    }
    URLTable urlTable = URLTable();
    for (int i = 0; i< 100; i++) {
        URLItem *urlItem = new URLItem;
        urlItem->docID = i;
        urlItem->fileID= 1;
        urlItem->startIndex=111;
        urlItem->url= "http://www.google.com";
        urlItem->urlLen = 132;
        urlTable.Add(urlItem);
    }
    
    //cout <<urlTable.Get(1)->url <<endl;
    return 0;
}

