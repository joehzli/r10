//
//  pfordelta.h
//  index
//
//  Created by Hanzhou Li on 3/31/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#ifndef __index__pfordelta__
#define __index__pfordelta__

#include <iostream>
#include <vector>
#include "invertedTable.h"

using namespace std;

void calpfordelata(const vector<Posting *> *list, vector<uint32_t> *zipDocID, vector<uint32_t> *zipFreq, vector<uint32_t> *zipPos);
#endif /* defined(__index__pfordelta__) */
