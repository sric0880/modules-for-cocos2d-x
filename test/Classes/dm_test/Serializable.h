//
//  Serializable.h
//  test
//
//  Created by qiong on 14-11-29.
//
//

#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

#include <TempVar.h>
#include <iostream>

class Person {
public:
    Person() {}
    Person(const std::string& name, unsigned age) : name_(name), age_(age) {}
    virtual ~Person();
    
    Person& operator=(const Person& rhs) {
        if (this == &rhs)
            return *this;
        name_ = rhs.name_;
        age_ = rhs.age_;
        return *this;
    }
    
protected:
    
    void serialize(ValueType& value) const;
    
    void deserialize(const ValueType& value);
    
    friend std::ostream& operator << (std::ostream& o, const Person& per);
private:
    std::string name_;
    unsigned age_;
};

class Education {
public:
    ~Education();
    Education(){}
    Education(const std::string& school, double GPA) : school_(school), GPA_(GPA) {}

    void serialize(ValueType& value) const;
    
    void deserialize(const ValueType& value);
    
    friend std::ostream& operator << (std::ostream& o, const Education& edu);
private:
    std::string school_;
    double GPA_;
};

class Dependent : public Person {
public:
    Dependent() : Person(), education_(0){}
    Dependent(const std::string& name, unsigned age, Education* education = 0) : Person(name, age), education_(education) {}
    Dependent(const Dependent& rhs) : Person(rhs), education_(0) { education_ = (rhs.education_ == 0) ? 0 : new Education(*rhs.education_); }
    virtual ~Dependent();
    
    Dependent& operator=(const Dependent& rhs) {
        if (this == &rhs)
            return *this;
        Person::operator=(rhs); //MUST Call this!!!
        delete education_;
        education_ = (rhs.education_ == 0) ? 0 : new Education(*rhs.education_);
        return *this;
    }
    
    void serialize(ValueType& value) const;
    void deserialize(const ValueType& value);
    
    friend std::ostream& operator << (std::ostream& o, const Dependent& dep);
private:
    
    Education *education_;
};

#endif