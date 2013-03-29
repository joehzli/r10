import BeautifulSoup

def parse(doc):
	fed = []
	soup = BeautifulSoup.BeautifulSoup(doc)
	for tag in soup.recursiveChildGenerator():
		if isinstance(tag,BeautifulSoup.Tag):
			if tag.string is not None:
				fed.append(tag.string)
	return ''.join(fed)