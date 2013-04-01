//
//  lexiconTable.h
//  index
//
//  Created by Hanzhou Li on 3/2/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#ifndef __index__lexiconTable__
#define __index__lexiconTable__

#include <iostream>
#include <string>
#include <vector>
#include "Utility.h"
#include "GlobalConfig.h"

typedef struct {
    std::string word;
    uint32_t num;   // number of docs containing the word
    uint32_t blockID;   //id of the block, 32 bit can represent 281TB index data
    uint16_t listID;     //id of the list in the block, the block size if 64KB
} LexiconItem;

typedef std::vector<LexiconItem *> LexiconTable;

void WriteLexiconTable(LexiconTable *lexiconTable, FILEMODE mode);

#endif /* defined(__index__lexiconTable__) */
