#include <iostream>
#include <fstream>
#include <stdlib.h> // run commands 
#include <filesystem>
#include <string>
namespace fs = std::filesystem;
using std::filesystem::exists;

using namespace std;


#include "miniGit.hpp"


Git::Git() {  // creats a DLL
    doublyNode* git = new doublyNode;
    git->head = NULL;
    git->next = NULL;
    git->previous = NULL;
    git->commitNUmber = 0;
    head = git; // set head
}

void Git::createDirectory() { // creates a hidden directory
    system("rm -rf .minigit"); // deletes .minigit 
    system("mkdir .minigit");
    // fs::create_directory(".minigit"); // for some reason remove_all did not work well
}

// stops apache2
void closeServer() {
    system("sudo service apache2 stop"); // stop webserver
    cout << "[+] Server terminated" << endl;
}


//sets head to the haed of the first DDL
// used for destructor to delete every DDL
void Git::setHeadToTheBeginning() {
    while(true) {
        if(head->previous == NULL) break;
        head = head->previous;
    }
}

// checks if commit list is empty
// returns false if the first element of current SLL is NULL
bool Git::checkCommitList() {
    if(head->head == NULL) return false;

    return true;
}


// sets head pointer to the head of the current commit
void Git::setHeadToCurrCommit() {    
    while(true) {
        if(head->next == NULL) break;
        head = head->next;
    }
}

// checks if the string is a number
bool isNumber(string str) {
    for (size_t i = 0; i < str.length(); i++) {
        if(!isdigit(str[i])) return false;
    }
    return true;
}

// returns a file name with commit number
// e.g.) fileName = driver.cpp
//       commitNum = 00
//       returns   driver00.cpp
std::string Git::getCommitName(std::string fileName, std::string commitNum) {
    bool valid = false;
    for (int i = 0; i < fileName.length(); i++)
    { // prevent from getting an error. makes sure that filename has a file type
        if(fileName[i] == '.') valid = true;
    }

    if(!valid) {
        return "";
    }

    std::stringstream ss(fileName);
    string name, type, finalName;
    if(stoi(commitNum) < 10) { // convert to 00 format
        commitNum = "0"+commitNum;
    }
    getline(ss, name, '.');
    // cout << "filename is " << endl;
    type = fileName.substr(name.length()+1, fileName.length() - name.length()+1);
    finalName = name + commitNum + "." + type;

    return finalName;

}


Git::~Git() { // deletes linked lists
    setHeadToTheBeginning();
    doublyNode* tempD = head;
    cout << "\n[*] Memory deletion log" << endl;
    while(tempD != 0) { // delete SSL
        singlyNode* tempS = tempD->head;
        while(tempS != NULL) {
            cout << "deleting SLL" << endl;
            singlyNode* prev = tempS;
            cout << prev->fileName << endl;
            tempS = tempS->next;
            delete prev;
            prev = nullptr;
        }
        tempD = tempD->next;
    }

    tempD = head; // get the first head of DLL
    while(tempD != NULL) {
        cout << "Deleting DLL" << endl;
        doublyNode* prevD = tempD;
        tempD = tempD->next;
        delete prevD;
    }

    cout << "Memory freed" << endl;

    if(serverON) closeServer();
}

// returns the version of DLL
int Git::getVersion(doublyNode* DLL) {
    int version = 0;
    doublyNode* temp = head;
    while(temp != NULL) {
        version++;
        temp = temp->next;
    }
    
    return version;
}

// returns the file name without its version number
std::string Git::getOriginalFileName(std::string fileName) {
    stringstream ss(fileName);
    string name, type, finalName;
    int nameI;

    getline(ss, name, '.');

    name = name.substr(0, name.length()-2); // since ther's a two degit number

    nameI = name.length();

    type = fileName.substr(nameI+3,fileName.length()-nameI+1);

    finalName = name+"."+type;

    return finalName;
}


