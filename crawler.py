import crparser
import crdownloader
import crscheduler
import googleapi
import argparse
import urllib

GOOGLE_RESULT_LIMIT = 10

def initCrawlerSeed(theQuery, theLimit):
	urls = googleapi.search(theQuery, theLimit)
	scheduler = crscheduler.Scheduler()
	for url in urls:
		scheduler.put(url)
		
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
	userOptions = parseUserOptions()
	scheduler = crscheduler.Scheduler()
	query = userOptions["query"]
	maxDownload = userOptions["n"]
	scheduler.setMaxDownload(maxDownload)
	initCrawlerSeed(query, GOOGLE_RESULT_LIMIT)
	downloader = crdownloader.Downloader()
	while(scheduler.downloadedAmount() < maxDownload):
		url = scheduler.get()
		if url is not None:
			downloader.download(url)


if __name__ == "__main__":
	main()