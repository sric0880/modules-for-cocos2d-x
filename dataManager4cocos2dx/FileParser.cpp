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
    if(filename.find_last_of(".plist")!=std::string::npos
       ||filename.find_last_of(".PLIST")!=std::string::npos)
    {
        return new FilePlistParser(filename);
    }else if(filename.find_last_of(".json")!=std::string::npos
             ||filename.find_last_of(".JSON")!=std::string::npos)
    {
        return new FileJsonParser(filename);
    }else if(filename.find_last_of(".sqlite")!=std::string::npos
             ||filename.find_last_of(".SQLITE")!=std::string::npos)
    {
//        return new FileSqliteParser(filename);
        return NULL;
    }else{
        return NULL;
    }
}