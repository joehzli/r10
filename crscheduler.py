import Queue
import hashlib
import threading
from singleton import singleton

# thread safe Queue
class CRQueue:
	def __init__(self) :
		self.mutex = threading.Lock()
		self.queue = []
		self.size = 0
	def put(self, item) :
		self.mutex.acquire()
		self.queue.append(item)
		self.size += 1
		self.mutex.release()
	def get(self) :
		self.mutex.acquire()
		if self.size == 0:
			item = None
		else:
			item = self.queue[0]
			del self.queue[0]
			self.size -= 1
		self.mutex.release()
		return item
	def qsize(self) :
		self.mutex.acquire()
		result = self.size
		self.mutex.release()
		return result

@singleton
class Scheduler:
	_maxdownload = 0

	def __init__(self):
		self.queue = CRQueue()
		self.downloaded = {} # Python's dict is already thread-safe

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

	def getMaxDownload(self):
		return self._maxdownload

	def downloadedAmount(self):
		return len(self.downloaded)

	def setDownloaded(self, theUrl):
		md5 = hashlib.md5()
		md5.update(theUrl)
		hashedUrl = md5.hexdigest()	# It's also the page name
		self.downloaded[theUrl] = hashedUrl
