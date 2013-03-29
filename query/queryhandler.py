# -*- coding: utf-8 -*-  

import tornado.web
import query

inputTemplate = open("template.html").read()
resultTemplate = open("resulttemplate.html").read()

class QueryHandler(tornado.web.RequestHandler):
    # get method: Weixin server requests to verify the url
    def get(self):
        queryWords = self.get_argument("query",default=None, strip=False)
        if queryWords is None:
            self.write(inputTemplate)
            return
        else:
            queryWords = queryWords.encode('utf-8')
            page = self.get_argument("page",default=None, strip=False)
            page = 0 if page is None else page
            q = query.Query()
            result = q.queryWords(queryWords, page*10, 10)
            output = ""
            for item in result:
                output += item.url+"<br>"+str(item.score)+"<br>"+item.snippet+"<br><br>"
            self.write(output)
            return