//
// Created by Ana Petrova on 19.07.22.
//

#ifndef DBMS_APPLICATION_H
#define DBMS_APPLICATION_H

#include <string>
#include <fstream>
#include <iostream>

class Application {
public:
    static Application& instance() {
        static Application instance;
        return instance;
    }

    Application(const Application& other) = delete;
    Application& operator=(const Application& other) = delete;

    void execute();
private:
    Application() {}

    //helper functions
    void checkIfDbIsValid(const std::string& pathToDb);

};


#endif //DBMS_APPLICATION_H
