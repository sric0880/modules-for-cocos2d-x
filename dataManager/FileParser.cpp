//
//  FileParser.cpp
//  fruit-slots
//
//  Created by qiong on 14-2-22.
//
//

#include "FileParser.h"
#include "FileJsonParser.h"
#include "FilePlistParser.h"

FileParser::~FileParser()
{
    
}

FileParser* FileParser::create(const std::string &filename)
{
    if(filename.find(".plist")!=std::string::npos
       ||filename.find(".PLIST")!=std::string::npos)
    {
        return new FilePlistParser(filename);
    }else if(filename.find(".json")!=std::string::npos
             ||filename.find(".JSON")!=std::string::npos)
    {
        return new FileJsonParser(filename);
    }else if(filename.find(".sqlite")!=std::string::npos
             ||filename.find(".SQLITE")!=std::string::npos)
    {
//        return new FileSqliteParser(filename);
        return NULL;
    }else{
        return NULL;
    }
}