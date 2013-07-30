from urltable import URLTable
from lexicon import LexiconTable
from index import IndexTable
from singleton import singleton
import bisect
import time
from readpage import PageReader
import bm25
#from pagerank import PageRank
from operator import itemgetter, attrgetter
import cgi
import config

class ResultItem:
	def __init__(self):
		self.bm25 = 0
		self.score = 0
		self.url = ""
		self.title = ""
		self.snippet = ""
		self.pos = []
		self.docID = 0

class PList:
	def __init__(self, invertedList):
		self.invertedList = invertedList
		self.cursor = 0
		if invertedList is None:
			self.size = 0
		else:
			self.size = len(invertedList)

@singleton
class Query:
	def __init__(self):
		self.indexCache = {}
		self.snippetCache = {}

	def __getFromCache(self, word):
		if self.indexCache.has_key(word):
			return self.indexCache[word]
		return None

	def __getFromFile(self, word):
		lexiconTable = LexiconTable()
		lexicon = lexiconTable[word.lower()]
		if lexicon is None:
			return None
		indexTable = IndexTable()
		index = indexTable.GetIndex(lexicon.fileID, lexicon.pointer, lexicon.size, lexicon.occurence)
		self.indexCache[word] = index
		return index

	def parseQuery(self, query):
		querylist = list(query)
		querylist = [" " if not ch.isalnum() else ch for ch in querylist]
		tmplist=''.join(querylist).split(" ")
		words = []
		for word in tmplist:
			if len(word) > 0:
				words.append(word)
		return words

	def getSnippetFromCache(self, docID, pos):
		key = "d"+str(docID)+"p"+str(pos)
		if self.snippetCache.has_key(key):
			return self.snippetCache[key]
		return None

	def getSnippetFromPage(self, docID, page, pos):
		if pos == 0: #means the word is in url, just return empty body
			return ""
		length = len(page)
		snippet = []
		startIndex = 0 if pos-50<0 else pos-50
		endIndex = length-1 if pos+100>length-1 else pos+100
		snippet.append(page[startIndex:endIndex].strip().replace("\n","").replace("\r",""))
		snippet.append("......")
		snippet = ''.join(snippet)
		self.snippetCache["d"+str(docID)+"p"+str(pos)] = snippet
		return snippet

	def openList(self, word):
		index = self.__getFromCache(word)
		if index is None:
			index = self.__getFromFile(word)
		pList = PList(index)
		return pList

	def nextGEQ(self, pList, docID):
		start = pList.cursor
		# i = bisect.bisect_left(pList.invertedList, docID, lo=start)
		# if i < start:
		# 	print "error in nextGEQ"
		# elif i == pList.size:
		# 	print "error in nextGEQ"
		# elif pList.invertedList[i].docID >= docID:
		# 	pList.cursor = i+1
		# 	return pList.invertedList[i].docID
		for i in range(start, pList.size):
			pList.cursor += 1
			if pList.invertedList[i].docID >= docID:
				return pList.invertedList[i].docID
		return config.MAXDID

	def getFreq(self, pList, docID):
		if pList.invertedList[pList.cursor-1].docID == docID:
			return pList.invertedList[pList.cursor-1].freq
		print "error, something wrong in getFreq"
		return -1

	def getPos(self, pList, docID):
		if pList.invertedList[pList.cursor-1].docID == docID:
			return pList.invertedList[pList.cursor-1].pos
		print "error, something wrong in getPos"
		return -1

	def daatQuery(self, words):
		queryResult = []
		urlTable = URLTable()
		lexiconTable = LexiconTable()
		num = len(words)
		print "nlist:", num
		lp = []
		openListStart = time.clock()
		for i in range(num):
			lp.append(self.openList(words[i]))
		print "openlist time:",str(time.clock()-openListStart)
		sortStart = time.clock()
		bm25Time = 0
		lp.sort(key=attrgetter('size'))
		did = 0
		print "sort list time:",str(time.clock()-sortStart)
		while did < config.MAXDID:
			did = self.nextGEQ(lp[0] ,did)
			if did == config.MAXDID:
				break
			d = None
			for i in range(1, num):
				d = self.nextGEQ(lp[i], did)
				if d != did:
					break
			if d is not None and d > did:
				did = d
			else:
				resultItem = ResultItem()
				resultItem.docID = did
				resultItem.url = urlTable[did].url
				score =0
				bm25Start = time.clock()
				for i in range(num):
					freq = self.getFreq(lp[i], did)
					score += bm25.getBM25(freq, lp[i].size, urlTable.N, urlTable[did].dl, urlTable.avgdl)
					pos = self.getPos(lp[i], did)
					resultItem.pos.append(pos)
				bm25Time += time.clock()-bm25Start
				resultItem.bm25 = score
				resultItem.score = resultItem.bm25
				queryResult.append(resultItem)
				did+=1
		print "bm25time: ", bm25Time
		return queryResult


	def queryWords(self, query, start, limit):		
		lexiconTable = LexiconTable()
		urlTable = URLTable()
		pageReader = PageReader()
		words = self.parseQuery(query)
		begin = time.clock()
		queryResult = self.daatQuery(words)
		print "get result & bm25 time:",str(time.clock()-begin)
		resultSize = len(queryResult)
		print "result size:",resultSize
		if start > resultSize-1:
			return [], 0
		begin = time.clock()
		queryResult.sort(key=attrgetter('score'), reverse=True)
		print "sort BM25 time:",str(time.clock()-begin)
		startIndex = 0 if start < 0 else start
		endIndex = startIndex + limit
		endIndex = resultSize if endIndex > resultSize else endIndex
		print "start index & end index:", startIndex, endIndex
		queryResult = queryResult[startIndex:endIndex]
		begin = time.clock()
		for resultItem in queryResult:
			page = None
			snippet = ""
			for i in range(len(words)):
				if  resultItem.pos[i] > 0:
					tmpSnap = self.getSnippetFromCache(resultItem.docID, resultItem.pos[i])
					if tmpSnap is None:
						if page is None:
							page = pageReader.ReadPage(resultItem.docID)
						tmpSnap = self.getSnippetFromPage(resultItem.docID, page, resultItem.pos[i])
					snippet += tmpSnap
			resultItem.snippet = cgi.escape(snippet)
		print "snippet time:",str(time.clock()-begin)
		return queryResult, resultSize

def Test():
	words = "hello world"
	q = Query()
	result, size = q.queryWords(words, 0, 10)
	print size
	for item in result:
		print item.url, item.score, item.snippet

#Test()
