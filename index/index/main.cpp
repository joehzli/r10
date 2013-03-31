//
//  main.cpp
//  index
//
//  Created by Hanzhou Li on 3/2/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#include <iostream>
#include "invertedTable.h"
#include "lexiconTable.h"
#include "urltable.h"
#include "datasource.h"
#include "tmpindex.h"
#include<time.h>

using namespace std;

int main(int argc, const char * argv[])
{
    time_t start, middle;
    start = time(NULL);
    middle = time(NULL);
    
    cout<<"Start to generate temporary index files and URLTable."<<endl;
    GenerateTmpIndex();
    cout << "Runtime: " << difftime(time(NULL), middle)<< " Seconds." << endl;
    middle = time(NULL);
    
    cout<<"Start to merge temporary index files."<<endl;
    MergeTmpIndex();
    cout << "Runtime: " << difftime(time(NULL), middle) << " Seconds." << endl;
    middle = time(NULL);
    
    cout<<"Start to generate final inverted index files and LexiconTable."<<endl;
    GenerateInvertedIndexFile();
    cout << "Runtime: " << difftime(time(NULL), middle)<< " Seconds." << endl;
    cout<<"All Done!"<<endl;
    cout << "Total Runtime: " << difftime(time(NULL), start) << " Seconds." << endl;
    return 0;
}

