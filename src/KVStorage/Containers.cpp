#include<Containers.h>

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




