//
//  lexiconTable.cpp
//  index
//
//  Created by Hanzhou Li on 3/2/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#include "lexiconTable.h"

void WriteLexiconTable(LexiconTable *lexiconTable, FILEMODE mode)
{
    if(mode == FILEMODE_ASCII) {
        FILE *fp = fopen(LEXICON_FILE, "w+");
        for(int i=0;i<lexiconTable->size();i++) {
            LexiconItem *lexiconItem =(*lexiconTable)[i];
            fprintf(fp,"%s %d %d %d\n", lexiconItem->word.c_str(), lexiconItem->num, lexiconItem->blockID, lexiconItem->listID);
        }
        
        fclose(fp);
    }
    
    if(mode == FILEMODE_BIN) {
        FILE *fp = fopen(LEXICON_FILE, "wb+");
        for(int i=0;i<lexiconTable->size();i++) {
            LexiconItem *lexiconItem =(*lexiconTable)[i];
            //in binary file, the format is:
            //Length_of_word, word, num, block_id, list_id
            uint16_t wordLength = lexiconItem->word.length();
            fwrite(&wordLength, sizeof(uint16_t),1, fp);    // write the word length
            fwrite(lexiconItem->word.data(), sizeof(char), wordLength, fp);
            fwrite(&lexiconItem->num, sizeof(uint32_t), 1, fp);
            fwrite(&lexiconItem->blockID, sizeof(uint32_t), 1, fp);
            fwrite(&lexiconItem->listID, sizeof(uint16_t), 1, fp);
        }
        
        fclose(fp);
    }
}