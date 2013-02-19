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
		#print "Lock CRQueue in put."
		self.mutex.acquire()
		self.queue.append(item)
		#print self.queue
		self.size += 1
		self.mutex.release()
		#print "Unlock CRQueue in put."
	def get(self) :
		#print "Lock CRQueue in get."
		self.mutex.acquire()
		if self.size == 0:
			item = None
		else:
			item = self.queue[0]
			del self.queue[0]
			self.size -= 1
		self.mutex.release()
		#print "Unlock CRQueue in get."
		return item
	def qsize(self) :
		#print "Lock CRQueue in size."
		self.mutex.acquire()
		result = self.size
		self.mutex.release()
		#print "Unlock CRQueue in size."
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

	def put(self, theUrl, theDepth):
		#print "Lock scheduler in put."
		self.mutex.acquire()
		isDuplicated = self.downloaded.has_key(theUrl)
		self.mutex.release()
		#print "Unlock scheduler in put."
		if isDuplicated:
			return
		else:
			self.queue.put(theUrl)
			#print "Lock scheduler in put else."
			self.mutex.acquire()
			self.downloaded[theUrl] = theDepth
			self.mutex.release()
			#print "Unlock scheduler in put else."

	def get(self):
		result = self.queue.get()
		if result is None:
			return None, -1
		return result, self.downloaded[result]

	def setMaxDownload(self, theMax):
		self._maxdownload = theMax

	def getMaxDownload(self):
		return self._maxdownload

	def downloadedAmount(self):
		#print "Lock scheduler in downloadedAmount."
		self.mutex.acquire()
		result = self._downloaded
		self.mutex.release()
		#print "Unlock scheduler in downloadedAmount."
		return result

	def setDownloaded(self, theUrl):
		#print "Lock scheduler in setDownloaded."
		self.mutex.acquire()
		self._downloaded +=1
		self.mutex.release()
		#print "Unlock scheduler in setDownloaded."
