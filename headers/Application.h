#ifndef DBMS_APPLICATION_H
#define DBMS_APPLICATION_H

#include <string>
#include <fstream>
#include <iostream>
#include "CommandParser.h"
#include "DataBase.h"
#include "CommandExecutor.h"

class Application {
public:
    static Application& instance() {
        static Application instance;
        return instance;
    }
    Application(const Application& other) = delete;
    Application& operator=(const Application& other) = delete;

    void menu();
    void execute();
private:
    Application() {}
};


#endif //DBMS_APPLICATION_H
