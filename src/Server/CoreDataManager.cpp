#include<CoreDataManager.h>

namespace RedisReImp::Server {


// class CoreDataManager {
//     private:
//         std::unordered_map<std::string, std::string> stringMap;
//         std::unordered_map<std::string, std::unordered_map<std::string, std::string>> hashmapMap;
//         std::unordered_map<std::string, std::deque<std::string>> listMap;

//     public:
//         inline std::unordered_map<std::string, std::string> getStringMap();
//         inline std::unordered_map<std::string, std::unordered_map<std::string, std::string>> getHashmapMap();
//         inline std::unordered_map<std::string, std::deque<std::string>> getListMap();
// }; 


std::unordered_map<std::string, std::string>& CoreDataManager::getStringMap() {
    return stringMap;
}

std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& CoreDataManager::getHashmapMap() {
    return hashmapMap;
};

std::unordered_map<std::string, std::deque<std::string>> & CoreDataManager::getListMap() {
    return listMap;
}


CoreDataManager CoreDataSingleton::coreData;
void CoreDataSingleton::initCoreData() {
    ;
}

CoreDataManager &  CoreDataSingleton::getInstance() {
    if (!coreData.inited) {
        initCoreData;
    }
    return coreData;
};



}