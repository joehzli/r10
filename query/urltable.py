import config
import struct
from singleton import singleton

class URLItem:
	def __init__(self):
		self.url = ""
		self.fileID = -1
		self.pointer = -1
		self.pagesize = -1  # -1 means read until the end of the datafile
		self.dl = -1

@singleton
class URLTable:
	def __init__(self):
		self.array = []
		self.avgdl = 0
		self.N = 0
		self.LoadData()

	def __getitem__(self, docID):
		return self.array[docID]

	def LoadData(self):
		print "start loading urltable..."
		fileName = config.URLFile
		totalWordCount = 0
		with open(fileName, "rb") as fb:
			while True:
				bytes = fb.read(struct.calcsize('=H'))
				if not bytes:
					break;
				(wordLen,) = struct.unpack("=H", bytes)
				urlItem = URLItem()
				urlItem.url = str(fb.read(wordLen))
				(urlItem.fileID, urlItem.pointer, urlItem.dl) = struct.unpack("=HII", fb.read(struct.calcsize('=HII')))
				length = len(self.array)
				if(length > 0):
					lastItem = self.array[length-1]
					lastItem.pagesize = urlItem.pointer - lastItem.pointer
					if lastItem.pagesize < 0:
						lastItem.pagesize = -1
				totalWordCount+=urlItem.dl
				self.array.append(urlItem)
		self.avgdl = totalWordCount/len(self.array)
		self.N = len(self.array)
		print "urltable size:",self.N

	def GetDoc(self, docID):
		return self.array[docID]

	def Length(self):
		return self.N

def test():
	a = URLTable()
	print a[14799].url

#test()
# print a.N, a.avgdl
# print a[l-1].url, a[l-1].fileID, a[l-1].pointer, a[l-1].pagesize
# print a[l-2].url, a[l-2].fileID, a[l-2].pointer, a[l-2].pagesize
# print a[l-3].url, a[l-3].fileID, a[l-3].pointer, a[l-3].pagesize
# print a[0].url, a[0].fileID, a[0].pointer, a[0].pagesize
# print a[1].url, a[1].fileID, a[1].pointer, a[1].pagesize
