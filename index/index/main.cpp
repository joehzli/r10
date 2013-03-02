//
//  main.cpp
//  index
//
//  Created by Hanzhou Li on 3/2/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#include <iostream>
#include "urltable.h"

int main(int argc, const char * argv[])
{
    URLTable urlTable = URLTable();
    for (int i = 0; i< 10000000; i++) {
        URLItem *urlItem = new URLItem;
        urlItem->docID = i;
        urlItem->fileName= "fdsfds.data";
        urlItem->startIndex=111;
        urlItem->url= "http://www.google.com";
        urlItem->urlLen = 132;
        urlTable.Add(urlItem);
    }
    // insert code here...
    std::cout << "Hello, World!\n";
    std::cout <<urlTable.Get(1)->url <<std::endl;
    return 0;
}

