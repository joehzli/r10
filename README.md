R10 - a web search engine
=========================

Assignment of CS6913


Crawler File List
---------
<br>|- config.conf: The configuration file of this program.
<br>|- crawler.log: The log file of this program. The log file name can be configured.
<br>|- crawler.py: The entry of this program.
<br>|- crconfig.py: Read the configuration and do the MIME type checks.
<br>|- crdb.py: Save the pages to the HardDisk and read it to the memory.
<br>|- crdownloader.py: Downloader of the crawler.
<br>|- crlogger.py: Return a logger.
<br>|- crparser.py: Parser of the crawler.
<br>|- crrobot.py: Cache the robots for different sites and do the robot rules check for a specific URL. Multi-thread safe.
<br>|- crscheduler.py: Maintain the loaded URLs and the URLs queue to be downloaded. Multi-thread safe.
<br>|- crsummary.py: Maintain the result list and write to a summry file.
<br>|- googleapi.py: Get the inital URLs from google using google's api.
<br>|- page.py: Maintain the page data structure.
<br>|- singleton.py: Define the singleton attribute.
<br>|- explain.txt: Explain file of this program.
<br>|- readme.txt Readme file of this program.
<br>|- htmlData The folder to save the downloaded pages. I do NOT put the downloaded pages here to reduce the package size.
<br>
<br>Crawler Usage
------
<br>python crawler.py [-h] -q QUERY_STRING [QUERY_STRING ...] -n PAGES_NUMBER
<br>example: 
<br>python crawler.py -q poly nyu -n 500
