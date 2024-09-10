#pragma once
#include<RedisReImpGeneral.h>
#include<unordered_map>
#include<deque>

#include<map>
namespace RedisReImp::Server {

class CoreDataSingleton;

class CoreDataManager {
    private:
        bool inited = false;
        std::map<std::string, std::string> stringMap;
        std::map<std::string, std::map<std::string, std::string>> hashmapMap;
        std::map<std::string, std::deque<std::string>> listMap;

        friend CoreDataSingleton;
    public:
        std::map<std::string, std::string>& getStringMap();
        std::map<std::string, std::map<std::string, std::string>>& getHashmapMap();
        std::map<std::string, std::deque<std::string>>& getListMap();
};


class CoreDataSingleton {
private:
	static CoreDataManager coreData;
	static void initCoreData();

public:
	static CoreDataManager &  getInstance();
};


}