import ConfigParser
import sys
import crlogger
from singleton import singleton

CONFIGFILE = "config.conf"

@singleton
class config:
	_mimeTypeBlackList = None
	_mimeSubTypeWhiteList = None
	_suffixBlackList = None
	_logFileName = ""

	def __init__(self):
		logger = crlogger.Logger()
		logger.info("Initial configuration success.")
		configParser = ConfigParser.ConfigParser()
		configParser.read(CONFIGFILE)
		try:
			logFileName = configParser.get("CrawlerConfig", "LogFile")
			mimeTypeBlackListStr = configParser.get("CrawlerConfig", "MimeTypeBlackList")
			mimeSubTypeWhiteListStr = configParser.get("CrawlerConfig", "MimeSubTypeWhiteList")
			suffixBlackListStr = configParser.get("CrawlerConfig", "SuffixBlackList")
		except Exception, exp:
			logger.error("Config file error. "+str(exp))
			sys.exit()
		else:
			mimeTypeBlackList = []
			mimeSubTypeWhiteList = []
			suffixBlackList = []
			if not logFileName or len(logFileName) < 1:					
				logger.error("Log file is not configured.")
				sys.exit()
			if len(mimeTypeBlackListStr) > 0:
				mimes = mimeTypeBlackListStr.split(',')
				for item in mimes:
					mimeTypeBlackList.append(item.strip())
			if len(mimeSubTypeWhiteListStr) > 0:
				mimes = mimeSubTypeWhiteListStr.split(',')
				for item in mimes:
					mimeSubTypeWhiteList.append(item.strip())
			if len(suffixBlackListStr) > 0:
				suffixes = suffixBlackListStr.split(',')
				for item in suffixes:
					suffixBlackList.append(item.strip())
			self._initMimeTypeBlackList(mimeTypeBlackList)
			self._initMimeSubTypeWhiteList(mimeSubTypeWhiteList)
			self._initSuffixBlackList(suffixBlackList)
			self._initLogFileName(logFileName)

	def _initMimeTypeBlackList(self, theBlackList):
		self._mimeBlackList = theBlackList

	def _initMimeSubTypeWhiteList(self, theWhiteList):
		self._mimeSubTypeWhiteList = theWhiteList

	def _initSuffixBlackList(self, theBlackList):
		self._suffixBlackList = theBlackList

	def _initLogFileName(self, theFileName):
		self._logFileName = theFileName

	def getMimeTypeBlackList(self):
		return self._mimeBlackList

	def getMimeSubTypeWhiteList(self):
		return self._mimeSubTypeWhiteList

	def getSuffixBlackList(self):
		return self._suffixBlackList

	def getLogFileName(self):
		return self._logFileName