#pragma once
#include<DataObjects.h>

namespace RedisReImp::KVStorage {

class STDMapContainer : MapInterface {
private:
    std::map<std::string, DataObject> container;
public:
    int del(StrPtr);
    void set(StrPtr key, DataObject& value);
    void set(StrPtr key, DataObject& value, Timestamp expireT);
    bool exist(StrPtr key);
    bool exist(StrPtr key);
    StrVecPtr getKeys();
    int getKeyNum();

    // served for innner implementation class
    std::string getImplementationClassName();

    // served for deletion
    ~STDMapContainer();
};
}