//
//  LocalVar.h
//
//  Created by qiong on 14-2-22.
//
//

#ifndef __PersisVar__
#define __PersisVar__

#include "TempVar.h"
#include "FileParser.h"
#include <string>

class LocalVar{
public:
    LocalVar();
    virtual ~LocalVar();
    void load(const std::string& filename);
    TempVar* get();
    void persist();
private:
    /**
     *	@brief	只支持ValueMap
     *
     *	@Modified by qiong at 2014-02-27 18:00:40
    **/
    TempVar* _tv;

    FileParser* _fileParser;
};

#endif
