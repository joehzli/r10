import config
import struct
from lexicon import LexiconTable
from singleton import singleton

class Posting:
	def __init__(self):
		self.docID = -1
		self.freq = -1
		self.pos = -1
		#self.poslist = []
	def __cmp__(self, other):
		if self.docID < other:
			return -1
		elif self.docID == other:
			return 0
		elif self.docID > other:
			return 1

@singleton
class IndexTable:
	def __init__(self):
		self.files = {}

	def GetIndex(self, fileID, pointer, occurence):
		fileName = config.IndexFile
		if self.files.has_key(fileID) is False:
			fb = open(fileName%(fileID), "rb")
			self.files[fileID] = fb
		fb = self.files[fileID]
		fb.seek(pointer)
		invertedList = []
		lastDocID = 0
		for i in range(occurence):
			posting = Posting()
			(posting.docID, posting.freq, posting.pos) = struct.unpack("=III", fb.read(4+4+4))
			if i == 0:
				lastDocID = posting.docID
			else:
				posting.docID = posting.docID+lastDocID
				lastDocID = posting.docID
			#print "docID, occurence:", hitItem.docID, hitItem.occurence
			invertedList.append(posting)
		return invertedList
def test():
	lexiconTable = LexiconTable()
	lexicon = lexiconTable["hello"]
	indexTable = IndexTable()
	l = indexTable.GetIndex(lexicon.fileID, lexicon.pointer,lexicon.occurence)
	print len(l)
	print l[0].docID, l[0].freq, l[0].pos

#test()