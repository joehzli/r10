import crscheduler
import urlparse
import crlogger
import re


class Parser:
	_regForHref = r'<a[^<>]*?href=[\'\"]?([^\'\" >]+)'	# the result of this regex is as same as parsing with BeautifulSoup, much better than HTMLParser
	
	def _putUrl(self, theUrl):
		#robot = crrobot.CRRobot()
		# to check if this url is allowed by robot.txt
		#if robot.CheckUrl(theUrl):
		scheduler = crscheduler.Scheduler()
		scheduler.put(theUrl)
		
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
					extractedUrl = currentScheme.decode('utf-8')+":".decode('utf-8')+extractedUrl.decode('utf-8') # in case the url is in unicode
				elif len(parsedUrlResult.scheme) > 0 and (parsedUrlResult.scheme != "http" and parsedUrlResult.scheme != "https"):
					continue
				#final check the combined url
				parsedUrlResult = urlparse.urlparse(extractedUrl)
				if parsedUrlResult.netloc is None or len(parsedUrlResult.netloc) == 0:
					continue
				self._putUrl(extractedUrl)
			except Exception, exp:
				logger = crlogger.Logger()
				logger.error("Parser Error. Url:"+pageUrl+" FileName:"+thePage.getName()+" "+str(exp))	

	def parseJs(self, theJs):
		scheduler = crscheduler.Scheduler()
		url = "http://www.baidu.com"
		scheduler.put(url)