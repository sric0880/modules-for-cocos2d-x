//
//  LocalVar.h
//
//  Created by qiong on 14-2-22.
//
//

#ifndef __LocalVar__
#define __LocalVar__

#include <cocos/platform/CCFileUtils.h>

inline std::string getWritableFilename(const std::string& filename)
{
    return cocos2d::FileUtils::getInstance()->getWritablePath().append(filename);
}

class LookUpDict{
public:
    
    constexpr static const char* const LookUpDictFile = "filename_lookup_dic.plist";
    
    static void addFilenameLookupDictionary(const std::string& filename)
    {
        _lookupDict[filename] = cocos2d::Value(getWritableFilename(filename));
        cocos2d::FileUtils::getInstance()->setFilenameLookupDictionary(_lookupDict);
    }
    
    static void loadFilenameLookupDictionary()
    {
        auto fu = cocos2d::FileUtils::getInstance();
        cocos2d::ValueMap vm;
        vm[LookUpDictFile] = cocos2d::Value(getWritableFilename(LookUpDictFile));
        fu->setFilenameLookupDictionary(vm);
        
        _lookupDict = fu->getValueMapFromFile(LookUpDictFile);
        fu->setFilenameLookupDictionary(_lookupDict);
    }
    
    static void saveFilenameLookupDictionary()
    {
        addFilenameLookupDictionary(LookUpDictFile);
        cocos2d::FileUtils::getInstance()->writeToFile(_lookupDict, LookUpDictFile);
    }
    
private:
    static cocos2d::ValueMap _lookupDict;
};


#include <json/document.h>
#include <json/rapidjson.h>
#include <json/stringbuffer.h>
#include <json/writer.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

template <typename DocumentType>
class JsonIO{
public:
    typedef DocumentType DocType;
    std::shared_ptr<DocType> getDocument(const std::string& filename);
    void writeDocument(DocType* ,const std::string& filename);
};

template <
template <typename T> class FileIO = JsonIO, //default file format is json
typename Encoding = rapidjson::UTF8<>,
typename Allocator = rapidjson::MemoryPoolAllocator<>
>
class LocalVar{
public:
    typedef rapidjson::GenericDocument<Encoding, Allocator> DocumentType;
    typedef std::unordered_map<std::string, std::shared_ptr<DocumentType> > DocumentsMap;

    static LocalVar& getInstance(){
        static LocalVar instance;
        return instance;
    };
    DocumentType& getDocument(const std::string& filename);
    bool persistDocument(const std::string& filename);
    
    ~LocalVar();
private:
    LocalVar();
    LocalVar(LocalVar const&);
    LocalVar& operator = (LocalVar const&);
    
    DocumentsMap _docMap;
    FileIO<DocumentType> _fileIO;
};

//! implementation

#include "aes/aes_my.h"


#define __AES__ 1

template <typename DocumentType>
std::shared_ptr<DocumentType> JsonIO<DocumentType>::getDocument(const std::string& filename)
{
    const char* content;
    std::shared_ptr<DocumentType> document(new DocumentType());
#if __AES__
    cocos2d::Data data = cocos2d::FileUtils::getInstance()->getDataFromFile(filename);
    if (data.isNull()) {
        return document;
    }
    unsigned char* bytes = data.getBytes();
    auto len = data.getSize();
    decrypt(len, bytes, bytes);
    std::string contentStr((char*)bytes, len);
    content = contentStr.c_str();
#else
    std::string contentStr = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
    if (contentStr == "") {//if the file not found, also return a document
        return document;
    }
    content = contentStr.c_str();
#endif
    auto& doc = document->Parse(content);
    if (doc.HasParseError()) {
        CCAssert(0, doc.GetParseError());
    }
    return document;
}

template <typename DocumentType>
void JsonIO<DocumentType>::writeDocument(DocumentType* document ,const std::string& filename)
{
    LookUpDict::addFilenameLookupDictionary(filename);
    auto fullPath = getWritableFilename(filename);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document->Accept(writer);
    
    const char* content = buffer.GetString();
#if __AES__
    encrypt_save(strlen(content), (unsigned char *)content, fullPath.c_str());
#else
    std::ofstream fout(fullPath);
    fout.write(content, strlen(content));
    fout.close();
#endif
}

template <
template <typename T > class FileIO, typename Encoding, typename Allocator>
typename LocalVar<FileIO, Encoding, Allocator>::DocumentType&
LocalVar<FileIO, Encoding, Allocator>::getDocument(const std::string& filename)
{
    auto doc_iter = _docMap.find(filename);
    if (doc_iter == _docMap.end()) {
        auto ret = _docMap.insert(std::make_pair(filename, _fileIO.getDocument(filename)));
        return *ret.first->second;
    }
    return *doc_iter->second;
}

template <
template <typename T > class FileIO, typename Encoding, typename Allocator>
bool LocalVar<FileIO, Encoding, Allocator>::persistDocument(const std::string& filename)
{
    auto doc_iter = _docMap.find(filename);
    if (doc_iter != _docMap.end()) {
        _fileIO.writeDocument(doc_iter->second.get(), filename);
        return true;
    }
    return false;
}
#endif
