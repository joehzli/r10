from urltable import URLTable
from lexicon import LexiconTable
from index import IndexTable
from singleton import singleton
#import parser
from readpage import PageReader
import bm25
from pagerank import PageRank
from operator import itemgetter, attrgetter
import cgi

class ResultItem:
	def __init__(self):
		self.bm25 = 0
		self.pagerank = 0
		self.score = 0
		self.url = ""
		self.title = ""
		self.snippet = ""
		self.docID = 0

@singleton
class Query:
	def __init__(self):
		self.indexCache = {}

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
		index = indexTable.GetIndex(lexicon.fileID, lexicon.pointer,lexicon.occurence)
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

	def getIndex(self, word):
		index = self.__getFromCache(word)
		if index is None:
			index = self.__getFromFile(word)
		return index

	def getSnippet(self, docID, url, words):
		pageReader = PageReader()
		page = pageReader.ReadPage(docID)
		length = len(page)
		snippet = []
		for word in words:
			startIndex = page.lower().find(word.lower())
			if startIndex < 0:
				startIndex = url.lower().find(word.lower())
				if startIndex <0:
					print startIndex, docID, word
				length = len(url)
				startIndex = 0 if startIndex-20 < 0 else startIndex-20
				endIndex = length-1 if startIndex+50>length else startIndex+50
				snippet.append(url[startIndex:endIndex])
				snippet.append("......")
			else:
				startIndex = 0 if startIndex-50<0 else startIndex-50
				endIndex = length-1 if startIndex+100>length else startIndex+100
				snippet.append(page[startIndex:endIndex].strip().replace("\n","").replace("\r",""))
				snippet.append("......")
		return ''.join(snippet)

	def queryWords(self, query, start, limit):		
		lexiconTable = LexiconTable()
		urlTable = URLTable()
		words = self.parseQuery(query)
		sets = []
		indexMap = {}
		for word in words:
			index = self.getIndex(word)
			docset = set([])
			if index is not None:
				docset = set(index.keys())
			indexMap[word] = index
			sets.append(docset)
		resultset = set.intersection(*sets)
		pagerank = PageRank()
		resultSize = len(resultset)
		if start > resultSize-1:
			return []
		queryResult = []
		for docID in resultset:
			resultItem = ResultItem()
			resultItem.docID = docID
			resultItem.url = urlTable[docID].url
			bm25Score = 0
			for word in words:
				bm25Score += bm25.getBM25(indexMap[word][docID].occurence, lexiconTable[word.lower()].occurence, urlTable.N, urlTable[docID].pagesize, urlTable.avgdl)
			resultItem.bm25 = bm25Score
			#resultItem.pagerank = pagerank.getRank(url.lower())
			resultItem.score = resultItem.bm25#*0.8 + resultItem.pagerank*0.2
			queryResult.append(resultItem)
		queryResult = sorted(queryResult, key=attrgetter('score'), reverse=True)
		startIndex = 0 if start < 0 else start
		endIndex = startIndex + limit
		endIndex = resultSize if endIndex > resultSize else endIndex
		print startIndex, endIndex
		queryResult = queryResult[startIndex:endIndex]
		for item in queryResult:
			item.snippet = cgi.escape(self.getSnippet(item.docID, item.url, words))
		return queryResult

def Test():
	words = "fuck china"
	q = Query()
	result = q.queryWords(words, 0, 10)
	print len(result)
	for item in result:
		print item.url, item.score, item.snippet

#Test()
