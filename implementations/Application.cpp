#include "../headers/Application.h"

const size_t WIDTH = 60;

void Application::menu() {
    std::cout << "The following commands are supported:" << std::endl;

    std::cout << std::setw(WIDTH) << std::left
              << "CreateTable <table_name> (<column_name>:<column_type>,... ,...) Index ON <column_name>";
    std::cout << "Creates Database Table with <object_name> and the specified columns." << std::endl;

    std::cout << std::setw(WIDTH) << std::left << "DropTable <table_name>";
    std::cout << "Drops table with <table_name> from the database" << std::endl;

    std::cout << std::setw(WIDTH) << std::left << "ListTables";
    std::cout << "Returns all of the table names in the database." << std::endl;

    std::cout << std::setw(WIDTH) << std::left << "TableInfo <table_name>";
    std::cout << "Returns information about table with <table_name> - size of the table in bytes, scheme, indexed columns and number of records." << std::endl;

    std::cout << std::setw(WIDTH) << std::left
              << "Select <column_names> FROM <table_name> WHERE <condition_1> OR|AND <condition_2> ORDER BY <column_name> DISTINCT";
    std::cout << "Returns the specified <column_names> according to the given conditions." << std::endl;

    std::cout << std::setw(WIDTH) << std::left
              << "Remove FROM <table_name> WHERE <condition_1> OR|AND <condition_2> ..";
    std::cout << "Remove one or more records from table with <table_name>." << std::endl;

    std::cout << std::setw(WIDTH) << std::left
              << "Insert INTO <table_name> {(<value_1>, <value_2>...), (<value_1>, <value_2>...)..}";
    std::cout << "Inserts one or more records in table with <table_name>.\n";

    std::cout << std::setw(WIDTH) << std::left << "Exit";
    std::cout << "Exits program.\n";
}

void Application::execute() {

    menu();

    std::string pathToDB = "/Users/petrovaan/Desktop/db/";
    std::string DBName = "FMISql";

    //check if there is already created DB - if not - create one
    std::ifstream in(pathToDB + DBName + ".bin", std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Couldn't open DB for reading. Creating new file directory for database..." << std::endl;
        DataBase dataBase(pathToDB, DBName);
        in.close();
    }

    std::ifstream newIn(pathToDB + DBName + ".bin", std::ios::binary);
    DataBase db(newIn);

    CommandParser commandParser;
    CommandExecutor commandExecutor;
    try {
        while (true) {
            std::string command;
            std::cout << "FMISql>";

            getline(std::cin, command);

            commandParser.clearCommand();

            try {
                commandParser.loadCommand(command);
            } catch (const std::invalid_argument &e) {
                std::cout << e.what() << std::endl;
                continue;
            }

            switch (commandParser.getCommandType(commandParser.getWordAtIndex(0))) {
                case CommandType::CREATE_TABLE:
                    try {
                        commandExecutor.createTable(commandParser, db);
                    } catch (const std::invalid_argument &e) {
                        std::cout << e.what() << std::endl;
                        break;
                    }
                    catch (const std::out_of_range &e) {
                        std::cout << e.what() << std::endl;
                        break;
                    }

                    std::cout << "Table " << commandParser.getWordAtIndex(1) << " created successfully!" << std::endl;
                    break;

                case CommandType::DROP_TABLE:
                    try {
                        commandExecutor.dropTable(commandParser, db);
                    } catch (const std::invalid_argument &e) {
                        std::cout << e.what() << std::endl;
                        break;
                    } catch (const std::out_of_range &e) {
                        std::cout << e.what() << std::endl;
                        break;
                    } catch (const std::exception &e) {
                        std::cout << e.what() << std::endl;
                        break;
                    }

                    std::cout << "Table " << commandParser.getWordAtIndex(1) << " dropped successfully!" << std::endl;
                    break;
                case CommandType::LIST_TABLES:
                    std::cout << "There " << (db.getTablesCount() == 1 ? "is " : "are ")
                              << db.getTablesCount() << " tables in the database:" << std::endl;
                    commandExecutor.listTables(db);
                    break;
                case CommandType::TABLE_INFO:
                    try {
                        commandExecutor.tableInfo(commandParser, db);
                    } catch (const std::invalid_argument &e) {
                        std::cout << e.what() << std::endl;
                        break;
                    } catch (const std::out_of_range &e) {
                        std::cout << e.what() << std::endl;
                        break;
                    }
                    break;
                case CommandType::INSERT:
                    try {
                        if (commandParser.getSizeOfCommands() > 4) {
                            std::cout << "Too many arguments." << std::endl;
                            return;
                        }
                        commandExecutor.insert(commandParser, db);
                    } catch (const std::invalid_argument &e) {
                        std::cout << e.what() << std::endl;
                        break;
                    } catch (const std::out_of_range &e) {
                        std::cout << e.what() << std::endl;
                        break;
                    }
                    break;
                case CommandType::SELECT:
                    try {
                        commandExecutor.select(commandParser, db);
                    } catch (const std::invalid_argument &e) {
                        std::cout << e.what() << std::endl;
                        break;
                    } catch (const std::out_of_range &e) {
                        std::cout << e.what() << std::endl;
                        break;
                    }
                    break;
                case CommandType::REMOVE:
                    try {
                        commandExecutor.remove(commandParser, db);
                    } catch (const std::invalid_argument &e) {
                        std::cout << e.what() << std::endl;
                        break;
                    } catch (const std::out_of_range &e) {
                        std::cout << e.what() << std::endl;
                        break;
                    }
                    break;
                case CommandType::EXIT:
                    db.saveDBInFile();
                    std::cout << "Goodbye" << std::endl;
                    return;
                case CommandType::NONE:
                    std::cout << "Unrecognized command." << std::endl;
                    break;
            }
        }
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}