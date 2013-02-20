import zipfile
import crlogger

def savePage(thePage):
	subFolder = "htmlData/"
	logger = crlogger.Logger()
	if thePage.size > 0 and thePage.data is not None and len(thePage.name) > 0:
		try:
			zipFile = zipfile.ZipFile(subFolder+thePage.name+".zip", mode="w")
			zipFile.writestr(thePage.name+".html", thePage.data)
			zipFile.close()
		except Exception, exp:
			logger.error("Can't save file:'"+thePage.name+"'.zip "+str(exp))

# thePageName is the hashed URL, return page data only
def getPage(thePageName):
	data = ""
	logger = crlogger.Logger()
	try:
		zipFile = zipfile.ZipFile(thePageName+".zip")
		data = zipFile.read(thePageName+".html")
	except Exception, exp:
		logger.error("Can't open file:'"+thePageName+"'. "+str(exp))
	else:
		zipFile.close()
	return data

