crawler_cs6913
==============

Assignment 1 of CS6913, a web crawler

Project source: 
https://github.com/joelai85/crawler_cs6913

File list:
|- config.conf: The configuration of this program.
|- crawler.log: The log file of this program. The log file name can be configured.
|- crawler.py: The entry of this program.
|- crconfig.py: Read the configuration and do the MIME type checks.
|- crdb.py: Save the pages to the HardDisk and read it to the memory.
|- crdownloader.py: Downloader of the crawler.
|- crlogger.py: Return a logger.
|- crparser.py: Parser of the crawler.
|- crrobot.py: Cache the robots for different sites and do the robot rules check for a specific URL. Multi-thread safe.
|- crscheduler.py: Maintain the loaded URLs and the URLs to be downloaded. Multi-thread safe.
|- crsummary.py: Maintain the result list and write to a summry file.
|- googleapi.py: Get the inital URLs from google using google's api.
|- page.py: Maintain the page data structure.
|- singleton.py: Define the singleton attribute.
|- explain.txt: Explain file of this program.
|- readme.txt Readme file of this program.
|- htmlData The folder to save the downloaded pages.

Usage:
python crawler.py [-h] -q QUERY_STRING [QUERY_STRING ...] -n PAGES_NUMBER
example: crawler.py -q poly nyu -n 500
No limitation on the parameters when user input. If the parameter is not correct, the program will tell you.

Special features
1. Use multi-thread to download and parse the pages, in order to accelerate the speed. The speed is around 10-30 pages per second with 20 threads and 10MB internet connection.
2. Use configuration file to config the crawler. 
3. The MIME type filter, MaxThread, MaxCrawlerSeed and log file name are configurable.
4. Use MIME type to filter the pages.
5. Cache robots to improve the performance.
6. Use regrex to match the links so that it can get all the matched links even if the parsing html file is malformed.
7. Compress the pages and save as zip files.
8. Customize HttpHandler to add "gzip, deflate" as the "Accpet-Encoding", to reduce the download time if server supports.
9. Customize HttpHandler to log all the http errors and it will not be stuck or crashed.
10. Print the status as the progress bar.
11. Use singleton pattern for some classes to improve memroy efficiency.
12. Convert the relative url to absolute url and try to fix some malformed urls.
13. Set timeout for download.
14. Try best to deal with exceptions to make it stable.
15. Well code-architecture so that it can be refactored and extended easily.

Shortages
1. Only parse "<a href>" style links.
2. Doesn't deal with character encoding when saving the pages. So when open the downloaded pages some characters may be messed.
3. Doesn't save the current status when the program is forced to quit and it can't resume when restart.
4. May be banned by some sites when using multi-thread.
5. There is character encoding issue when parsing the urls.


To be improved
1. Improve the parser to deal with more cases.
2. Checkthe encoding of the pages to encode it accordingly when saving the pages.
3. Save the current status when forced quit and resume unfinished tasks.
4. Maintain a httpconnection pool to ease the side-effect of multi-thread.
5. Check the url encoding and use correct code when parsing the urls.
6. Refactor to make it can run on distributed machines.