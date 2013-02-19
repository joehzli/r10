import crscheduler
import urlparse
import page
import crlogger
import re


class Parser:
	_regForHref = r'<a[^<>]*?href=[\'\"]?([^\'\" >]+)'	# the result of this regex is as same as parsing with BeautifulSoup, much better than HTMLParser
	
	def parseHTMLWithReg(self, thePage):
		data = thePage.getData()
		pageUrl = thePage.getUrl()
		urls = re.findall(self._regForHref, data, re.S|re.I)
		for url in urls:
			try:
				extractedUrl = url
				parsedUrlResult = urlparse.urlparse(extractedUrl)
				currentScheme = urlparse.urlparse(pageUrl).scheme
				isRelative = False
				if parsedUrlResult.netloc is None or len(parsedUrlResult.netloc) == 0:
					isRelative = True
				if isRelative:
					extractedUrl = urlparse.urljoin(pageUrl, extractedUrl)
				elif parsedUrlResult.scheme is None or len(parsedUrlResult.scheme) == 0:
					print currentScheme
					print extractedUrl
					extractedUrl = currentScheme.decode('utf-8')+":".decode('utf-8')+extractedUrl.decode('utf-8') # in case the url is in unicode
				elif len(parsedUrlResult.scheme) > 0 and (parsedUrlResult.scheme != "http" or parsedUrlResult.scheme != "https"):
					continue
				scheduler = crscheduler.Scheduler()
				scheduler.put(extractedUrl)
			except Exception, exp:
				logger = crlogger.Logger()
				logger.error("Parser Error. Url:"+pageUrl+" FileName:"+thePage.getName()+" "+str(exp))	

	def parseJs(self, theJs):
		scheduler = crscheduler.Scheduler()
		url = "http://www.baidu.com"
		scheduler.put(url)