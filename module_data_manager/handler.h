#ifndef RAPIDJSON_HANDLER_H_
#define RAPIDJSON_HANDLER_H_

#include <iostream>
#include <json/rapidjson.h>

namespace rapidjson {
//! GenericValue writer
//! implements the concept Handler.
template <typename Encoding = UTF8<> >
class Handler {
public:
	typedef typename Encoding::Ch Ch;

	enum State { InObject , InArray, None  };

	Handler(std::ostream& out):_out(out),_state(None){}
	//@name Implementation of Handler
	//@{
	void Null()					{_out << "Null";suffix();}
	void Bool(bool b)			{_out << std::boolalpha << b;suffix();}
	void Int(int i)				{_out << i;suffix();}
	void Uint(unsigned u)		{_out << u;suffix();}
	void Int64(int64_t i64)		{_out << i64;suffix();}
	void Uint64(uint64_t u64)	{_out << u64;suffix();}
	void Double(double d)		{_out << d;suffix();}

	void String(const Ch* str, SizeType length, bool copy = false) {
		int i = 0;
		while(i < length)
		{
			_out << str[i];
			++i;
		}
		suffix();
	}

	void StartObject() {
		_state = InObject;
		_out<<"{";
	}

	void EndObject(SizeType memberCount = 0) {
		_state = None;
		_out<<"}";
	}

	void StartArray() {
		_state = InArray;
		_out<<"[";
	}

	void EndArray(SizeType elementCount = 0) {
		_state = None;
		_out<<"]";
	}
	//}
private:
	std::ostream& _out;
	State _state;
	void suffix()
	{
		if(_state == InObject) _out << ":";
		else if(_state == InArray) _out << ",";
	}
};

}

#endif
