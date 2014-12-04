//
//  Serializable.cpp
//  test
//
//  Created by qiong on 14-11-29.
//
//

#include "Serializable.h"



Person::~Person() {
//    std::cout << "Person destructor"<< std::endl;
}

void Person::serialize(ValueType& value) const {
    ValueType name(name_.c_str(), (rapidjson::SizeType)name_.length(), getAllocator<TempVar<> >());
    value.AddMember("name", getAllocator<TempVar<> >(), name, getAllocator<TempVar<> >());
    value.AddMember("age", age_, getAllocator<TempVar<> >());
}

void Person::deserialize(const ValueType& value)
{
    name_ = std::string( value["name"].GetString() );
    age_ = value["age"].GetUint();
}

Education::~Education()
{
//    std::cout << "Education destructor"<< std::endl;
}
void Education::serialize(ValueType& value) const {
    ValueType school(school_.c_str(), (rapidjson::SizeType)school_.length(), getAllocator<TempVar<> >());
    value.AddMember("school", getAllocator<TempVar<> >(), school, getAllocator<TempVar<> >());
    value.AddMember("GPA", GPA_, getAllocator<TempVar<> >());
}

void Education::deserialize(const ValueType& value)
{
    school_ = std::string( value["school"].GetString() );
    GPA_ = value["GPA"].GetDouble();
}

Dependent::~Dependent()
{
//   std::cout << "Dependent destructor"<< std::endl;
    delete education_;
}

void Dependent::serialize(ValueType& value) const {
    if (education_) {
        ValueType obj;
        obj.SetObject();
        education_->serialize(obj);
        value.AddMember("education", obj, getAllocator<TempVar<> >());
    }
    Person::serialize(value);
}

void Dependent::deserialize(const ValueType& value)
{
    if(value.HasMember("education")){
        if (!education_) {
            education_ = new Education();
        }
        education_->deserialize( value["education"] );
    }
    Person::deserialize(value);
}

std::ostream& operator << (std::ostream& o, const Person& per )
{
    o << "Age: " << per.age_<<", Name: "<<per.name_<<std::endl;
    return o;
}

std::ostream& operator << (std::ostream& o, const Education& edu)
{
    o << "School: "<< edu.school_ << ", GPA: "<< edu.GPA_ << std::endl;
    return o;
}

std::ostream& operator << (std::ostream& o, const Dependent& dep)
{
    if (dep.education_) {
        o << *dep.education_;
    }
    o << (Person)dep;
    return o;
}