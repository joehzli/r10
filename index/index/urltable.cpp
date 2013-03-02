//
//  urltable.cpp
//  index
//
//  Created by Hanzhou Li on 3/2/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#include "urltable.h"

URLTable::URLTable()
{
    _urlTable = new URLMap;
}

URLTable::~URLTable()
{
    _urlTable->clear();
    delete _urlTable;
    _urlTable = NULL;
}

URLItem* URLTable::Get(uint32_t docID)
{
    URLMap::const_iterator it = _urlTable->find(docID);
    if(it != _urlTable->end()) {
        return it->second;
    }
    
    return NULL;
}

void URLTable::Add(URLItem* urlitem)
{
    URLMap::const_iterator it = _urlTable->find(urlitem->docID);
    if (it != _urlTable->end()) {
        return;
    }
    
    _urlTable->insert(std::make_pair(urlitem->docID, urlitem));
}