// adds files to the current commit
void Git::addFile(std::string fileName) {
    bool exist = false;

    if(exists(fileName)) {
        
        // singlyNode* temp = head->head;
        singlyNode* temp = getCurrCommit();

        while(temp != NULL) { // checks if the file already exist in the current commit
            if(temp->originalName == fileName) {
                cout << "[-] The file already exists in the current commit\n" << endl;
                exist = true;
                break;
            }
            
            temp = temp->next;
        }
        

        if(!exist) { // if the file does not exist in SSL
            int version = getVersion(head) -1; // get the version of the file
            // cout << "filename is " << fileName << endl;
            // cout << "version is " << version << endl;
            string newFileName = getCommitName(fileName, to_string(version));
            string newVersion;

            if(newFileName == "") {
                cout << "[-] File type missing" << endl;
                return;
            }
    
            if(version < 10) {
                newVersion = "0" + to_string(version-1);
            }

            else {
                newVersion = to_string(version-1);
            }

            cout << "[+] Adding " << fileName << " to the current commit\n" << endl;
            singlyNode* newFile = new singlyNode;
            newFile->fileName = newFileName;
            newFile->fileVersion = newVersion;
            newFile->originalName = fileName;

            setHeadToCurrCommit();
            temp = head->head;// set temp to the head of SSL

            if(head->head == NULL) { // add it at the beginning
                //cout << "beginning" << endl;
                head->head = newFile;
            }

            else { // commits exist in SSL
                while(temp != NULL) { 
                    if(temp->next == NULL) {
                        //cout << "pass" << endl;
                        temp->next = newFile; // add it at the end
                        break;
                    }
                    temp = temp->next;
                }
            }
        }
    }

    else {
        cout << "[-] The file does not exist" << endl;
    }
}       


// checks if the file exist in current version of repository
bool Git::exist(std::string fileName) {
    bool found = false;
 

    doublyNode* tempD = head;
    while(true) { // tempD wil be the last node (get the current version of repository )
        if(tempD->next == NULL) break;
    }

    singlyNode* tempS = tempD->head;

    while(tempS != NULL) {
        if(tempS->originalName == fileName) {
            cout << "[+] File found" << endl;
            found = true;
            break;
        }
        tempS = tempS->next;
    }

     if(!found) {
        return false;
    }

    return true;
}

// removes files from the currrent commmit
void Git::removeCommit(string fileName) {
    
    doublyNode* tempD = head;
    while(true) { // tempD wil be the last node (get the current version of repository )
        if(tempD->next == NULL) break;
    }

    singlyNode* tempS = tempD->head;

    while(tempS != NULL) { // set tempS to the target value
        if(tempS->originalName == fileName) {
            break;
        }
        tempS = tempS->next;
    }



    if(tempD->head->originalName == fileName) { // first node is the target
        tempD->head = tempD->head->next;
        cout << "[+] " << tempS->originalName << " has been successfully deleted from the current commit\n" << endl;
        delete tempS;
    }

    else { // delete tempS
        singlyNode* target = tempD->head;
        singlyNode* prev;
        while(target != NULL) {
            if(fileName == target->originalName) {
                prev->next = target->next;
                cout << "[+] " << target->originalName << " has been successfully removed from the current commit\n" << endl;
                delete target;
                break;
            }
            prev = target;
            target = target->next;
        }   
    }
}


// returns a head pointer to the current commit
singlyNode* Git::getCurrCommit() { // can be removed
    doublyNode* tempD = head;
    while(true) { // tempD wil be the last node (get the current version of repository )
        if(tempD->next == NULL) break;

    tempD = tempD->next;
    }
    // cout << "Commit Number: " << tempD->commitNUmber << endl; 
    return tempD->head;
}

// prints the current commit list
void Git::printCurrCommit() {
    singlyNode* tempS = getCurrCommit();
    int count = 1;
    while(tempS != NULL) {   
        cout << "[" << count << "] " << tempS->originalName << endl;
        if(tempS->next == NULL) return;

        count++;
        tempS = tempS->next;
    }
    cout << "[-] Commit list empty\n" << endl;
}



