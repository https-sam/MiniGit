#ifndef MINIGIT_HPP
#define MINIGIT_HPP

#include <string>

struct singlyNode { 
    std::string fileName;
    std::string fileVersion;
    std::string originalName;
    singlyNode* next;
};


struct doublyNode {
    int commitNUmber;
    singlyNode* head;
    doublyNode* previous;
    doublyNode* next;
};


class Git {
    public:
        Git(); // creates an empty DLL
        ~Git(); // destructor, deletes all nodes in SLLs and DLLs
        void createDirectory(); // creates a hidden directory
        void addFile(std::string fileName); // add files to current commit list
        void removeCommit(std::string fileName); // remove files from current commit list
        void commit(); // commmit, copies files
        void checkOut(int commitNumber); // checks out and copies files
        singlyNode* getCurrCommit(); // returns a pointer to the head of the current commit list
        void copyFile(std::string fileName, std::string dest); // copies file
        int getVersion(doublyNode* DLL); // returns the version of DLL
        bool exist(std::string fileName); // checks if the file exist in current version of repository
        void printCurrCommit(); // prints current commit list
        bool noChanges(std::string filename); // checks if there have been any changes in both files
        void setHeadToTheBeginning(); // helper for destructor, set head to the first DLL
        bool checkCommitList(); //check if commit list is empty
        std::string getCommitName(std::string fileName, std::string commitNum);
        std::string getOriginalFileName(std::string fileName); // returns the file name without its version number
        void setHeadToCurrCommit(); // sets head pointer to the first element on DLL
        void _startServer(); //Obtains ip address and starts server (startServer helper)
        void _copyHTML(std::string fileName); // copies file into /var/www/html folder
        void startServer(); // start server

    private:
        doublyNode* head; // head pointer to DDL
        bool serverON = false; // true if online server is running
};


#endif