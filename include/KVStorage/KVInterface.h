#include<RedisReImpGeneral.h>
#include<string>
#include<memory>
#include<ctime>
// #define StrPtr std::shared_ptr<std::string> 


namespace RedisReImp::KVStorage {

class KVInterface {
public:
    // key related

    // del target key
    virtual int delKey(StrPtr) = 0;
    // unlink target key for soft deletion
    virtual int unlinkKey(StrPtr) = 0;
    virtual int existKey(StrPtr) = 0;


    virtual int expireKey(StrPtr key, std::time_t expireTime) = 0;
    

    // String interfaces
    virtual StrPtr getString(StrPtr key) = 0;
    virtual void setString(StrPtr key, StrPtr content) = 0;
    virtual void setString(StrPtr key, StrPtr content, std::time_t expireTime) = 0;

    // hash interfaces
    virtual StrPtr getHash(StrPtr hashKey, StrPtr key) = 0;
    virtual StrPtr setHash(StrPtr hashKey, StrPtr key, StrPtr value) = 0;
    virtual int existHash(StrPtr hashKey, StrPtr key);
    virtual int delHashKey(StrPtr hashKey, StrPtr key) = 0;
    virtual StrVecPtr getHashInnerKeys(StrPtr hashKey) = 0;

    // virtual int delHashKey(StrPtr hashKey, StrPtr key) = 0;

    // list interfaces
    virtual void lpushList(StrPtr listKey, StrPtr value) = 0; // push left
    virtual StrPtr lpopList(StrPtr listKey) = 0; // pop left
    virtual void rpushList(StrPtr listKey, StrPtr value) = 0; // push right
    virtual StrPtr rpopList(StrPtr listKey) = 0; // pop right
    virtual int lenList(StrPtr listKey) = 0; // get list len
    virtual StrPtr indexList(StrPtr listKey, int index) = 0; // get index str in target list
    virtual StrVecPtr getallList(StrPtr listKey) = 0;
    virtual StrVecPtr getrangeList(StrPtr listKey, int range0, int range1) = 0;


    // These functions to get excution infomation
};

}