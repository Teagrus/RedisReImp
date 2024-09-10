#pragma once

#include<RedisReImpGeneral.h>
#include<RESPData.h>
#include<string>
#include<memory>


namespace RedisReImp::General {
// -------------

// base stages
// 1. Read
// 2. Read data parse
// 3. Data process
// 4. Write data parse
// 5. Write

// base Event: an echo event
class BaseEvent
{
private:
    // whether has a write back stage
    bool hasWriteStage = true;
    // int default Buffer Len
    int bufferLen = 4096;

    int readDataLen = 0;
    char * readDataRaw = nullptr;
    // used to save raw input data
    std::string readDataString;
    //use to save parsed data;
    RedisReImp::RESPSolver::RESPData readData;

    RedisReImp::RESPSolver::RESPData responseData;

    std::string responseDataString;
    // bytes written
    size_t bytesWritten = 0;

    // used to save corresponding file descriptor
    int clientFD = -1;
    // used to mark stages from 1 - 5
    int stageNow = 1;
    // stage statue 0 : processing, 1 : finished
    // int stageStatus = 0;

    std::function< RedisReImp::RESPSolver::RESPData (RedisReImp::RESPSolver::RESPData, std::string)> registeredProcessFunction;
    

public:
    int eRead();
    int eReadDataParse();
    int eDataProcess();
    int eWriteDataParse();
    int eWrite();
    

    void registerProcessFunction(std::function< RedisReImp::RESPSolver::RESPData (RedisReImp::RESPSolver::RESPData, std::string)> & func);

    bool triggerEvent();
    int getStageNow();
    void setToFirstStage();


    void setClientFD(int);
    bool eventFinished();
    

    BaseEvent();
    BaseEvent(int);
    ~BaseEvent();
};

// -------------
}