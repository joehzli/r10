//
//  invertedTable.cpp
//  index
//
//  Created by Hanzhou Li on 3/2/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#include "invertedTable.h"
#include "stdio.h"
#include "lexiconTable.h"
#include "pfordelta.h"

using namespace std;

InvertedTable::InvertedTable()
{
    _mode = CURRENT_FILEMODE;
    _fileID=0;
    sprintf(_outputPath, INVERTEDINDEX_FILE, _fileID);
    _counter = 0;
    _fp = fopen(_outputPath, "a");
    _block.blockSize = 0;
    _word = "";
    _lastDocID = MAX_DOCID;
}
InvertedTable::~InvertedTable()
{
    fclose(_fp);
}

void InvertedTable::SetFileMode(FILEMODE mode)
{
    _mode = mode;
}

uint32_t InvertedTable::GetDocNumLastWord()
{
    return _DocNumLastWord;
}

void InvertedTable::write()
{
    if(_mode == FILEMODE_ASCII) {
        _DocNumLastWord = (uint32_t)_invertedList.size();
        for(int j=0;j<_invertedList.size();j++) {
            Posting *posting =_invertedList[j];
            _counter += fprintf(_fp,"%u %u %d", posting->docID, posting->freq, posting->pos);
            delete posting;
            _invertedList[j] = NULL;
        }
        _counter += fprintf(_fp, "\n");
        _invertedList.clear();
        vector<Posting *> tmp;
        _invertedList.swap(tmp);
    }else if(_mode == FILEMODE_BIN) {
        _DocNumLastWord = (uint32_t)_invertedList.size();
        for(int j=0;j<_invertedList.size();j++) {
            Posting *posting =_invertedList[j];
            uint32_t docListLength =(uint32_t)posting->posArray.size();
            _counter += sizeof(uint32_t) * fwrite(&posting->docID, sizeof(uint32_t), 1, _fp);
            _counter += sizeof(uint32_t) * fwrite(&docListLength, sizeof(uint32_t),1,_fp);
            for(int i=0;i<posting->posArray.size();i++) {
                _counter += sizeof(uint32_t) * fwrite(&posting->posArray[i]->pos, sizeof(uint32_t), 1, _fp);
                delete docTuple->posArray[i];
                docTuple->posArray[i] = NULL;
            }
            docTuple->posArray.clear();
            PostingVector tmp;
            docTuple->posArray.swap(tmp);
            delete docTuple;
            _invertedList[j] = NULL;
        }
        _invertedList.clear();
        vector<posting *> tmp;
        _invertedList.swap(tmp);
    }
}

uint32_t InvertedTable::WriteOutstanding()
{
    write();
    _invertedList.clear();
    return _counter;
}

