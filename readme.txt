Project source: 
https://github.com/joelai85/crawler_cs6913
Author: 
Hanzhou Li
PolyID 0490250

File list
---------
|- 2013-02-19_235450.log
|- 2013-02-19_235545.log
|- 2013-02-19_235749.log
|- 2013-02-20_000041.log
|- config.conf: The configuration file of this program.
|- crawler.log: The log file of this program. The log file name can be configured.
|- crawler.py: The entry of this program.
|- crconfig.py: Read the configuration and do the MIME type checks.
|- crdb.py: Save the pages to the HardDisk and read it to the memory.
|- crdownloader.py: Downloader of the crawler.
|- crlogger.py: Return a logger.
|- crparser.py: Parser of the crawler.
|- crrobot.py: Cache the robots for different sites and do the robot rules check for a specific URL. Multi-thread safe.
|- crscheduler.py: Maintain the loaded URLs and the URLs queue to be downloaded. Multi-thread safe.
|- crsummary.py: Maintain the result list and write to a summry file.
|- googleapi.py: Get the inital URLs from google using google's api.
|- page.py: Maintain the page data structure.
|- singleton.py: Define the singleton attribute.
|- explain.txt: Explain file of this program.
|- readme.txt Readme file of this program.
|- htmlData The folder to save the downloaded pages. I do NOT put the downloaded pages here to reduce the package size.

Usage
------
python crawler.py [-h] -q QUERY_STRING [QUERY_STRING ...] -n PAGES_NUMBER
example: 
python crawler.py -q poly nyu -n 500

No limitation on the parameters user inputs. If the parameter is not correct, the program will tell you.

