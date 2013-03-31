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

using namespace std;

InvertedTable::InvertedTable()
{
    _mode = CURRENT_FILEMODE;
    _fileID=0;
    sprintf(_outputPath, INVERTEDINDEX_FILE, _fileID);
    _counter = 0;
    _fp = fopen(_outputPath, "a");
}
InvertedTable::~InvertedTable()
{
    fclose(_fp);
}

void InvertedTable::SetFileMode(FILEMODE mode)
{
    _mode = mode;
}

uint16_t InvertedTable::GetFileID()
{
    return _fileID;
}

uint32_t InvertedTable::GetDocNumLastWord()
{
    return _DocNumLastWord;
}

void InvertedTable::write()
{
    if(_mode == FILEMODE_ASCII) {
        if(_counter > MAX_FILE_SIZE) {
            // close current file and open a new file
            fclose(_fp);
            _fileID++;
            sprintf(_outputPath, INVERTEDINDEX_FILE, _fileID);
            _fp = fopen(_outputPath, "a");
            _counter = 0;
        }
        _DocNumLastWord = (uint32_t)_invertedList.size();
        for(int j=0;j<_invertedList.size();j++) {
            DocTuple *docTuple =_invertedList[j];
            _counter += fprintf(_fp,"%u %lu ", docTuple->docID, docTuple->posArray.size());
            for(int i=0;i<docTuple->posArray.size();i++) {
                _counter += fprintf(_fp,"%d ", docTuple->posArray[i]->pos);
                delete docTuple->posArray[i];
                docTuple->posArray[i] = NULL;
            }
            docTuple->posArray.clear();
            delete docTuple;
            _invertedList[j] = NULL;
        }
        _counter += fprintf(_fp, "\n");
        _invertedList.clear();
        vector<DocTuple *> tmp;
        _invertedList.swap(tmp);
    }
    
    if(_mode == FILEMODE_BIN) {
        if(_counter > MAX_FILE_SIZE) {
            fclose(_fp);
            _fileID++;
            sprintf(_outputPath, INVERTEDINDEX_FILE, _fileID);
            _fp = fopen(_outputPath, "a");
            _counter = 0;
        }
        _DocNumLastWord = (uint32_t)_invertedList.size();
        for(int j=0;j<_invertedList.size();j++) {
            DocTuple *docTuple =_invertedList[j];
            uint32_t docListLength =(uint32_t)docTuple->posArray.size();
            _counter += sizeof(uint32_t) * fwrite(&docTuple->docID, sizeof(uint32_t), 1, _fp);
            _counter += sizeof(uint32_t) * fwrite(&docListLength, sizeof(uint32_t),1,_fp);
            for(int i=0;i<docTuple->posArray.size();i++) {
                _counter += sizeof(uint32_t) * fwrite(&docTuple->posArray[i]->pos, sizeof(uint32_t), 1, _fp);
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
        vector<DocTuple *> tmp;
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
    // first insert, new word;
    if(_invertedList.size() == 0) {
        _word = rawPosting->word;
        DocTuple * docTuple = new DocTuple;
        _lastDocID = rawPosting->docID;
        docTuple->actualDocID = rawPosting->docID;
        docTuple->docID = rawPosting->docID;
        Posting *posting = new Posting;
        posting->actualPos = rawPosting->pos;
        posting->pos = rawPosting->pos;
        docTuple->posArray.push_back(posting);
        _invertedList.push_back(docTuple);
    } else if (_word != rawPosting->word){
        //write and free the memory
        write();        
        _word = rawPosting->word;
        DocTuple * docTuple = new DocTuple;
        _lastDocID = rawPosting->docID;
        docTuple->actualDocID = rawPosting->docID;
        docTuple->docID = rawPosting->docID;
        
        Posting *posting = new Posting;
        posting->actualPos = rawPosting->pos;
        posting->pos = rawPosting->pos;
        docTuple->posArray.push_back(posting);
        _invertedList.push_back(docTuple);

        return _counter;
    } else {//insert same word
        // The same doc, compress position
        if(rawPosting->docID == _lastDocID) {
            DocTuple * docTuple = _invertedList.back();
            Posting *posting = new Posting;
            posting->actualPos = rawPosting->pos;
            // compress the position
            posting->pos = (rawPosting->pos - docTuple->posArray.back()->actualPos);
            docTuple->posArray.push_back(posting);
        } else {
            // Different doc, compress the docID
            DocTuple * docTuple = new DocTuple;
            _lastDocID = rawPosting->docID;
            docTuple->actualDocID = rawPosting->docID;
            // compress docID
            docTuple->docID = (rawPosting->docID-_invertedList.back()->actualDocID);
            Posting *posting = new Posting;
            posting->actualPos = rawPosting->pos;
            posting->pos = rawPosting->pos;
            docTuple->posArray.push_back(posting);
            _invertedList.push_back(docTuple);
        }
        
    }
    // 0 means no data written
    return 0;
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
    string lastWord = "";
    uint32_t lastCounter = 0;
    uint32_t count = 0;
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
        
        // first time
        if(lastWord == "") {
            lastWord = posting.word;
            LexiconItem *lexiconItem = new LexiconItem;
            lexiconItem->word = posting.word;
            lexiconItem->invertedPointer = 0;
            lexiconTable.push_back(lexiconItem);
        }
        
        count = invertedTable.Insert(&posting);
        
        // count > 0 means meet new word and there is data written
        if(count > 0) {
            LexiconItem *lexiconItem = lexiconTable.back();
            lexiconItem->fileID = invertedTable.GetFileID();
            lexiconItem->num = invertedTable.GetDocNumLastWord();
            if(count < lastCounter) {
                lexiconItem->invertedPointer = 0;
            }
            
            lexiconItem = new LexiconItem;
            lexiconItem->word = posting.word;
            lastWord = posting.word;
            lexiconItem->invertedPointer = count;
            lexiconTable.push_back(lexiconItem);
            lastCounter = count;
        }
        
    }
    
    count = invertedTable.WriteOutstanding();
    lexiconTable.back()->fileID = invertedTable.GetFileID();
    lexiconTable.back()->num = invertedTable.GetDocNumLastWord();
    if(count > 0 && count < lastCounter) {
        lexiconTable.back()->invertedPointer = 0;
    }
    
    WriteLexiconTable(&lexiconTable, CURRENT_FILEMODE);
}
