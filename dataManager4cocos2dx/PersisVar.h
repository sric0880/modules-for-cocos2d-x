//
//  PersisVar.h
//
//  Created by qiong on 14-2-22.
//
//

#ifndef __PersisVar__
#define __PersisVar__

#include "TempVar.h"
#include "FileParser.h"
#include <string>

class PersisVar{
protected:
    PersisVar(bool);
    virtual ~PersisVar();
public:
    static PersisVar psConst;
    static PersisVar psMutable;
    void loadFile(const std::string& filename);
    TempVar& getTempVar();
private:
    TempVar* _tv;
    FileParser* _fileParser;
    bool _isMutable;
};

#endif
