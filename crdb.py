import page
import crlogger

def savePage(thePage):
	if thePage.size > 0 and thePage.data is not None and thePage.name is not "":
		fi = open(thePage.name, "w")
		fi.write(thePage.data)
		fi.close()

# thePageName is the hashed URL, return page.data only
def getPage(thePageName):
	data = ""
	logger = crlogger.Logger()
	try:
		fi = open(thePageName, "r")
		data = fi.read()
	except IOError, exp:
		logger.error("Can't open file:'"+thePageName+"'. "+str(exp))
	else:
		fi.close()
	return data

a = getPage("abc")
print a == ""

