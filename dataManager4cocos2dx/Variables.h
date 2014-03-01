//
//  Variables.h
//  全局变量管理
//
//  Created by qiong on 14-2-22.
//
//

#ifndef _Variables_h
#define _Variables_h

#include "LocalVar.h"
#include "TempVar.h"
#include <unordered_map>

class Variables{
    typedef std::unordered_map<std::string,LocalVar*> LocalVarMap;
public:
    static Variables& getInstance(){
        static Variables instance;
        return instance;
    };
    
    /**
     *	@brief the varialbes only store in memory
     *
     *	@Modified by qiong at 2014-02-27 15:28:10
     *
     *	@return	TempVar pointer
    **/
    TempVar* getMemStorage();

    /**
     *	@brief	get the local variables
     *
     *	@Modified by qiong at 2014-02-27 15:28:21
     *
     *	@param 	filename if not found, empty tempvar will be retured.
     *
     *	@return	TempVar never be null
    **/
    TempVar* getLocal(const std::string& filename);

    /**
     *	@brief	write the variables back to file system
     *
     *	@Modified by qiong at 2014-02-27 15:28:25
     *
     *	@param 	the mapping variables need to persist
    **/
    void persistLocal(const std::string& filename);
    
private:
    Variables():_memStorage(new TempVar()){
    }
    Variables(Variables const&);
    Variables& operator=(Variables const&);
    virtual ~Variables();
    LocalVarMap _lcMap;
    TempVar* _memStorage;
};

#define VARIABLES Variables::getInstance()
#define USERDEFAULT UserDefault::getInstance()

#endif
