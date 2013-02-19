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
		#print self.queue
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
		#print self.queue
		return item
	def qsize(self) :
		self.mutex.acquire()
		result = self.size
		self.mutex.release()
		return result
	
	def qprint(self):
		print self.queue
		

@singleton
class Scheduler:
	_maxdownload = 0
	_downloaded = 0

	def __init__(self):
		self.queue = CRQueue()
		self.mutex = threading.Lock()
		self.downloaded = {} # Python's dict is already thread-safe

	def size(self):
		return self.queue.qsize()

	def put(self, theUrl):
		self.mutex.acquire()
		isDuplicated = self.downloaded.has_key(theUrl)
		self.mutex.release()
		if isDuplicated:
			return
		else:
			self.queue.put(theUrl)
			self.mutex.acquire()
			self.downloaded[theUrl] = -1
			self.mutex.release()

	def get(self):
		result = self.queue.get()
		return result

	def setMaxDownload(self, theMax):
		self._maxdownload = theMax

	def getMaxDownload(self):
		return self._maxdownload

	def downloadedAmount(self):
		self.mutex.acquire()
		result = self._downloaded
		self.mutex.release()
		return result

	def setDownloaded(self, theUrl):
		md5 = hashlib.md5()
		md5.update(theUrl)
		hashedUrl = md5.hexdigest()	# It's also the page name
		self.mutex.acquire()
		self.downloaded[theUrl] = hashedUrl
		self._downloaded +=1
		self.mutex.release()
