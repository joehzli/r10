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
#include "data.h"
#include<time.h>

using namespace std;

int main(int argc, const char * argv[])
{
    clock_t start, middle;
    start = clock();
    middle = clock();
    
    cout<<"Start to generate temporary index files and URLTable."<<endl;
    GenerateTmpIndex();
    cout << "Runtime: " << ((double)((clock()- middle))/CLOCKS_PER_SEC)/60.0 << " Minutes." << endl;
    middle = clock();
    
    cout<<"Start to merge temporary index files."<<endl;
    MergeTmpIndex();
    cout << "Runtime: " << ((double)((clock()- middle))/CLOCKS_PER_SEC)/60.0 << " Minutes." << endl;
    middle = clock();
    
    cout<<"Start to generate final inverted index files and LexiconTable."<<endl;
    GenerateInvertedIndexFile();
    cout << "Runtime: " << ((double)((clock()- middle))/CLOCKS_PER_SEC)/60.0 << " Minutes." << endl;
    cout<<"All Done!"<<endl;
    cout << "Total Runtime: " << ((double)((clock()- middle))/CLOCKS_PER_SEC)/60.0 << " Minutes." << endl;
    return 0;
}

