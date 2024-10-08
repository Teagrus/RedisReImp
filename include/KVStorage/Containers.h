#pragma once
#include<DataObjects.h>
#include<RedisReImpGeneral.h>
namespace RedisReImp::KVStorage {

class STDMapContainer : MapInterface {
private:
    std::map<std::string, DataObject> container;
public:
    int del(StrPtr);
    void set(StrPtr key, DataObject& value);
    void set(StrPtr key, DataObject& value, Timestamp expireT);
    int exist(StrPtr key);
    int exist(StrPtr key);
    StrVecPtr getKeys();
    int getKeyNum();

    // served for innner implementation class
    std::string getImplementationClassName();

    // served for deletion
    ~STDMapContainer();
};



class STDListContainer : ListInterface {
private:
    std::deque<DataObject> container;
public:
    void lpush(DataObject& value);
    DataObject lpop();
    void rpush(DataObject& value); 
    DataObject rpop();
    int len();
    DataObject index(int index);
    DataObjectVecPtr getall();
    DataObjectVecPtr getrange(int range0, int range1);
    int clear();
    ~STDListContainer();
};


}