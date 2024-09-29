#pragma once
#include<ExecutorInterface.h>
#include<RedisReImpGeneral.h>
#include<CoreDataManager.h>
#include<unordered_map>

namespace RedisReImp::Server { 

std::unordered_map<std::string, RESPSolver::RESPData>& getCommandConfigs();

// base executor that has an echo behavior
class BaseExecutor: public ExecutorInterface, public General::Reflectable {
protected:
// status datas
    std::string status;
    std::string commandName="Unknown";
    bool success = false;

public:
    // interface functions
    virtual std::string getCommandName();
    virtual RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
    virtual std::string getStatus(); 
    virtual bool isSuccess();

    BaseExecutor();
    virtual ~BaseExecutor();
};

// Executor mapper
class ExecutorsCluster {
private:
    static std::unordered_map<std::string, std::shared_ptr<ExecutorInterface>> executorMap;
    static bool inited;
    static void _initExecutorCluster();
public:
    static std::shared_ptr<ExecutorInterface> getExcutor(std::string commandName);
};


//Command
class ExecutorCOMMAND: public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};


// Command Executors
class ExecutorDEL: public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};


class ExecutorSET:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};


class ExecutorGET:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};

class ExecutorEXISTS:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};


class ExecutorGETSET:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};


// Hash map executors
class ExecutorHGETALL:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};

class ExecutorHKEYS:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};

class ExecutorHEXISTS:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};

class ExecutorHSET:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};

class ExecutorHGET:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};

class ExecutorHDEL:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};


// for list
class ExecutorLPOP:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};

class ExecutorLPUSH:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};

class ExecutorLREM:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};

class ExecutorLSET:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};

class ExecutorRPOP:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};

class ExecutorRPUSH:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};

class ExecutorLLEN:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};

class ExecutorLINDEX:public BaseExecutor{
public:
    RESPSolver::RESPData execute(RESPSolver::RESPData &, CoreDataManager&);
};







}
