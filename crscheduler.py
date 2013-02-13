import Queue
import hashlib
from singleton import singleton

@singleton
class Scheduler:
	_maxdownload = 0

	def __init__(self):
		self.queue = Queue.Queue()
		self.downloaded = {}

	def size(self):
		return self.queue.qsize()

	def put(self, theUrl):
		if(self.downloaded.has_key(theUrl)):
			return
		else:
			self.queue.put(theUrl)

	def get(self):
		return self.queue.get()

	def setMaxDownload(self, theMax):
		self._maxdownload = theMax

	def downloadedAmount(self):
		return len(self.downloaded)

	def setDownloaded(self, theUrl):
		md5 = hashlib.md5()
		md5.update(theUrl)
		hashedUrl = md5.hexdigest()	# It's also the page name
		self.downloaded[theUrl] = hashedUrl
