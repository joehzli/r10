import json
import urllib
import urllib2

APIKEY = "AIzaSyCyka6XVV4z9U_zvFqZF6vxj4NOyroPwWE"
APIURL = "https://www.googleapis.com/customsearch/v1?"
CX = "014740285518625860143:-e2xdug0ta0"

def search(theQuery, theLimit):
    if theLimit < 1:
        return []
    param = {"key":APIKEY, "cx":CX, "num":theLimit}
    url = APIURL + urllib.urlencode(param)
    url = url + "&q="+theQuery
    response = urllib2.urlopen(url)
    responseJson = json.loads(response.read())
    items = responseJson["items"]
    result = []
    for item in items:
        result.append(item["link"])
    return result