//
//  gzfile.h
//  index
//
//  Created by Hanzhou Li on 3/31/13.
//  Copyright (c) 2013 Hanzhou Li. All rights reserved.
//

#ifndef __index__gzfile__
#define __index__gzfile__

/****************************************************
 Return the uncompress content of the GZipped file.
 @filePath: The path of the compressed file.
 ****************************************************/
char *ReadGZFile(const char *filePath);

#endif /* defined(__index__gzfile__) */
