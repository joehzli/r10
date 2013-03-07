//
//  GlobalConfig.h
//  index
//
//  Created by Hanzhou Li on 3/6/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#ifndef index_GlobalConfig_h
#define index_GlobalConfig_h

#define FILEMODE_ASCII 0
#define FILEMODE_BIN 1

#define CURRENT_FILEMODE FILEMODE_BIN
#define INDEX_CHUNK 409600 //50KB
#define DATA_CHUNK 20971520 //2.5MB
#define TMP_INDEX_PATH "./tmp/merge.index"
#define MERGE_SORT_COMMAND "sort -m -k 2f,2f -k 1n,1n ./tmp/*.tmp -o  ./tmp/merge.index"
#define CLEAR_TMP_COMMAND "rm -rf ./tmp/*.tmp"
#define MAX_POSTING_PER_TMP_INDEX 10000000 // 10 million
#define MAX_FILE_SIZE 4*1000*1000*1000 // Max size of final inverted index file
#define LEXICON_FILE "data/lexicon.index"
#define URL_FILE "data/url.index"

#endif
