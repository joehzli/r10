import hashlib
import datetime

class Page:
	def __init__(self):
		self.data = None
		self.name = ""
		self.url = ""
		self.size = 0
		self.downloadTime = None
		self.returnCode = -1
	def setData(self, theData):
		self.data = theData
		self.size = len(theData)
		utcDatetime = datetime.datetime.utcnow()
		self.downloadTime = utcDatetime.strftime("%m/%d/%Y %H:%M:%S")

	def setReturnCode(self, theCode):
		self.returnCode = theCode

	def setUrl(self, theUrl):
		md5 = hashlib.md5()
		self.url = theUrl
		md5.update(theUrl)
		self.name = md5.hexdigest()

	def getData(self):
		return self.data

	def getName(self):
		return self.name

	def getUrl(self):
		return self.url

	def getSize(self):
		return self.size

	def getDownloadTime(self):
		return self.downloadTime

	def getReturnCode(self):
		return self.returnCode