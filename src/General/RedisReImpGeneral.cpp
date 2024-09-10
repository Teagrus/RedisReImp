#include<RedisReImpGeneral.h>
#include<iostream>
#include <INIReader.h>

namespace RedisReImp::General  {


Configs GlobalConfig::configs;

void GlobalConfig::initConfig() {
    static const std::string CONFIG_PATH = "./Config.ini";
    INIReader reader(CONFIG_PATH);

    if (reader.ParseError() < 0) {
        std::cout << "Can't load 'Config.ini'\nUSE DEFAULT SETTINGS!!\n";
        configs.inited = true;
    } else{
        std::cout << "Use settings from 'Config.ini'\n";
        configs.inited = true;
        // Network
        configs.port = reader.GetInteger("Network", "port", configs.port);
        configs.listenAdress = reader.GetString("Network", "listenAdress", configs.listenAdress);
        configs.epollEventsLen = reader.GetInteger("Network", "epollEventsLength", configs.epollEventsLen);

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
    std::cout << "RegisterClass - " << className << std::endl;
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


}




