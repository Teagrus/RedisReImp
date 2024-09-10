#pragma once
#include <string>
#include <functional>
#include <memory>
#include <ini.h>
#include <unordered_map>
// namespace
namespace RedisReImp{
	namespace General {
		// Codes of general purpose and project management
	}
	
	namespace NetCore {
		// manage for network connection
	}

	namespace Client {
	}
	
	namespace Server {
	}

	namespace RESPSolver {
	}

}



namespace RedisReImp::General {


class GlobalConfig;

// config instance
class Configs {
private:
	bool inited = false;
	friend GlobalConfig;
public:
	// server settings
	int port = 33400;
	std::string listenAdress = "0.0.0.0";

		//epoll settings
	int epollEventsLen = 10;
	std::string commandConfigPath;

	// client settings
};


class GlobalConfig {
private:
	static Configs configs;
	static void initConfig();

public:
	static const Configs &  getInstance();
};



// for reflection
class Reflectable {
public:
	virtual ~Reflectable();
};

class ClassRegistry {
private:
	static std::unordered_map<std::string, std::function<std::shared_ptr<Reflectable>()>>& getRegistry() {
        static std::unordered_map<std::string, std::function<std::shared_ptr<Reflectable>()>> registry;
        return registry;
    }
public:
	// register a class
	static void registerClass(const std::string& className, std::function<std::shared_ptr<Reflectable>()> constructor);
	// create instance
	static std::shared_ptr<Reflectable> createInstance(const std::string & className);

};


// String functions

void toUpper(std::string& targetStr);
void toLower(std::string& targetStr);


// This to count calculate process time
#include <time.h>
#include <sys/time.h>


uint64_t GetSysTimestamp_ns();

class RuntimeCounter{
	static uint64_t start;
	static uint64_t end;
	static uint64_t all;

public:
	static void reset();
	static void loopStart();
	static void loopEnd();
	static void printStatistics();
	static void printStatistics(const std::string start, const std::string end);

};



}


#define REGISTER_CLASS(className) \
	static struct className##Register { \
        className##Register() { \
            RedisReImp::General::ClassRegistry::registerClass( \
				#className, []() -> std::shared_ptr<RedisReImp::General::Reflectable> \
				{ return std::shared_ptr<RedisReImp::General::Reflectable>(dynamic_cast<RedisReImp::General::Reflectable*>(new className()) ); }); \
        } \
    } className##RegisterInstance; 







