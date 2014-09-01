cdef extern from "math.h":
	float log(float x)

def getBM25(int f, int n, int N, int dl, float avgdl):
	cdef float k=1.2
	cdef float b=0.75
	cdef float idf
	cdef float score
	idf = log(((N-n)+0.5)/(n+0.5))
	if idf < 0:
		idf = 0
	score = idf * (f*(k+1)/(f+k*(1-b+b*dl/avgdl)))
	return score