uint32_t InvertedTable::Insert(const RawPosting *rawPosting)
{
    if (_mode == FILEMODE_ASCII) {
        // first insert, new word;
        if(_invertedList.size() == 0) {
            _word = rawPosting->word;
            _lastDocID = rawPosting->docID;
            Posting * posting = new Posting;
            posting->actualDocID = rawPosting->docID;
            posting->docID = rawPosting->docID;
            posting->pos = rawPosting->pos;
            posting->freq++;
            _invertedList.push_back(posting);
        } else if (_word != rawPosting->word){  // got a new word
            write(); //write and free the list
            _word = rawPosting->word;
            _lastDocID = rawPosting->docID;
            Posting * posting = new Posting;
            posting->actualDocID = rawPosting->docID;
            posting->docID = rawPosting->docID;
            posting->pos = rawPosting->pos;
            posting->freq++;
            _invertedList.push_back(posting);
            return _counter;
        } else {    //insert same word
            // The same doc, increase freq
            if(rawPosting->docID == _lastDocID) {
                Posting *posting = _invertedList.back();
                posting->freq++;
            } else {    // not the same doc, insert a new posting
                // Different doc, compress the docID
                Posting * posting = new Posting;
                _lastDocID = rawPosting->docID;
                posting->actualDocID = rawPosting->docID;
                // compress docID
                posting->docID = (rawPosting->docID-_invertedList.back()->actualDocID);
                posting->freq++;
                _invertedList.push_back(posting);
            }
            
        }
        // 0 means no data written
        return 0;
    } else if(_mode == FILEMODE_BIN) {
        
        //check if it's a new word, means new list
        int isNewWord = 0;
        if(_word != rawPosting->word) {
            _word = rawPosting->word;
            isNewWord = 1; // means new word
        }
        
        // not new word and not new doc, just increase its frequency
        if(isNewWord == 0 && _lastDocID == rawPosting->docID) {
            Posting *posting = _invertedList.back();
            posting->freq++;
        } else {
            // new posting
            // the first pos of each chunk must be actual pos
            // and the pos of the first posting must be actual pos
            // first insert, new word;
            if(_invertedList.size() == 128) {   // new chunk
                Chunk *newChunk = new Chunk;
                calpfordelata(&_invertedList, &newChunk->zipDocID, &newChunk->zipFreq, &newChunk->zipPos);
                if(_lastDocID != rawPosting->docID)
                    _lastDocID = rawPosting->docID;
                Posting * posting = new Posting;
                posting->actualDocID = rawPosting->docID;
                if(_invertedList.size() == 0 || isNewWord) {
                    posting->docID = rawPosting->docID;
                } else {
                    posting->docID = (rawPosting->docID - _invertedList.back()->actualDocID);
                }
                posting->pos = rawPosting->pos;
                posting->freq++;
                _invertedList.push_back(posting);
            } else{  // got a new word
                write(); //write and free the list
                _word = rawPosting->word;
                _lastDocID = rawPosting->docID;
                Posting * posting = new Posting;
                posting->actualDocID = rawPosting->docID;
                posting->docID = rawPosting->docID;
                posting->pos = rawPosting->pos;
                posting->freq++;
                _invertedList.push_back(posting);
                return _counter;
            } else {    //insert same word
                // The same doc, increase freq
                if(rawPosting->docID == _lastDocID) {
                    Posting *posting = _invertedList.back();
                    posting->freq++;
                } else {    // not the same doc, insert a new posting
                    // Different doc, compress the docID
                    Posting * posting = new Posting;
                    _lastDocID = rawPosting->docID;
                    posting->actualDocID = rawPosting->docID;
                    // compress docID
                    posting->docID = (rawPosting->docID-_invertedList.back()->actualDocID);
                    posting->freq++;
                    _invertedList.push_back(posting);
                }
                
            }

        }
        
                
        return isNewWord;
    }
}


void GenerateInvertedIndexFile()
{
    //convert it into inverted index
    FILE *mergedIndex = fopen(TMP_INDEX_PATH, "r");
    RawPosting posting;
    char invertedWord[MAX_WORD_LENGTH];
    uint32_t invertedDocID;
    uint32_t invertedPost;
    InvertedTable invertedTable;
    LexiconTable lexiconTable;
    int ret = 0;
    while(fscanf(mergedIndex, "%d %s %d\n", &invertedDocID, invertedWord, &invertedPost) != EOF) {
        if(strlen(invertedWord) > MAX_WORD_LENGTH) {
            cout<<invertedWord<<endl;
            cout<<strlen(invertedWord)<<endl;
            exit(1);
        }
        posting.docID = invertedDocID;
        posting.word=invertedWord;
        bzero(invertedWord, MAX_WORD_LENGTH);
        posting.pos = invertedPost;
        ret = invertedTable.Insert(&posting);
        
        if(CURRENT_FILEMODE == FILEMODE_ASCII) {
            continue;
        }
        
        // we only save lexicon in BIN mode
        if (ret > 0) {  // ret > 0 means meet new word
            LexiconItem *lexiconItem = new LexiconItem;
            lexiconItem->word = posting.word;
            lexiconItem->blockID = invertedTable.blockID;
            lexiconItem->listID = invertedTable.listID;
            lexiconTable.push_back(lexiconItem);
            if(lexiconTable.size()>1) {
                lexiconTable[lexiconTable.size()-2]->num = invertedTable.GetDocNumLastWord();
            }
        }        
    }
    
    invertedTable.WriteOutstanding();
    
    if(CURRENT_FILEMODE == FILEMODE_ASCII) {
        return;
    }
    
    lexiconTable.back()->num = invertedTable.GetDocNumLastWord();
    
    WriteLexiconTable(&lexiconTable, CURRENT_FILEMODE);
}
