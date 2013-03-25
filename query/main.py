from pagerank import AlexaTrafficRank, GooglePageRank

if __name__ == "__main__":
    url = "http://www.heastyle.com"
    pagerank = GooglePageRank()
    pr = pagerank.get_rank(url)
    print pr