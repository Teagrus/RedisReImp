#pragma once

#include<RedisReImpGeneral.h>
#include<string>
#include<vector>
#include<map>
#include<memory>

namespace RedisReImp::RESPSolver {

class RESPData{
private:
    // '+' '-' ':' '$' '*' - corresponding to the description of RESP protocol
    // '\0' this is not an invalid RESPData
    char dataType;

    // for *   ---- use unique ptr to avoid memory copy for vector or string
    std::shared_ptr< std::vector<RESPData> > commandDatas = nullptr;

    // for '+' '$' '-'
    std::shared_ptr< std::string > strData = nullptr;

    // for :
    int intData;

    

public:

    // if return nullptr -> received string is invalid , resultStatus < 0 the input string is invalid
    static RESPData checkData(std::string & strDataIn, int offset, int * resultStatus = nullptr, int* nextPartOffset = nullptr);
    
    std::shared_ptr<std::string> parseRESPString();
    
    // getters and setters
    std::shared_ptr<std::string> getDataStr();
    const std::shared_ptr<std::vector<RESPData>> getDataVec();
    int getDataInt();
    char getDataType();
    RESPData& getByIndex(int index);
    int getDataVecLen();
    

    void setDataVec(std::shared_ptr< std::vector<RESPData> >);
    void setDataVecEmplace(RESPData &);
    void setDataStr(std::shared_ptr< std::string> );
    void setDataStr(const std::string datain);
    void setDataInt(int);
    void setDataType(char dtype);
    

    bool isValidDataType();

    // constructors
    RESPData(const RESPData&);
    RESPData(char dataType);
    RESPData();
    ~RESPData();

};


}