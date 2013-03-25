import config
import struct
from singleton import singleton

class URLItem:
	def __init__(self):
		self.url = ""
		self.fileID = -1
		self.pointer = -1
		self.pagesize = -1

@singleton
class URLTable:
	def __init__(self):
		self.array = []

	def __getitem__(self, docID):
		return self.array[docID]

	def LoadData(self):
		fileName = config.URLFile
		with open(fileName, "rb") as fb:
			while True:
				bytes = fb.read(struct.calcsize('=H'))
				if not bytes:
					break;
				(wordLen,) = struct.unpack("=H", bytes)
				urlItem = URLItem()
				urlItem.url = str(fb.read(wordLen))
				(urlItem.fileID,urlItem.pointer) = struct.unpack("=HI", fb.read(struct.calcsize('=HI')))
				length = len(self.array)
				if(length > 0):
					lastItem = self.array[length-1]
					lastItem.pagesize = urlItem.pointer - lastItem.pointer
				self.array.append(urlItem)

	def GetDoc(self, docID):
		return self.array[docID]

	def Length(self):
		return len(self.array)

a = URLTable()
a.LoadData()
l = a.Length()
print l
print a[l-1].url, a[l-1].fileID, a[l-1].pointer, a[l-1].pagesize
print a[l-2].url, a[l-2].fileID, a[l-2].pointer, a[l-2].pagesize
print a[l-3].url, a[l-3].fileID, a[l-3].pointer, a[l-3].pagesize
print a[0].url, a[0].fileID, a[0].pointer, a[0].pagesize
print a[1].url, a[1].fileID, a[1].pointer, a[1].pagesize