//copys a file to a specific destination
void Git::copyFile(std::string fileName, std::string dest) {
    ofstream out;
    


    if(dest == ".") { // this means user is checking out
        string originalName = getOriginalFileName(fileName);
        out.open(dest+"/"+originalName); // creats a file in the dest directory
        ifstream in; // open the file that's in the current commit list
        string line;

        in.open(".minigit/"+fileName);

        if(in.is_open()) {  // copy the file
            while(getline(in, line)) {// stores a line in the original file
                out << line << endl;
            }
            in.close();
        }

        else cout << "[-] File name invalid" << endl;

        out.close();

        return;
    }






    else { // copy from currnt directory
        out.open(dest+"/"+fileName); // creats a file in the dest directory
        if(out.is_open()) {
            ifstream in; // open the file that's in the current commit list
            string line;

            string realFileName = getOriginalFileName(fileName);

            in.open(realFileName);

            if(in.is_open()) {  // copy the file
                while(getline(in, line)) {// stores a line in the original file
                    out << line << endl;
                }
                in.close();
            }

            else cout << "[-] File name invalid" << endl;

            out.close();
        }

        else cout << "[-] Destination invalid" << endl;
    }
}

// checks if the file in the current directory has been changed or not
// returns true if there have been no changes and returns false is there are.
bool Git::noChanges(std::string fileName) {
    ifstream currFile;
    int firstFile = 0;
    int secondFile = 0;
    string line1;
    string line2;


    currFile.open(".minigit/"+fileName); // creats a file in the dest directory
    while(getline(currFile, line1)) firstFile++; // counts the number of lines
    currFile.close();

    currFile.open(".minigit/"+fileName);
    

    if(currFile.is_open()) {
        ifstream inComitted; // open the file that's in the current directory
        
        string realFileName = getOriginalFileName(fileName);

        inComitted.open(realFileName);
        while(getline(inComitted, line2)) secondFile++;
        inComitted.close();


        inComitted.open(realFileName);

        if(inComitted.is_open()) {  // copy the file
            while(getline(currFile, line1) && getline(inComitted, line2)) {// stores a line in the original file
                // cout << "comparing  " << line1 << " : " << line2 << endl;
                if(line1 != line2) return false;

            }
            if(getline(inComitted, line2)) {
                inComitted.close();
                currFile.close();
                return false; // when new line is added, also returns false
            }

            else if (firstFile != secondFile) return false;
            inComitted.close();
        }

        else cout << "file name invalid" << endl;

        currFile.close();
    }

    else cout << "Destination invalid" << endl;

    
    return true;
}




