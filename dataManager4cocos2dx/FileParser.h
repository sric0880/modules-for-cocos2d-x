//
//  FileParser.h
//
//  Created by qiong on 14-2-22.
//
//

#ifndef __FileParser__
#define __FileParser__

#include <string>
#include "TempVar.h"

class FileParser{
public:
    virtual ~FileParser();
    virtual TempVar* parse() = 0;
    virtual bool persist(TempVar* tv) = 0;
    
    static FileParser* create(const std::string &filename);
protected:
    std::string _filename;
};

#endif
