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
            fprintf(fp,"%s %d %d %d %d\n", lexiconItem->word.data(), lexiconItem->num, lexiconItem->fileID, lexiconItem->invertedPointer, lexiconItem->invertedPointerEnd);
        }
        
        fclose(fp);
    }
}