#include<RedisReImpGeneral.h>
#include<iostream>
#include <INIReader.h>

namespace RedisReImp::General  {


Configs GlobalConfig::configs;

void GlobalConfig::initConfig() {
    static const std::string CONFIG_PATH = "../config.ini";
    INIReader reader(CONFIG_PATH);

    if (reader.ParseError() < 0) {
        //deleted std::cout << "Can't load 'Config.ini'\nUSE DEFAULT SETTINGS!!\n";
        configs.inited = true;
    } else{
        //deleted std::cout << "Use settings from 'Config.ini'\n";
        configs.inited = true;
        // Network
        configs.port = reader.GetInteger("Network", "port", configs.port);
        configs.listenAdress = reader.GetString("Network", "listenAdress", configs.listenAdress);
        configs.epollEventsLen = reader.GetInteger("Network", "epollEventsLength", configs.epollEventsLen);
        configs.ioWorkerThreadNum = reader.GetInteger("Network", "ioWorkerThreadNum", configs.epollEventsLen);
        // Redis
        configs.commandConfigPath = reader.GetString("Redis", "commandsConfigPath", "./commands.resp");

    }

}


const Configs & GlobalConfig::getInstance() {
    if (!configs.inited) {
        initConfig();
    }
    return configs;
}


void ClassRegistry::registerClass(const std::string& className, std::function<std::shared_ptr<Reflectable>()> constructor) {
    //deleted std::cout << "RegisterClass - " << className << std::endl;
    getRegistry()[className] = constructor;
};

Reflectable::~Reflectable() {

};

std::shared_ptr<Reflectable> ClassRegistry::createInstance(const std::string& className) {
    auto it = getRegistry().find(className);
    if (it != getRegistry().end()) {
        return it->second();
    }
    
    return nullptr;
}


void toUpper(std::string& targetStr) {
    std::transform(targetStr.begin(), targetStr.end(), targetStr.begin(), [](unsigned char c) { return std::toupper(c); });
}

void toLower(std::string& targetStr) {
    std::transform(targetStr.begin(), targetStr.end(), targetStr.begin(), [](unsigned char c) { return std::tolower(c); });
}

// time count
uint64_t getSysTimestampNS() {
    struct timespec timer_nsec;
    uint64_t timestamp_ns = 0;
    if (!clock_gettime(CLOCK_REALTIME, &timer_nsec)) {
        timestamp_ns =
            (uint64_t)timer_nsec.tv_sec * 1e9L + (uint64_t)timer_nsec.tv_nsec;
    }
    return timestamp_ns;
}



// Runtime counter
uint64_t RuntimeCounter::loopStartT = 0;
uint64_t RuntimeCounter::loopAllT = 0;
uint64_t RuntimeCounter::moduleStartT = 0;
uint64_t RuntimeCounter::internalStartT = 0;
uint64_t RuntimeCounter::internalT = 0;

std::string RuntimeCounter::moduleNow = "";
uint64_t RuntimeCounter::loopCount = 0;
uint64_t RuntimeCounter::loopPrintCounts = 0;
std::unordered_map<std::string, uint64_t> RuntimeCounter::moduleTimes;

void RuntimeCounter::internalHead() {
    internalStartT = getSysTimestampNS();
}

void RuntimeCounter::internalTail() {
    internalT +=  getSysTimestampNS() - internalStartT;
}


void RuntimeCounter::addModuleTime(const std::string & moduleName, uint64_t t) {
    auto it = moduleTimes.find(moduleName);
    if (it == moduleTimes.end()) {
        moduleTimes[moduleName] = t;
    } else {
        moduleTimes[moduleName] += t;
    }
}

void RuntimeCounter::reset() {
    loopStartT = 0;
    loopAllT = 0;
    moduleStartT = 0;
    internalT = 0;
    internalStartT = 0;
    moduleNow = "";
    loopCount = 0;
    loopPrintCounts = 0;
    moduleTimes.clear();
}

void RuntimeCounter::loopStart() {
    // stop last loop time count
    loopEnd();
    loopStartT = getSysTimestampNS();
}

void RuntimeCounter::loopEnd() {
    internalHead();
    if (loopStartT != 0) {
        // add loop time to all times
        auto interval = getSysTimestampNS() - loopStartT;
        loopAllT += interval;
        loopCount += 1;
        // reset start
        loopStartT = 0;
        // whether to print log
        if (loopPrintCounts != 0 && loopCount % loopPrintCounts == 0) {
            printStatistics();
        }
    }
    internalTail();
}

void RuntimeCounter::setPrintLoopNum(uint64_t printLoopLength){
    loopPrintCounts = printLoopLength;
}

void RuntimeCounter::moduleStart(const std::string & moduleName) {
    // save last module tcount
    moduleEnd();
    // reset moduleNow
    moduleNow = moduleName;
    moduleStartT = getSysTimestampNS();
}


void RuntimeCounter::moduleEnd() {
    internalHead();
    if (moduleStartT != 0) {
        auto interval = getSysTimestampNS() - moduleStartT;
        addModuleTime(moduleNow, interval);
        moduleStartT = 0;
    }
    internalTail();
}

void RuntimeCounter::printStatistics() {
    std::cout << "-------------Time occupation--------------\n";
    std::cout << "loopAllT : " << loopAllT << "\n";
    double tall = 0;
    for (auto pa:moduleTimes) {
        tall += (moduleTimes[pa.first] / (double)loopAllT) * 100;
        std::cout << pa.first << ": " << (moduleTimes[pa.first] / (double)loopAllT) * 100 << "% \n";
    }

    
    std::cout << "Internal Time: " << (internalT / (double)loopAllT) * 100 << "% \n";
    tall += (internalT / (double)loopAllT) * 100;
    std::cout << "Above all: " << tall  << "% \n";
    std::cout << "==========================================\n";
}


}