//commits changes
void Git::commit() {
    if(!checkCommitList()) {
        cout << "[-] Operation unsuccessfull. Commit list empty. \n" << endl;
        return;
    }

    singlyNode* tempS = getCurrCommit();
    int changeCount = 0;
    bool foundAndChanged = false;
    int newVersion;
    string newVersionString; // set to new DLL
    string fileNameChanged;
    string newFileName;
    
    while(tempS != 0) {
        string fileName = tempS->fileName;
        
        if(!exists(".minigit/"+fileName)) { // file does not exist in .minigit
            cout << "[*] File does not exist in .minigit" << endl;
            cout << "[+] Creating " << fileName << endl;
            cout << "[+] File successfully copied\n" << endl;
            cout << "[+] Commit Number: " << head->commitNUmber << endl;
            copyFile(fileName, ".minigit"); 
        }

        else { // file exists in .minigit (2)
            //cout << "[*] File exists in .minigit" << endl;
            if(noChanges(fileName)) { // do nothing
                // cout << "[*] No changes have been made" << endl;
            }

            else { // 1) copy the new file   2) update the name&version of the file in SLL 
                foundAndChanged = true;
                fileNameChanged = tempS->originalName;
                newVersion = stoi(tempS->fileVersion)+1; // increment the version number
                newVersionString = to_string(newVersion);
                cout << "[*] Some changes have been made in " << tempS->originalName << endl;
                cout << "[+] Changes saved" << endl;
                //cout << "[*] New version number: " << newVersion << endl << endl;
                cout << "[+] Commit Number: " << head->commitNUmber << endl; 
                changeCount++;
               
                if(newVersion < 10) {
                    newVersionString = to_string(newVersion);
                }
               
                newFileName = getCommitName(tempS->originalName, newVersionString);
                copyFile(newFileName, ".minigit");
                
            }
        }
        tempS = tempS->next;
    }

    if(changeCount == 0)  {
        
        cout << "[*] No changes have been made in the file" << endl;
    }

    /////////////adds a new DLL
    cout << "[*] Adding a new DLL" << endl;
    doublyNode* nextNewNode = new doublyNode;
    nextNewNode->head = NULL;
    doublyNode* curr = head;
    while(curr != NULL) {
        if(curr->next == NULL) {
            curr->next = nextNewNode;
            nextNewNode->previous = curr;
            nextNewNode->commitNUmber = curr->commitNUmber+1;
            nextNewNode->next = NULL;
            nextNewNode->head = NULL;
            break;
        }
    curr = curr->next;
    }   


    // [*] curr is pointing to the previous DLL
    // copies the content of previous SLL to the new SLL

    singlyNode* previousSLL = curr->head;
    // set head of the new SLL
    singlyNode* newSLL = new singlyNode;
    singlyNode* newSLLHead = newSLL; // head pointer to newSLL head
    newSLL->fileName = previousSLL->fileName;
    newSLL->fileVersion = previousSLL->fileVersion;
    newSLL->originalName = previousSLL->originalName;
    newSLL->next = NULL;
    if(foundAndChanged && previousSLL->originalName == fileNameChanged) {
            newSLL->fileName = newFileName;
            newSLL->fileVersion = newVersionString;
        }
    previousSLL = previousSLL->next;
    
    while(previousSLL != NULL) {
        cout << "[*] Copying old SLL" << endl;
        newSLL->next = new singlyNode;
        newSLL->next->fileName = previousSLL->fileName;
        newSLL->next->fileVersion = previousSLL->fileVersion;
        newSLL->next->originalName = previousSLL->originalName;
        newSLL->next->next = NULL;
        if(foundAndChanged && previousSLL->originalName == fileNameChanged) {
            newSLL->fileName = newFileName;
            newSLL->fileVersion = newVersionString;
        }
        //cout << "new SLL: " << newSLL->fileName << endl;
        newSLL = newSLL->next;
        previousSLL = previousSLL->next;
    }


    //cout << "copied" << endl;
    nextNewNode->head = newSLLHead;
    //cout << "New first node " << nextNewNode->head->fileName << endl;
    // set head to the new DLL
    head = nextNewNode;
    // add a new DLL
}




//checks out a specific version 
/*
1) check if the version exists 
2)

*/
void Git::checkOut(int commitNumber) {
    setHeadToTheBeginning();
    doublyNode* DLL = head; // set head to the first DLL
    bool found = false;

    if(head->head == NULL) {
        cout << "[-] Nothing has been committed" << endl;
        setHeadToCurrCommit();
        return;
    }

    if(commitNumber < 0) {
        cout << "[-] Enter a valid commit number" << endl;
        setHeadToCurrCommit();
        return;
    }

    while(DLL != NULL) {
        if(DLL->commitNUmber-1 == commitNumber) {
            found = true;
            cout << "[*] Commit num is " << DLL->commitNUmber-1 << endl;
            cout << "[+] Commit number found\n" << endl;
            break;
        }
        DLL = DLL->next;
    }

    if(!found) {
        cout << "[-] Commit number not found\n" << endl;
        setHeadToCurrCommit();
        return;
    }

    if(found) {
        singlyNode* SLL = DLL->head;
        singlyNode* print = SLL;
        string choice;
        cout << "[*] Commit number " << DLL->commitNUmber-1 << " contains:" << endl; 
        while(print != NULL) {
            cout << print->fileName << endl;
            print = print->next;
        }

        cout << "[*] Are you sure you want to checkout? All files are going to be over written (y/n)" << endl;
        cout << ">> ";
        cin >> choice;


        if(choice == "y" || choice == "Y") {
            while(SLL != NULL) {
                cout << "[+] Copying " << SLL->fileName << endl;
                copyFile(SLL->fileName, ".");
                SLL = SLL->next;
            }
        }
    }

    setHeadToCurrCommit();
}


 // copies file into /var/www/html folder
void Git::_copyHTML(std::string fileName) { 
    string copy = "cp .minigit/"+fileName+" /var/www/html/"+fileName;
    system(copy.c_str());
}


