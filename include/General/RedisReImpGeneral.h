#pragma once
#include <string>
#include <functional>
#include <memory>
#include <ini.h>
#include <vector>
#include <unordered_map>
#include <variant>
#include <ctime>
#include <map>
#include <deque>
#include <time.h>
#include <sys/time.h>
#include <chrono>
#include <KVInterface.h>
// using alises
using StrPtr = std::shared_ptr<std::string>;
using StrVecPtr = std::shared_ptr< std::vector< std::shared_ptr<std::string> > >;
using Timestamp = int64_t;
// allocate memory for StrVecPtr
inline StrVecPtr createStrVecPtr() {
	return std::make_shared< std::vector< std::shared_ptr<std::string>> >();
};


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
	namespace KVStorage {
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
	int ioWorkerThreadNum = 0;
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


uint64_t getSysTimestampNS();
inline Timestamp getTimestampNow() {
	auto now = std::chrono::system_clock::now();
    // 将时间点转换为时间戳（自 1970 年 1 月 1 日以来的秒数）
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
	return timestamp;
}

class RuntimeCounter {
	static uint64_t loopStartT;
	static uint64_t loopAllT;
	static uint64_t moduleStartT;
	static uint64_t internalT;
	static uint64_t internalStartT;
	static std::string moduleNow;
	static uint64_t loopCount;
	static uint64_t loopPrintCounts;
	static std::unordered_map<std::string, uint64_t> moduleTimes;
	inline static void addModuleTime(const std::string &, uint64_t);
	inline static void internalHead();
	inline static void internalTail();


public:
	static void reset();
	static void loopStart();
	static void loopEnd();
	static void printStatistics();
	static void moduleStart(const std::string&);
	static void moduleEnd();
	static void setPrintLoopNum(uint64_t);
	
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







