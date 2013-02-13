import logging

LOGFILE = "crawler.log"
class Logger(object):
	_logger = None
	def __new__(cls, *args, **kwargs):
		if not cls._logger:
			cls._logger = logging.getLogger()
			handler = logging.FileHandler(LOGFILE)
			formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
			handler.setFormatter(formatter)
			cls._logger.addHandler(handler)
			cls._logger.setLevel(logging.NOTSET)
		return cls._logger