import crdownloader
import crscheduler
import crsummary
import crconfig
import googleapi
import argparse
import urllib
import time
import threading
import sys

def initCrawlerSeed(theQuery, theLimit):
	urls = googleapi.search(theQuery, theLimit)
	scheduler = crscheduler.Scheduler()
	for url in urls:
		scheduler.put(url, 0)
		
def parseUserOptions():
	parser = argparse.ArgumentParser(description='A simple web crawler by Hanzhou Li.')
	parser.add_argument("-q", dest = "query", metavar="QUERY_STRING", required= True, type=str, nargs="+",
                   help="The query string.")
	parser.add_argument("-n", dest = "pageN", metavar="PAGES_NUMBER", required= True, type=int,
                   help="An integer for the total pages to be downloaded.")
	arguments = parser.parse_args()
	result = {"query":"", "n":0}
	for word in arguments.query:
		result["query"] += urllib.quote(word)+"+"
	if len(result["query"]) > 0:
		result["query"] = result["query"][:-1]
	result["n"] = arguments.pageN
	return result

def main():
	config = crconfig.Config()
	userOptions = parseUserOptions()
	scheduler = crscheduler.Scheduler()
	query = userOptions["query"]
	maxDownload = userOptions["n"]
	scheduler.setMaxDownload(maxDownload)
	maxCrawlerSeed = int(config.getMaxCrawlerSeed())
	initCrawlerSeed(query, maxCrawlerSeed)
	maxThread = int(config.getMaxThread())
	downloader = crdownloader.Downloader(maxThread)
	startTime = time.time()
	while(scheduler.downloadedAmount() < maxDownload):
		sys.stdout.write("Running threads: %d, downloaded pages: %d             \r"%(threading.active_count(),scheduler.downloadedAmount()))
		sys.stdout.flush()
		if scheduler.size() == 0 and threading.active_count() == 1:
			break
		if (threading.active_count()-1) < (maxDownload - scheduler.downloadedAmount()):		
			url, depth = scheduler.get()
			if url is not None:
				downloader.download(url, depth)
	endTime = time.time()
	duration = endTime -startTime
	summary = crsummary.CRSummary()
	summary.saveSummary(urllib.unquote(query), maxDownload, duration)
	sys.stdout.write("Running threads: %d, downloaded pages: %d                 \n"%(threading.active_count(),scheduler.downloadedAmount()))
	sys.stdout.flush()
	print "Done."
	print "Total time: %2.f seconds\n"%(duration)

if __name__ == "__main__":
	main()