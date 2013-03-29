import gzip, os
import config
from urltable import URLTable
from urltable import URLItem
from singleton import singleton

@singleton
class PageReader:
	def __init__(self):
		self.cache = {}

	def FindFile(self,fileID):
		if self.cache.has_key(fileID):
			return self.cache[fileID]
		i = 0;
		for root, dirnames, filenames in os.walk(config.RawDataFile):
			#print root, filenames
			for filename in filenames:
				if '_data' in filename:
					if i == fileID:
						self.cache[fileID] = os.path.join(root, filename)
						return  os.path.join(root, filename)
					i+=1
		return None

	def ReadGZFile(self, filepath, pointer, pagesize):
		try:
			pages = gzip.open(filepath, 'r')
		except Exception ,e:
			print e
			return None
		pages.seek(pointer)
		page = pages.read(pagesize)
		pages.close()
		return page

	def ReadPage(self, docID):
		urlTable = URLTable()
		urlItem = urlTable[docID]
		fileID = urlItem.fileID
		pointer = urlItem.pointer
		#print pointer
		pagesize = urlItem.pagesize	# the pagesize of the last docID is -1, means to read until the end
		filepath = self.FindFile(fileID)
		#print filepath, pagesize
		if not filepath:
			print "No such file."
			return None
		return self.ReadGZFile(filepath, pointer, pagesize)

#if __name__ == "__main__":
	# print urltable.uTable.Length()
	# print FindFile(4109)
	# print ReadGZFile(FindFile(4109), 380, 100)
	#print ReadGZFile("ABC", 0, 100)
	# print ReadPage(0)
	# print ReadPage(1)
	# reader = PageReader()
	# for i in range(25):
	# 	reader.ReadPage(14799)
	# print ReadPage(1000)