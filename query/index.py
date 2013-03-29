import config
import struct
from singleton import singleton

class PosItem:
	def __init__(self):
		self.pos = -1
		self.context = -1

class HitItem:
	def __init__(self):
		self.docID = -1
		self.occurence = -1
		#self.poslist = []

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
		hitMap = {}
		lastDocID = 0
		for i in range(occurence):
			hitItem = HitItem()
			(hitItem.docID, hitItem.occurence) = struct.unpack("=II", fb.read(4+4))
			if i == 0:
				lastDocID = hitItem.docID
			else:
				hitItem.docID = hitItem.docID+lastDocID
				lastDocID = hitItem.docID
			#print "docID, occurence:", hitItem.docID, hitItem.occurence
			fb.seek(hitItem.occurence*6,1)	#skip position

			#IGNORE POSITION
			# for j in range(hitItem.occurence):
			# 	posItem = PosItem()
			# 	(posItem.pos, posItem.context) = struct.unpack("<IH", fb.read(4+2))
			# 	hitItem.poslist.append(posItem)
				#print "pos, context:", posItem.pos, posItem.context
			hitMap[hitItem.docID] = hitItem
		return hitMap
def test():
	a = IndexTable()
	l = a.GetIndex(0, 97964104,361)
	print len(l)
	print l[51459].docID, l[51459].occurence

#test()