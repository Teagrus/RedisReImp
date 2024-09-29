#pragma once
#include<memory>
#include<map>
#include<string>
#include<deque>
#include<cassert>
#include<iostream>
#include<string>
#include<RedisReImpGeneral.h>
#include<KVInterface.h>
#include<variant>

using MapPtr = std::shared_ptr< RedisReImp::KVStorage::MapInterface >;
using ListPtr = std::shared_ptr< RedisReImp::KVStorage::ListInterface >;

namespace RedisReImp::KVStorage {

enum class DataTypes {
    STRING,
    MAP,
    LIST
};


class DataObject {
    std::variant<StrPtr, MapPtr, ListPtr> data;

    // extra info for redis server
    bool expired = false;
    Timestamp timeExpire = 0; // 0 as not expire by time
    
public:
    DataObject(const std::string & str); 
    // {
    //     data = std::make_shared<std::string>(str);
    // }
    
    DataObject(MapPtr & map); 
    // {
    //     data = std::make_shared<std::string>(map);
    // }

    DataObject(ListPtr & list);
    // {
    //     data = std::make_shared<std::string>(list);
    // }

    DataObject(std::variant<StrPtr, MapPtr, ListPtr> & data);

    inline void setTimeExpire(Timestamp ts) {
        timeExpire = ts;
    }

    inline void setExpired(bool ex) {
        expired = ex;
    }

    inline void setData(std::variant<StrPtr, MapPtr, ListPtr> d) {
        data = d;
    }

    inline void setData(std::variant<StrPtr, MapPtr, ListPtr> d, bool ex, Timestamp ts) {
        expired = ex;
        timeExpire = ts;
        data = d;
    }

    inline Timestamp getTimeExpire() {
        return timeExpire;
    }

    inline bool getExpired() {
        return expired;
    }

    inline std::variant<StrPtr, MapPtr, ListPtr> getData() {
        return data;
    }

    inline bool checkValid() {
        if (expired) 
            return false;
        if (General::getTimestampNow() >= timeExpire) {
            expired = true;
            return false;
        }
        return true;
    }


    DataTypes getDataType();
    
};


};