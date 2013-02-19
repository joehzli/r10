import crscheduler
import crparser
import page
import crdb
import urllib2
import gzip
import zlib
import StringIO

# Reference: http://www.pythonclub.org/python-network-application/observer-spider

def deflate(data):
	try:
		return zlib.decompress(data, -zlib.MAX_WBITS)
	except zlib.error:
		return zlib.decompress(data)

# to accept gzip/deflate encoding
# handle http errors
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
	def download(self, theUrl):
		print "downloading url: "+theUrl
		crHttpHandler = CRHttpHandler
		opener = urllib2.build_opener(crHttpHandler, urllib2.HTTPHandler)
		response = opener.open(theUrl)
		statusCode = response.getcode()
		pageData = response.read()
		newPage = page.Page()
		newPage.setData(pageData)
		newPage.setUrl(theUrl)
		newPage.setReturnCode(statusCode)
		f = open("a.txt", "w")
		f.write(pageData)
		f.close()
		crdb.savePage(newPage)
		scheduler = crscheduler.Scheduler()
		scheduler.setDownloaded(theUrl)
		parser = crparser.Parser()
		parser.parseHTMLWithReg(newPage)