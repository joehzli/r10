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
#define MAX_PATH_LENGTH 128 //Max length of file path
#define MAX_URL_LENGTH  4096    //Max length of URL
#define MAX_PAGE_LENGTH 33554432    // 32m Max per page
#define MAX_WORD_LENGTH 524288    //52k max per wordsingle word buffer
#define MAX_GZFILE_LENGTH 536870912 //512M Max gzip file size
#define TMP_INDEX_PATH "./tmp/merge.index"
#define MERGE_SORT_COMMAND "sort -S 1G -m -k 2f,2f -k 1n,1n ./tmp/*.tmp -o  ./tmp/merge.index"
#define CLEAR_TMP_COMMAND "rm -rf ./tmp/*.tmp"
#define MAX_POSTING_PER_TMP_INDEX 10000000 // 10 million
#define MAX_FILE_SIZE 4*1000*1000*1000 // Max size of final inverted index file
#define LEXICON_FILE "data/lexicon.index"
#define URL_FILE "data/url.index"
#define INVERTEDINDEX_FILE "data/inverted_%d.index"
#define TMPINDEX_FILE "tmp/%d.index.tmp"
#define DATASOURCE_FILE "datasource"
#define POST_PER_CHUNK 128
#define BLOCK_SIZE 65536   //64k bytes
#define MAX_DOCID 268435456 //use almost 28 bits representing 0-268435455
                            //, which means we can only have 268.43 million pages

#endif
