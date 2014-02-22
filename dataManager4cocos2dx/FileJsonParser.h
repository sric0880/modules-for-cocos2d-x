//
//  FileJsonParser.h
//
//  Created by qiong on 14-2-22.
//
//

#ifndef __FileJsonParser__
#define __FileJsonParser__

#include <string>
#include "FileParser.h"

class FileJsonParser : public FileParser
{
public:
    FileJsonParser(const std::string &filename);
    virtual ~FileJsonParser();
    
    virtual TempVar* parse();
    virtual bool persist(TempVar* tv);
};

#endif
