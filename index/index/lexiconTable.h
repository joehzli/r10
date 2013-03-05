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

typedef struct {
    std::string word;
    uint32_t num;   // number of docs containing the word
    uint16_t invertedID;    // the ID of the file containing the invertd index
    uint32_t invertedPointer;   //pointer to the inverted index
    uint32_t length;    // the length of this inverted record
} LexiconItem;

class LexiconTable
{
private:
    
    
};

#endif /* defined(__index__lexiconTable__) */
