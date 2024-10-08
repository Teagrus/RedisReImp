#include<RedisReImpGeneral.h>
#include<string>
#include<memory>
#include<ctime>
#include<DataObjects.h>
// #define StrPtr std::shared_ptr<std::string> 


namespace RedisReImp::KVStorage {
class MapInterface {
public:
    DataObject get(StrPtr);
    virtual int del(StrPtr);
    virtual void set(StrPtr key, DataObject& value);
    virtual void set(StrPtr key, DataObject& value, Timestamp expireT);
    virtual int exist(StrPtr key);
    virtual StrVecPtr getKeys();
    virtual int getKeyNum();

    // served for innner implementation class
    virtual std::string getImplementationClassName();


    // served for deletion
    virtual ~MapInterface() = 0;
};

class ListInterface {
public:
    virtual void lpush(DataObject& value) = 0; // push left
    virtual DataObject lpop() = 0; // pop left
    virtual void rpush(DataObject& value) = 0; // push right
    virtual DataObject rpop() = 0; // pop right
    virtual int len() = 0; // get list len
    virtual DataObject index(int index) = 0; // get index str in target list
    virtual DataObjectVecPtr getall() = 0;
    virtual DataObjectVecPtr getrange(int range0, int range1) = 0;
    virtual int clear() = 0;
    virtual ~ListInterface() = 0;
};

// class KVInterface {
// public:
//     // key related
//     // del target key
//     virtual int delKey(StrPtr) = 0;
//     // unlink target key for soft deletion
//     virtual int unlinkKey(StrPtr) = 0;
//     virtual int existKey(StrPtr) = 0;


//     virtual int expireKey(StrPtr key, std::time_t expireTime) = 0;
    

//     // String interfaces
//     virtual StrPtr getString(StrPtr key) = 0;
//     virtual void setString(StrPtr key, StrPtr content) = 0;
//     virtual void setString(StrPtr key, StrPtr content, std::time_t expireTime) = 0;

//     // hash interfaces
//     virtual StrPtr getHash(StrPtr hashKey, StrPtr key) = 0;
//     virtual StrPtr setHash(StrPtr hashKey, StrPtr key, StrPtr value) = 0;
//     virtual int existHash(StrPtr hashKey, StrPtr key);
//     virtual int delHashKey(StrPtr hashKey, StrPtr key) = 0;
//     virtual StrVecPtr getHashInnerKeys(StrPtr hashKey) = 0;

//     // virtual int delHashKey(StrPtr hashKey, StrPtr key) = 0;

//     // list interfaces
//     virtual void lpushList(StrPtr listKey, StrPtr value) = 0; // push left
//     virtual StrPtr lpopList(StrPtr listKey) = 0; // pop left
//     virtual void rpushList(StrPtr listKey, StrPtr value) = 0; // push right
//     virtual StrPtr rpopList(StrPtr listKey) = 0; // pop right
//     virtual int lenList(StrPtr listKey) = 0; // get list len
//     virtual StrPtr indexList(StrPtr listKey, int index) = 0; // get index str in target list
//     virtual StrVecPtr getallList(StrPtr listKey) = 0;
//     virtual StrVecPtr getrangeList(StrPtr listKey, int range0, int range1) = 0;


//     // These functions to get excution infomation
// };

}