import crscheduler
import crparser
import crlogger
import crsummary
import crconfig
import crrobot
import page
import crdb
import urllib2
import gzip
import zlib
import StringIO
import socket
import threading
import sys

# Reference: http://www.pythonclub.org/python-network-application/observer-spider
HTTP_TIMEOUT = 10

def deflate(data):
	try:
		return zlib.decompress(data, -zlib.MAX_WBITS)
	except zlib.error:
		return zlib.decompress(data)

# To accept gzip/deflate encoding
class CRHttpHandler(urllib2.BaseHandler):
	def http_request(self, request):
		request.add_header("Accept-Encoding", "gzip, deflate")
		request.add_header("User-Agent", "CRCrawler")
		return request

	def http_response(self, request, response):
		old_response = response
		if response.headers.get("content-encoding") == "gzip":
			gzipFile = gzip.GzipFile(fileobj=StringIO.StringIO(response.read()),mode="r")
			response = urllib2.addinfourl(gzipFile, old_response.headers, old_response.url, old_response.code)
			response.msg = old_response.msg
		if response.headers.get("content-encoding") == "deflate":
			gzipFile = StringIO.StringIO(deflate(response.read()) )
			response = urllib2.addinfourl(gzipFile, old_response.headers, old_response.url, old_response.code)
			response.msg = old_response.msg
		return response

class Downloader:
	def __init__(self, theMaxThread):
		self.sema = threading.BoundedSemaphore(theMaxThread)
		
	def download(self, theUrl, theDepth):
		# Acquire the sema to control the total running threads.
		self.sema.acquire()
		threading.Thread(target = self._threadDownload, args=(theUrl, theDepth,)).start()
		
	def _threadDownload(self, theUrl, theDepth):
		summary = crsummary.CRSummary()
		
		# Set timeout
		socket.setdefaulttimeout(HTTP_TIMEOUT)
		robot = crrobot.CRRobot()
		
		# To check if this url is allowed by robot.txt
		# Dont forget to release sema before return
		if robot.CheckUrl(theUrl) is False:
			self.sema.release()
			return
		#print "downloading url: "+theUrl
		logger = crlogger.Logger()
		crHttpHandler = CRHttpHandler
		opener = urllib2.build_opener(crHttpHandler, urllib2.HTTPHandler)
		try:
			response = opener.open(theUrl)
			pageData = response.read()
		# Deal with the Http Error
		except urllib2.HTTPError, exp:
			logger.error("HTTP Error. Url:"+theUrl+" "+str(exp))
			summary.add(theUrl, theDepth, "", 0, exp.getcode(), True)
		# Deal with other exceptions, such as timeout
		except Exception, exp:
			logger.error("Download Error. Url:"+theUrl+" "+str(exp))
			summary.add(theUrl, theDepth, "", 0, "OTHER_EXCEPTION", False)
		else:
			statusCode = response.getcode()
			
			# Check the MimeType
			mimeType = response.info().getheader('Content-Type')
			config = crconfig.Config()
			isValidMimeType = False
			if mimeType is not None:
				isValidMimeType = config.checkMimeType(mimeType)
			if isValidMimeType is False:
				logger.error("Download Error. Url:"+theUrl+" MimeType is not Valid")
				summary.add(theUrl, theDepth, "", 0, "OTHER_EXCEPTION", False)
			else:
				newPage = page.Page()
				newPage.setData(pageData)
				newPage.setUrl(theUrl)
				newPage.setReturnCode(statusCode)
				crdb.savePage(newPage)
				summary.add(theUrl, theDepth, newPage.getName(), newPage.getSize(), newPage.getReturnCode(), False)
				scheduler = crscheduler.Scheduler()
				scheduler.setDownloaded(theUrl)
				parser = crparser.Parser()
				parser.parseHTMLWithReg(newPage, theDepth)
		finally:
			# Must release it, otherwise the program will be stuck.
			self.sema.release()