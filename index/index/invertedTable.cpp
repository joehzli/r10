//
//  invertedTable.cpp
//  index
//
//  Created by Hanzhou Li on 3/2/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#include "invertedTable.h"
#include "stdio.h"

using namespace std;

InvertedTable::InvertedTable()
{
    _mode = CURRENT_FILEMODE;
    _fileID=0;
    sprintf(_outputPath, "data/inverted_%d.index", _fileID);
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
            sprintf(_outputPath, "data/inverted_%d.index", _fileID);
            _fp = fopen(_outputPath, "a");
            _counter = 0;
        }
        _DocNumLastWord = (uint32_t)_invertedList.size();
        for(int j=0;j<_invertedList.size();j++) {
            DocTuple *docTuple =_invertedList[j];
            _counter += fprintf(_fp,"%u %lu ", docTuple->docID, docTuple->posArray.size());
            for(int i=0;i<docTuple->posArray.size();i++) {
                _counter += fprintf(_fp,"%d %d ", docTuple->posArray[i]->pos, docTuple->posArray[i]->context);
                delete docTuple->posArray[i];
            }
            docTuple->posArray.clear();
            delete docTuple;
        }
        _counter += fprintf(_fp, "\n");
    }
    
    if(_mode == FILEMODE_BIN) {
        if(_counter > MAX_FILE_SIZE) {
            fclose(_fp);
            _fileID++;
            sprintf(_outputPath, "data/inverted_%d.index", _fileID);
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
                _counter += sizeof(uint16_t) * fwrite(&docTuple->posArray[i]->context, sizeof(uint16_t), 1, _fp);
                delete docTuple->posArray[i];
            }
            docTuple->posArray.clear();
            delete docTuple;
        }
    }
}

int InvertedTable::WriteOutstanding()
{
    write();
    _invertedList.clear();
    return _counter;
}

int InvertedTable::Insert(RawPosting *rawPosting)
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
        posting->context = rawPosting->context;
        docTuple->posArray.push_back(posting);
        _invertedList.push_back(docTuple);
    } else if (_word != rawPosting->word){
        //write and free the memory
        write();
        _invertedList.clear();
        
        _word = rawPosting->word;
        DocTuple * docTuple = new DocTuple;
        _lastDocID = rawPosting->docID;
        docTuple->actualDocID = rawPosting->docID;
        docTuple->docID = rawPosting->docID;
        
        Posting *posting = new Posting;
        posting->actualPos = rawPosting->pos;
        posting->pos = rawPosting->pos;
        posting->context = rawPosting->context;
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
            posting->context = rawPosting->context;
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
            posting->context = rawPosting->context;
            docTuple->posArray.push_back(posting);
            _invertedList.push_back(docTuple);
        }
        
    }
    // -1 means no data written
    return -1;
}

void freeRawPostingVector(RawPostingVector *vector)
{
    for(int i=0;i<vector->size();i++) {
        RawPosting *posting = (*vector)[i];
        delete posting;
    }
    
    vector->clear();
}

void WriteRawPostingToFile(RawPostingVector *vector, const char* filePath, FILEMODE mode)
{
    FILE *fp = fopen(filePath, "w+");
    if(fp != NULL) {
        std::stable_sort(vector->begin(), vector->end(), CompareRawPostingDocID);
        // must use stable sort in the second round
        std::stable_sort(vector->begin(), vector->end(), CompareRawPostingWord);
        for(int i=0; i<vector->size();i++) {
            RawPosting *posting = (*vector)[i];
            fprintf(fp, "%d %s %d %d\n", posting->docID, posting->word.c_str(), posting->pos, posting->context);
        }
        fclose(fp);
    }
}

int WriteRawPostingToBuffer(char* buffer, RawPostingVector *vector, const char* filePath, FILEMODE mode)
{
    int count = 0;
    std::sort(vector->begin(), vector->end(), CompareRawPostingDocID);
    // must use stable sort in the second round
    std::stable_sort(vector->begin(), vector->end(), CompareRawPostingWord);
    for(int i=0; i<vector->size();i++) {
        RawPosting *posting = (*vector)[i];
        int nRead = 0;
        nRead = sprintf(buffer, "%d %s %d %d\n", posting->docID, posting->word.c_str(), posting->pos, posting->context);
        count +=nRead;
        buffer += nRead;
    }
    return count;
}

bool CompareRawPostingDocID(RawPosting *posting1, RawPosting *posting2)
{
    return posting1->docID < posting2->docID;
}

bool CompareRawPostingWord(RawPosting *posting1, RawPosting *posting2)
{
    return posting1->word < posting2->word;
}