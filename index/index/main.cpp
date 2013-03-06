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
#include "lexiconTable.h"
#include "urltable.h"
#include "data.h"
#include "parser.h"
#include "Utility.h"

using namespace std;

int main(int argc, const char * argv[])
{
    //GenerateTmpIndex();
    // MergeTmpIndex();
    GenerateInvertedIndexFile();
    
//    URLTable urlTable = URLTable();
//    for (int i = 0; i< 100; i++) {
//        URLItem *urlItem = new URLItem;
//        urlItem->docID = i;
//        urlItem->fileID= 1;
//        urlItem->startIndex=111;
//        urlItem->url= "http://www.google.com";
//        urlItem->urlLen = 132;
//        urlTable.Add(urlItem);
//    }
    
    //cout <<urlTable.Get(1)->url <<endl;
    return 0;
}

