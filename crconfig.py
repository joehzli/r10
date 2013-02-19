import ConfigParser
import sys
from singleton import singleton

CONFIGFILE = "config.conf"

@singleton
class Config:
	_mimeTypeBlackList = None
	_mimeSubTypeBlackList = None
	_mimeSubTypeWhiteList = None
	_suffixBlackList = None
	_logFileName = ""

	def __init__(self):
		configParser = ConfigParser.ConfigParser()
		configParser.read(CONFIGFILE)
		try:
			logFileName = configParser.get("CrawlerConfig", "LogFile")
			mimeTypeBlackListStr = configParser.get("CrawlerConfig", "MimeTypeBlackList")
			mimeSubTypeWhiteListStr = configParser.get("CrawlerConfig", "MimeSubTypeWhiteList")
			mimeSubTypeBlackListStr = configParser.get("CrawlerConfig", "MimeSubTypeBlackList")
			suffixBlackListStr = configParser.get("CrawlerConfig", "SuffixBlackList")
		except Exception, exp:
			print("Config file error. "+str(exp))
			sys.exit()
		else:
			mimeTypeBlackList = []
			mimeSubTypeWhiteList = []
			mimeSubTypeBlackList = []
			suffixBlackList = []
			if not logFileName or len(logFileName) < 1:					
				print("Log file is not configured.")
				sys.exit()
			if len(mimeTypeBlackListStr) > 0:
				mimes = mimeTypeBlackListStr.split(',')
				for item in mimes:
					mimeTypeBlackList.append(item.strip().lower())
			if len(mimeSubTypeWhiteListStr) > 0:
				mimes = mimeSubTypeWhiteListStr.split(',')
				for item in mimes:
					mimeSubTypeWhiteList.append(item.strip().lower())
			if len(mimeSubTypeBlackListStr) > 0:
				mimes = mimeSubTypeBlackListStr.split(',')
				for item in mimes:
					mimeSubTypeBlackList.append(item.strip().lower())
					
			if len(suffixBlackListStr) > 0:
				suffixes = suffixBlackListStr.split(',')
				for item in suffixes:
					suffixBlackList.append(item.strip().lower())
			self._initMimeTypeBlackList(mimeTypeBlackList)
			self._initMimeSubTypeWhiteList(mimeSubTypeWhiteList)
			self._initMimeSubTypeBlackList(mimeSubTypeBlackList)
			self._initSuffixBlackList(suffixBlackList)
			self._initLogFileName(logFileName)

	def _initMimeTypeBlackList(self, theBlackList):
		self._mimeBlackList = theBlackList

	def _initMimeSubTypeBlackList(self, theBlackList):
		self._mimeSubTypeBlackList = theBlackList
		
	def _initMimeSubTypeWhiteList(self, theWhiteList):
		self._mimeSubTypeWhiteList = theWhiteList

	def _initSuffixBlackList(self, theBlackList):
		self._suffixBlackList = theBlackList

	def _initLogFileName(self, theFileName):
		self._logFileName = theFileName

	def getMimeTypeBlackList(self):
		return self._mimeBlackList

	def getMimeSubTypeBlackList(self):
		return self._mimeSubTypeBlackList
	
	def getMimeSubTypeWhiteList(self):
		return self._mimeSubTypeWhiteList

	def getSuffixBlackList(self):
		return self._suffixBlackList

	def getLogFileName(self):
		return self._logFileName
	
	def checkMimeType(self, theMimeType):
		if theMimeType.strip().lower() in self._mimeSubTypeWhiteList:
			return True
		if theMimeType.strip().lower() in self._mimeSubTypeBlackList:
			return False
		types = theMimeType.split("/")
		if len(types) == 2:
			if types[0].strip().lower() in self._mimeBlackList:
				return False
		return True