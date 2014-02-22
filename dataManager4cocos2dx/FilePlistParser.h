//
//  FilePlistParser.h
//
//  Created by qiong on 14-2-22.
//
//

#ifndef __FilePlistParser__
#define __FilePlistParser__

#include <string>
#include "FileParser.h"

class FilePlistParser : public FileParser
{
public:
    FilePlistParser(const std::string &filename);
    virtual ~FilePlistParser();
    
    virtual TempVar* parse();
    virtual bool persist(TempVar* tv);
};

#endif
