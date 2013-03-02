# Main file of this program

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

# Get the initial URLs from google 
def initCrawlerSeed(theQuery, theLimit):
	urls = googleapi.search(theQuery, theLimit)
	scheduler = crscheduler.Scheduler()
	for url in urls:
		scheduler.put(url, 0)
	
# Parse and get the parameters user inputs
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
	
	# Get the user inputs
	query = userOptions["query"]
	maxDownload = userOptions["n"]
	
	# Set amount of pages to be downloaded in this task.
	scheduler.setMaxDownload(maxDownload)
	
	# Get the amount of URLs to be fetched from major search engine.
	maxCrawlerSeed = int(config.getMaxCrawlerSeed())
	initCrawlerSeed(query, maxCrawlerSeed)
	
	# Get the MaxThread and initial the downloader
	maxThread = int(config.getMaxThread())
	downloader = crdownloader.Downloader(maxThread)
	
	# working loop
	startTime = time.time()
	while(scheduler.downloadedAmount() < maxDownload):
		# To print the status in the same line so that looks like a progress bar.
		sys.stdout.write("Running threads: %d, downloaded pages: %d             \r"%(threading.active_count(),scheduler.downloadedAmount()))
		sys.stdout.flush()
		
		# If not URL in the queue and not URL is downloading, quit
		if scheduler.size() == 0 and threading.active_count() == 1:
			break
		
		# If downloading URLs are less than the left URL to be downloaded, get new URL from the scheduler to downlaod.
		# Do this check to avoid the total download pages more than the number user inputs
		if (threading.active_count()-1) < (maxDownload - scheduler.downloadedAmount()):		
			url, depth = scheduler.get()
			if url is not None:
				downloader.download(url, depth)
	endTime = time.time()
	duration = endTime -startTime
	
	# Write the results and statics to a log.
	summary = crsummary.CRSummary()
	summary.saveSummary(urllib.unquote(query), maxDownload, duration)
	sys.stdout.write("Running threads: %d, downloaded pages: %d                 \n"%(threading.active_count(),scheduler.downloadedAmount()))
	sys.stdout.flush()
	print "Done."
	print "Total time: %2.f seconds\n"%(duration)

if __name__ == "__main__":
	main()