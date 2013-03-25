import config
import struct
from singleton import singleton

class Lexicon:
	def __init__(self):
		self.occurence = 0
		self.fileID = 0
		self.pointer = 0

@singleton
class LexiconTable:
	def __init__(self):
		self.dict = {}

	def __getitem__(self, word):
		if(self.dict.has_key(word)):
			return self.dict[word]
		else:
			return None

	def LoadData(self):
		fileName = config.LexiconFile
		with open(fileName, "rb") as fb:
			while True:
				bytes = fb.read(struct.calcsize('=H'))
				if not bytes:
					break
				(wordLen,) = struct.unpack("=H", bytes)
				word =str(fb.read(wordLen))
				lexicon = Lexicon()
				(lexicon.occurence,lexicon.fileID, lexicon.pointer) = struct.unpack("=IHI", fb.read(struct.calcsize('=IHI')))
				self.dict[word] = lexicon
	def GetLexicon(self, word):
		return self.dict[word]

	def Length(self):
		return len(self.dict)
b = LexiconTable()
b.LoadData()
def test():
	a = LexiconTable()
	print a.Length()
	litem = a["fuck"]
	print litem.fileID, litem.pointer, litem.occurence

test()