//
// Created by rotten on 17.01.23.
//
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <list>
#include "json_formatter/json_formatter.h"
#ifndef DIRECTORYCHANGELOGGER_LOGER_H
#define DIRECTORYCHANGELOGGER_LOGER_H


void WriteToFile(std::string path, std::string format, std::string data, std::time_t time){
    if (format == "JSON"){
        JSONFormatter JSON;
        std::ofstream file(path + "log"+".json", std::ios::app);
        if (file.is_open()){
            file << JSON.Serialize(path, data, time);
        }
        file.close();
    }
    if (format == "CSV"){
        std::ofstream file(path + "log"+".csv", std::ios::app);
        if (file.is_open()){
            file << ctime(&time) << data;
        }
        file.close();
    }
    if (format == "YAML"){
        std::ofstream file;
        file.open(path + "log"+".yaml");
        file << data +"\n";
        file.close();
    }
}

class Loger {
private:
    std::time_t Time;
    std::string Format;
    std::string Path;
    bool UseFile;
public:

    Loger Configurate(std::string format, std::string path, bool useFile){
        Time = time(NULL);
        Format = format;
        Path = path;
        UseFile = useFile;
        return Loger();
    }
    void Logln(std::string data, std::string name){
        if (this->UseFile == true){
            WriteToFile(this->Path, this->Format, data, this->Time);
        }
        std::cout << ctime(&Time) + data;
    }
    void Logln(std::string data){
        if (this->UseFile == true){
            WriteToFile(this->Path, this->Format, data, this->Time);
        }
        std::cout << ctime(&Time) + data;
    }
    //Set log format JSON, CSV, YAML
    void SetFormat(std::string format){
        this->Format = format;
    }
};

#endif //DIRECTORYCHANGELOGGER_LOGER_H
