import crscheduler
from HTMLParser import HTMLParser
import urlparse
import page
import crlogger

class CRHtmlParser(HTMLParser):
	_pageUrl = ""
	def __init__(self, theUrl):
		HTMLParser.__init__(self)
		self._pageUrl = theUrl
	def _combineFullUrl(self, theUrl):
		pass
	def handle_starttag(self, tag, attrs):
		if tag == "a":
			if len(attrs) < 1:
				pass
			else:
				for (name, value) in attrs:
					if name == "href":
						# to check whether this is a relative url, 
						# if it's relative, convert it to absolute url
						extractedUrl = value
						parsedUrlResult = urlparse.urlparse(extractedUrl)
						isRelative = False
						if parsedUrlResult.netloc is None or len(parsedUrlResult.netloc) == 0:
							isRelative = True
						if isRelative:
							extractedUrl = urlparse.urljoin(self._pageUrl, extractedUrl)
						scheduler = crscheduler.Scheduler()
						scheduler.put(extractedUrl)


class Parser:
	def parseHtml(self, thePage):
		pageUrl = thePage.getUrl()
		htmlParser = CRHtmlParser(pageUrl)
		try:
			htmlParser.feed(thePage.getData())
			htmlParser.close()
		except Exception, exp:
			logger = crlogger.Logger()
			logger.error("Parser Error. Url:"+pageUrl+" FileName:"+thePage.getName()+" "+str(exp))

	def parseJs(self, theJs):
		scheduler = crscheduler.Scheduler()
		url = "http://www.baidu.com"
		scheduler.put(url)

html_code = """
    <a href="http://www.google.com"> google.com</a>
    <A Href="http://www.pythonclub.org"> PythonClub </a>
    <A Href="/2.gif"> 2222 </a>
    <A HREF = "www.sina.com.cn" Sina </a>
    """
# parser = Parser()
# page = page.Page()
# page.setUrl("http://localhost/abc/sdfsdf/shit/")
# page.setData(html_code)
# parser.parseHtml(page)
# scheduler = crscheduler.Scheduler()
# print scheduler.size()
# while(scheduler.size() > 0):
# 	print scheduler.get()