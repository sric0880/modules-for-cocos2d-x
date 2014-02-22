//
//  Variables.h
//
//  Created by qiong on 14-2-22.
//
//

#ifndef _Variables_h
#define _Variables_h

#include "PersisVar.h"
#include "TempVar.h"

class VARIABLES{
public:
    static TempVar& TEMP(){
        static TempVar tv;
        return tv;
    }
    
    static void MUTABLE_LOAD(const char* filename){
        PersisVar::psMutable.loadFile(filename);
    }
    
    static TempVar& MUTABLE(){
        return PersisVar::psMutable.getTempVar();
    }
    
    static void CONST_LOAD(const char* filename){
        PersisVar::psConst.loadFile(filename);
    }
    
    static const TempVar& CONST(){
        return PersisVar::psConst.getTempVar();
    }
    
};

#endif
