//
//  FileSqliteParser.h
//
//  Created by qiong on 14-2-22.
//
//

#ifndef __FileSqliteParser__
#define __FileSqliteParser__

#include <string>
#include "FileParser.h"

class FileSqliteParser : public FileParser
{
public:
    FileSqliteParser(const std::string &filename);
    virtual ~FileSqliteParser();
    
    virtual TempVar* parse();
    virtual bool persist(TempVar* tv);
};

#endif