// if i = 0, returns local ip. If i = 1, returns puiblic ip
string getIPaddress(int i) {
    string ifconfig;
    if(i == 0) { // get local ip
        ifconfig = "hostname -I | cut -d' ' -f1 > ip.txt"; // outputs ip.txt, which contains local ip address,
    }

    else if(i == 1) { // get public ip
        ifconfig = "wget -qO- http://ipecho.net/plain | xargs echo > ip.txt"; // outputs ip.txt, which contains local ip address,
    }
    
    system(ifconfig.c_str());
    ifstream in;
    string IPAddress;
    in.open("ip.txt");
    getline(in, IPAddress);

    system("rm ip.txt"); // removes ip.txt 


    return IPAddress;
}

void Git::_startServer() {
    string IPAddress = getIPaddress(0); // stores the local ip address of the computer
    cout << "[*] Starting a webserver. (Requires sudo privilege)" << endl;
    system("sudo service apache2 start");
    serverON = true;
    cout << "[+] Server initiated\n" << endl;

    cout << "[+] Do you wish to portforward? (y/n) (This allows you to access your files from anywhere in the world)" << endl;
    cout << ">> ";
    string choice;
    cin >> choice;

    if(choice == "Y" || choice == "y") {
        
        cout << "[*] Do you have miniupnpc installed? (y/n) (If you don't know what it is, choose n)" << endl;
        cout << ">> ";
        cin >> choice;

        if(choice == "N" || choice == "n") { // needs to be installed
            system("sudo apt-get install miniupnpc");
        }
        
        string portforward = "upnpc -a "+ IPAddress +" 80 80 TCP"; // opens port 80 to 80
        // cout << "Ipaddress is " << IPAddress << endl;
        // cout << "command ::::" << portforward << endl;
        system(portforward.c_str());
        IPAddress = getIPaddress(1); // get public ip
        cout << "\n[+] Server successfully initiated" << endl;
        cout << "[*] Use the link below to visit the server\n" << endl;
        cout << "[+] http://" << IPAddress << "/\n" << endl;
    }

    else { // local server // get local ip address and display
        cout << "\n[+] Local server successfully initiated" << endl;
        cout << "[*] Use the link below to visit the local server" << endl;
        cout << "http://" << IPAddress << "/" << endl;
    }
}


void Git::startServer(){
    int commitNumber;

    cout << "[*] Enter the commit number" << endl;
    cin >> commitNumber;


    setHeadToTheBeginning();
    doublyNode* DLL = head; // set head to the first DLL
    bool found = false;

    if(head->head == NULL) {
        cout << "[-] Nothing has been committed" << endl;
        setHeadToCurrCommit();
        return;
    }

    if(commitNumber < 0) {
        cout << "[-] Enter a valid commit number" << endl;
        setHeadToCurrCommit();
        return;
    }

    while(DLL != NULL) {
        if(DLL->commitNUmber-1 == commitNumber) {
            found = true;
            cout << "[*] Commit num is " << DLL->commitNUmber-1 << endl;
            cout << "[+] Commit number found\n" << endl;
            break;
        }
        DLL = DLL->next;
    }

    if(!found) {
        cout << "[-] Commit number not found\n" << endl;
        setHeadToCurrCommit();
        return;
    }

    if(found) {
        singlyNode* SLL = DLL->head;
        singlyNode* print = SLL;
        string choice;
        cout << "[*] Commit number " << DLL->commitNUmber-1 << " contains:" << endl; 
        while(print != NULL) {
            cout << print->fileName << endl;
            print = print->next;
        }

        cout << "[*] Are you sure you want to continue? (y/n)" << endl;
        cout << ">> ";
        cin >> choice;


        if(choice == "y" || choice == "Y") {
            while(SLL != NULL) {
                cout << "[+] Copying " << SLL->fileName << endl;
                _copyHTML(SLL->fileName);
                SLL = SLL->next;
            }
        }

        else if(choice == "N" || choice == "n") return;

        else {
            cout << "[-] Invalid input" << endl;
            return;
        }
    }
    if(!serverON) _startServer();

    else cout << "[+] Success!" << endl;

    setHeadToCurrCommit();
}