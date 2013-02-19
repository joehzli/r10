import urlparse
import robotparser
from singleton import singleton
import threading
import crlogger

@singleton
class CRRobot:
    def __init__(self):
        self.mutex = threading.Lock()
        self.robots = {}
    def _getRobot(self, theUrl):
        parsedUrl = urlparse.urlparse(theUrl)
        domain = parsedUrl.netloc
        result = None
        if domain in self.robots:
            result = self.robots[domain]
        else:
            baseUrl = parsedUrl.scheme+"://"+domain
            robotUrl = urlparse.urljoin(baseUrl, "robots.txt")
            robot = robotparser.RobotFileParser()
            robot.set_url(robotUrl)
            robot.read()
            self.mutex.acquire()
            self.robots[domain] = robot
            result = robot
            self.mutex.release()
        return result
              
    def CheckUrl(self, theUrl):
        try:
            robot = self._getRobot(theUrl)
            return robot.can_fetch("CRRobot", theUrl.encode('utf-8'))
        except Exception, exp:
            logger = crlogger.Logger()
            logger.error("Robot Error. Url:"+theUrl+" "+str(exp))
            return True