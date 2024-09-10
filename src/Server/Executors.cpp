#include<Executors.h>
#include<fstream>
#include<sstream>
#include<iostream>
#include<algorithm>
#include<string>

// register classes
namespace RedisReImp::Server {


bool _commandConfigMapInited = false;
std::unordered_map<std::string, RESPSolver::RESPData> _commandConfigMap;
std::unordered_map<std::string, RESPSolver::RESPData>& getCommandConfigs() {
    if (!_commandConfigMapInited) {
        std::string commandConfigPath = General::GlobalConfig::getInstance().commandConfigPath;
        // load command config resp data file
        std::ifstream targetFile(commandConfigPath);
        if (!targetFile.is_open()) {
            //deleted std::cout << "Failed to open command config file: " << commandConfigPath << std::endl;
            exit(1);
        }


        std::ostringstream contents;
        contents << targetFile.rdbuf();  
        targetFile.close();  
        std::string configDataStr = contents.str();
        
        // //deleted std::cout << "Loaded Commands Data :\n" << configDataStr << std::endl;

        RESPSolver::RESPData commandsData = RESPSolver::RESPData::checkData(configDataStr, 0);
        if (!commandsData.isValidDataType()) {
            //deleted std::cout << "Command config path err, please check " << commandConfigPath << std::endl;
        }

        // resolve datas
        int commandNum = commandsData.getDataVecLen();
        for (auto command: *commandsData.getDataVec()) {
            auto commandStr = *command.getByIndex(0).getDataStr();
            General::toUpper(commandStr);
            _commandConfigMap[commandStr] = command;
        }
        //deleted std::cout << "Command data loaded!! Data from " << commandConfigPath << std::endl;
    // ------------ finished init data map
    }
    

    return _commandConfigMap;
}



std::vector<std::string> commandList = {
    "COMMAND",
    "DEL",
    "SET",
    "GET",
    // "EXIST",
    // "GETSET",
    // "HGETALL",
    // "HKEYS",
    // "HEXISTS",
    // "HSET",
    // "HGET",
    // "HDEL",
    // "LPOP",
    // "LPUSH",
    // "LREM",
    // "LSET",
    // "RPOP",
    // "RPUSH",
    // "LLEN",
    // "LINDEX",
    // "LLEN"
};

// register clases
REGISTER_CLASS(ExecutorCOMMAND)
REGISTER_CLASS(ExecutorDEL)
REGISTER_CLASS(ExecutorSET)
REGISTER_CLASS(ExecutorGET)
// REGISTER_CLASS(ExecutorEXIST)
// REGISTER_CLASS(ExecutorGETSET)
// REGISTER_CLASS(ExecutorHGETALL)
// REGISTER_CLASS(ExecutorHKEYS)
// REGISTER_CLASS(ExecutorHEXISTS)
// REGISTER_CLASS(ExecutorHSET)
// REGISTER_CLASS(ExecutorHGET)
// REGISTER_CLASS(ExecutorHDEL)
// REGISTER_CLASS(ExecutorLPOP)
// REGISTER_CLASS(ExecutorLPUSH)
// REGISTER_CLASS(ExecutorLREM)
// REGISTER_CLASS(ExecutorLSET)
// REGISTER_CLASS(ExecutorRPOP)
// REGISTER_CLASS(ExecutorRPUSH)
// REGISTER_CLASS(ExecutorLLEN)
// REGISTER_CLASS(ExecutorLINDEX)
// REGISTER_CLASS(ExecutorLLEN)


// 



//----- base executors
BaseExecutor::BaseExecutor() {
    ;
}
std::string BaseExecutor::getCommandName() {
    return commandName;
}
RESPSolver::RESPData BaseExecutor::execute(RESPSolver::RESPData& inputData, CoreDataManager& coreData) {
    success = true;
    return inputData;
}

std::string BaseExecutor::getStatus() {
    return status;
}

bool BaseExecutor::isSuccess() {
    return success;
}

BaseExecutor::~BaseExecutor() {
    ;
}


std::unordered_map<std::string, std::shared_ptr<ExecutorInterface>> ExecutorsCluster::executorMap;
bool ExecutorsCluster::inited = false;
void ExecutorsCluster::_initExecutorCluster() {
    for (auto command:commandList) {
        executorMap.emplace(command, 
            std::dynamic_pointer_cast<ExecutorInterface>( General::ClassRegistry::createInstance("Executor" + command) ));
    }
}

std::shared_ptr<ExecutorInterface> ExecutorsCluster::getExcutor(std::string command) {
    if (!ExecutorsCluster::inited) {
        _initExecutorCluster();
    }
    // print all commands
    // std :: cout << "ALL KEYS:\n";
    // for (auto key : executorMap) {
    //     std :: cout << "|" << key.first << "|" << std::endl;
    // }

   
    
    // return executorMap[command];
    auto it = executorMap.find(command);
    if (it != executorMap.end()) {
        return it->second;
    }
    return nullptr;
}

//COMMAND
RESPSolver::RESPData ExecutorCOMMAND::execute(RESPSolver::RESPData & input, CoreDataManager & coreData) {
    RESPSolver::RESPData result('*');
    auto cmapper = getCommandConfigs();
    
    for (auto p : cmapper ) {
        result.setDataVecEmplace(cmapper[p.first]);
    }

    return result;
}


// command, behavior refered to redis command documentation 
RESPSolver::RESPData ExecutorDEL::execute(RESPSolver::RESPData & input, CoreDataManager & coreData) {
    RESPSolver::RESPData result(':');
    auto dataVec = input.getDataVec();
    int successCount = 0;
    for (size_t i = 1; i < (*dataVec).size(); i++)
    {
        auto it = coreData.getStringMap().find( *(*dataVec).at(i).getDataStr() );
        if (it != coreData.getStringMap().end()){
            coreData.getStringMap().erase(it);
            successCount += 1;
        }
    }

    result.setDataInt(successCount);
    return result;
}

RESPSolver::RESPData ExecutorSET::execute(RESPSolver::RESPData & input, CoreDataManager & coreData) {
    RESPSolver::RESPData result('+');
    result.setDataStr("OK");
    auto dataVec = input.getDataVec();

    // set data
    coreData.getStringMap()[*(dataVec->at(1).getDataStr())] = *(dataVec->at(2).getDataStr());
    return result;
}

RESPSolver::RESPData ExecutorGET::execute(RESPSolver::RESPData & input, CoreDataManager & coreData) {
    RESPSolver::RESPData result('$');
    result.setDataStr(coreData.getStringMap()[*input.getByIndex(1).getDataStr()]);
    return result;
}



}

