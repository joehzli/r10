from singleton import singleton
import datetime

# Summary format:
# DOWNLOAD_TIME, URL, FILE_NAME, SIZE, RETURN_CODE
# ...
# ...
# Statics:
# The N user input:
# Total downloaded pages:
# Total http errors:
# Total other errors:
# Total time:
# Total size:

@singleton
class CRSummary:
    def __init__(self):
        self.records = []
        self.n = 0
    
    def add(self, theUrl, theFileName, theSize, theReturnCode, theHasHttpError):
        utcDatetime = datetime.datetime.utcnow()
        downloadTime = utcDatetime.strftime("%m/%d/%Y %H:%M:%S")
        record = {"downloadtime":downloadTime, "url":theUrl, "filename":theFileName, "size":theSize, "code":theReturnCode, "hashttperror":theHasHttpError}
        self.records.append(record)
    
    def saveSummary(self, theQuery, theN, theTotalTime):
        utcDatetime = datetime.datetime.utcnow()
        fileName = utcDatetime.strftime("%Y-%m-%d_%H%M%S.log")
        f = open(fileName, "w")
        totalSize = 0
        totalDownloaded = 0
        totalHttpError = 0
        for record in self.records:
            downloadtime = str(record["downloadtime"])
            url = str(record["url"])
            fileName = ""
            if len(record["filename"]) > 0:
                fileName = str(record["filename"])+".zip"
                totalDownloaded +=1
            if record["hashttperror"] is True:
                totalHttpError +=1
            size = str(record["size"])
            code = str(record["code"])
            totalSize += record["size"]/1024.0/1024.0
            f.write(downloadtime+", "+url+", "+fileName+", "+size+", "+code+"\n")
        f.write("Statics:\n")
        f.write("Query words: "+theQuery+"\n")
        f.write("The N user input: "+str(theN)+"\n")
        f.write("Total tried download: "+str(len(self.records))+"\n")
        f.write("Total downloaded pages: "+str(totalDownloaded)+"\n")
        f.write("Total http errors: "+str(totalHttpError)+"\n")
        f.write("Total other errors: "+str(len(self.records)-totalDownloaded-totalHttpError)+"\n")
        f.write("Total time: %2.f seconds\n"%(theTotalTime))
        f.write("Total size: %.2f MB\n"%(totalSize))
        f.close()
            