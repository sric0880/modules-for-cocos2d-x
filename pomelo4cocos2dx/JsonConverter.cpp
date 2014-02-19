#include "JsonConverter.h"

Value convertFrom(json_t* json)
{
    Value v;
    switch (json_typeof(json)) {
        case JSON_OBJECT:
        {
            ValueMap* dict = new ValueMap();
            const char* key;
            json_t* value;
            json_object_foreach(json, key, value)
            {
                (*dict)[key] = convertFrom(value);
            }
            v = dict;
            break;
        }
        case JSON_ARRAY:
        {
            ValueVector* arr = new ValueVector();
            size_t arr_size = json_array_size(json);
            for (int i = 0; i < arr_size; ++i) {
                json_t *value = json_array_get(json, i);
                arr->push_back(convertFrom(value));
            }
            v = arr;
            break;
        }
        case JSON_STRING:
        {
            v = json_string_value(json);
            break;
        }
        case JSON_INTEGER:
        {
            v = (int)json_integer_value(json);
            break;
        }
        case JSON_REAL:
        {
            v = json_real_value(json);
            break;
        }
        case JSON_TRUE:
        {
            v = true;
            break;
        }
        case JSON_FALSE:
        {
            v = false;
            break;
        }
        case JSON_NULL:
        default:
            break;
    }
    return v;
}

json_t* convertFrom(Value& value)
{
    Value::Type type = value.getType();
    CCASSERT(type!=Value::Type::INT_KEY_MAP, "not support Type::INT_KEY_MAP");
    CCASSERT(type!=Value::Type::BYTE, "not support Type::BYTE");
    json_t* json;
    switch (type) {
        case Value::Type::INTEGER:
        {
            json = json_integer(value.asInt());
            break;
        }
        case Value::Type::FLOAT:
        {
            json = json_real(value.asFloat());
            break;
        }
        case Value::Type::DOUBLE:
        {
            json = json_real(value.asDouble());
            break;
        }
        case Value::Type::BOOLEAN:
        {
            json = json_boolean(value.asBool());
            break;
        }
        case Value::Type::STRING:
        {
            json = json_string(value.asString().c_str());
            break;
        }
        case Value::Type::VECTOR:
        {
            json = json_array();
            ValueVector arr = value.asValueVector();
            size_t arr_size = arr.size();
            for (int i = 0; i < arr_size; ++i) {
                json_array_append_new(json, convertFrom(arr[i]));
            }
            break;
        }
        case Value::Type::MAP:
        {
            json = json_object();
            for(auto iter : value.asValueMap()){
                json_object_set_new(json, iter.first.c_str(), convertFrom(iter.second));
            }
            break;
        }
        default:
            break;
    }
    return json;
}