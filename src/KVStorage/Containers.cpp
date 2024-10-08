#include<Containers.h>
#include<RedisReImpGeneral.h>
using namespace RedisReImp::KVStorage;

int STDMapContainer::del(StrPtr key) {
    auto it = container.find(*key);
    if (it == container.end()) {
        return 0;
    }

    // lazy deletion
    it->second.setExpired(true);
    return 1;
}

void STDMapContainer::set(StrPtr key, DataObject& value) {
    auto it = container.find(*key);
    if (it == container.end()) {
        container.emplace(key, value.getData());
    } else{
        it->second.setData(value.getData());
        it->second.setExpired(false);
        it->second.setTimeExpire(0);
    }
    return;
}

void STDMapContainer::set(StrPtr key, DataObject& value, Timestamp exptireT) {
    auto it = container.find(*key);
    if (it == container.end()) {
        container.emplace(key, value.getData(), false, exptireT);
    } else{
        it->second.setData(value.getData());
        it->second.setExpired(false);
        it->second.setTimeExpire(exptireT);
    }
    return;
}

int STDMapContainer::exist(StrPtr key) {
    auto it = container.find(*key);
    if (it == container.end()) {
        return 0;
    } else {
        if (it->second.checkValid()) {
            return 1;
        } else {
            return 0;
        }
    }
};

StrVecPtr STDMapContainer::getKeys() {
    StrVecPtr results = createStrVecPtr();
    auto it = container.begin();
    while (it != container.end())
    {
        results->push_back(std::make_shared<std::string>(it->first));
    }
    return results;
};

int STDMapContainer::getKeyNum() {
    int result = 0;
    auto it = container.begin();
    while (it != container.end())
    {
        result++;
    }
    return result;
};

std::string STDMapContainer::getImplementationClassName() {
    return "STDMap";
}


STDMapContainer::~STDMapContainer() {
    // do nothing now
}

STDListContainer::STDListContainer() {
    ;
}

STDListContainer::~STDListContainer() {
    // do nothing now
    ;
}

void STDListContainer::lpush(DataObject & data) {
    //  container.emplace(key, value.getData(), false, exptireT);
    container.emplace_back(data.getData());
}

void STDListContainer::rpush(DataObject & data) {
    //  container.emplace(key, value.getData(), false, exptireT);
    container.emplace_front(data.getData());
}

DataObject STDListContainer::lpop() {
    auto && data = container.front();
    container.pop_front();
    return data;
}

DataObject STDListContainer::rpop() {
    auto && data = container.back();
    container.pop_back();
    return data;
}

int STDListContainer::len() {
    return container.size();
}

DataObject STDListContainer::index(int index) {
    return container[index];
}

DataObjectVecPtr STDListContainer::getall() {
    DataObjectVecPtr result = createDataObjectVecPtr();
    auto it = container.begin();
    while (it != container.end())
    {
        result->emplace_back(*it);
        ++it;
    }
    
    return result;
}

DataObjectVecPtr STDListContainer::getrange(int range0, int range1) {
    DataObjectVecPtr result = createDataObjectVecPtr();
    int csize = container.size();
    for (size_t i = range0; i < range1; i++)
    {
        if (i >= csize) {
            break;
        }
        result->emplace_back(container[i]);
    }

    return result;
}

int STDListContainer::clear() {
    int csize = container.size();
    container.clear();
    return csize;
}

