//
//  cquery.cpp
//  cquery
//
//  Created by Hanzhou Li on 3/30/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#include "cquery.h"
#include <math.h>

double bm25(int f, int n, int N, int dl, double avgdl)
{
    int score = 0.0;
    int k=1.2, b=0.75;
	int idf = log((N-n)+0.5)/(n+0.5);
	if (idf < 0) {
		idf = 0;
    }
    score = idf * (f*(k+1)/(f+k*(1-b+b*dl/avgdl)));
    return score;
}