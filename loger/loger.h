//
// Created by rotten on 17.01.23.
//
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <list>
#include "json_formatter/json_formatter.h"
#include "csv_formatter/csv_formatter.h"

#ifndef DIRECTORYCHANGELOGGER_LOGER_H
#define DIRECTORYCHANGELOGGER_LOGER_H


void WriteToFile(std::string path, std::string format, std::string msg, std::time_t time){
    if (format == "JSON"){
        JSONFormatter JSON;
        std::ofstream file(path + "log"+".json", std::ios::app);
        if (file.is_open()){
            file << JSON.Serialize(path, msg, time) << "\n";
        }
        file.close();
    }
    if (format == "CSV"){
        CSVFormatter CSV;
        std::ofstream file(path + "log"+".csv", std::ios::app);
        if (file.is_open()){
            file << CSV.Serialize(path, msg, time) << "\n";
        }
        file.close();
    }
}

class Loger {
private:
    std::time_t Time;
    std::string Format;
    std::string Path;
    bool UseFile;
    JSONFormatter JSON;
    CSVFormatter CSV;
public:
    Loger Configurate(std::string format, std::string path, bool useFile){
        Time = time(NULL);
        Format = format;
        Path = path;
        UseFile = useFile;
        JSON = JSONFormatter();
        CSV = CSVFormatter();
        return Loger();
    }
    void Logln(std::string msg){
        if (this->UseFile == true){
            WriteToFile(this->Path, this->Format, msg, this->Time);
        }
        if (this->Format == "JSON")
            printf("%s\n",this->JSON.Serialize(this->Path, msg, this->Time).c_str());
        if (this->Format == "CSV")
            printf("%s\n",this->CSV.Serialize(this->Path, msg, this->Time).c_str());
    }
    //Set log format JSON, CSV, YAML
    void SetFormat(std::string format){
        this->Format = format;
    }
};

#endif //DIRECTORYCHANGELOGGER_LOGER_H
