import logging
import crconfig

# Define the default log file name
LOGFILE = "crawler.log"
class Logger(object):
	_logger = None
	def __new__(cls, *args, **kwargs):
		if not cls._logger:
			cls._logger = logging.getLogger()
			logFileName = LOGFILE
			config = crconfig.Config()
			tmpFileName = config.getLogFileName()
			if tmpFileName is not None and len(tmpFileName) > 0:
				logFileName = tmpFileName
			handler = logging.FileHandler(logFileName)
			formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
			handler.setFormatter(formatter)
			cls._logger.addHandler(handler)
			cls._logger.setLevel(logging.NOTSET)
		return cls._logger