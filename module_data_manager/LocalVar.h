//
//  LocalVar.h
//
//  Created by qiong on 14-11-28.
//
//

#ifndef __LocalVar__
#define __LocalVar__

#include <json/document.h>
#include <json/rapidjson.h>
#include <json/stringbuffer.h>
#include <json/writer.h>
#include <json/prettywriter.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include "allocator.h"
#include "lookup_dict.h"

template <typename DocumentType>
class JsonIO{
public:
    typedef DocumentType DocType;
    std::shared_ptr<DocumentType> getDocument(const char* filename);
    void writeDocument(DocumentType* ,const char* filename);
};

template <
template <typename T> class FileIO = JsonIO, //default file format is json, change when need support other files
typename Encoding = rapidjson::UTF8<>,
typename Allocator = rapidjson::MemoryPoolAllocator<>
>
class LocalVar{
public:
    typedef rapidjson::GenericDocument<Encoding, Allocator> DocumentType;
    typedef std::unordered_map<std::string, std::shared_ptr<DocumentType> > DocumentsMap;
    typedef Allocator AllocatorType;
    
    //Single Instance
    static LocalVar* getInstance(){
        static LocalVar<FileIO, Encoding, Allocator> localvar;
        return &localvar;
    };
    
    //return empty document if not found
    DocumentType& getDocument(const char* filename);
    //return false if not found the document, else return true
    bool persistDocument(const char* filename);
    //print the document to the output stream
    void printDocument(const char* filename, std::ostream& o);
    //if not need any more, release the document to save memory
    void releaseDocument(const char* filename);
    
    ~LocalVar(){};
private:
    LocalVar(){};
    LocalVar(LocalVar const&);
    LocalVar& operator = (LocalVar const&);
    
    DocumentsMap _docMap;
    FileIO<DocumentType> _fileIO;
};

#define LOCAL_VAR LocalVar<>::getInstance()
typedef typename LocalVar<>::DocumentType DocumentType;

//! implementation

#include "aes/aes_my.h"


#define __AES__ 1//change to 0 if dont need encrypt

template <typename DocumentType>
std::shared_ptr<DocumentType> JsonIO<DocumentType>::getDocument(const char* filename)
{
    const typename DocumentType::Ch* content;
    std::shared_ptr<DocumentType> document(new DocumentType());
#if __AES__
    cocos2d::Data data = cocos2d::FileUtils::getInstance()->getDataFromFile(filename);
    if (data.isNull()) {
        document->SetObject();
        return document;
    }
    unsigned char* bytes = data.getBytes();
    auto len = data.getSize();
    decrypt(len, bytes, bytes);
    std::string contentStr((char*)bytes, len);
    content = contentStr.c_str(); // FIXME: only support utf8 char*
#else
    std::string contentStr = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
    if (contentStr == "") {//if the file not found, also return a empty document
        document->SetObject();
        return document;
    }
    content = contentStr.c_str();
#endif
    auto& doc = document->template Parse<rapidjson::kParseDefaultFlags>(content);
    if (doc.HasParseError()) {
        CCAssert(0, doc.GetParseError());
    }
    return document;
}

template <typename DocumentType>
void JsonIO<DocumentType>::writeDocument(DocumentType* document ,const char* filename)
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
auto LocalVar<FileIO, Encoding, Allocator>::getDocument(const char* filename)->DocumentType&
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
bool LocalVar<FileIO, Encoding, Allocator>::persistDocument(const char* filename)
{
    auto doc_iter = _docMap.find(filename);
    if (doc_iter != _docMap.end()) {
        _fileIO.writeDocument(doc_iter->second.get(), filename);
        return true;
    }
    return false;
}

template <
template <typename T > class FileIO, typename Encoding, typename Allocator>
void LocalVar<FileIO, Encoding, Allocator>::releaseDocument(const char* filename)
{
    _docMap.erase(filename);
}

template <
template <typename T > class FileIO, typename Encoding, typename Allocator>
void LocalVar<FileIO, Encoding, Allocator>::printDocument(const char* filename, std::ostream& o)
{
    auto& doc = getDocument(filename);
    assert(doc.IsObject()||doc.IsArray()); // connot print empyt document
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> pw(buffer);
    doc.Accept(pw);
    o << buffer.GetString() << std::endl;
}
#endif
