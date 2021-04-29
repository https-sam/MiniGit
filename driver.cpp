#include "miniGit.hpp"

using namespace std;

#include <iostream>
#include <filesystem>
using std::filesystem::exists;

void printMINIGIT() {
    cout << "___  ____       _ _____ _ _   " << endl;
    cout << "|  \\/  (_)     (_)  __ (_) |  " << endl;
    cout << "| .  . |_ _ __  _| |  \\/_| |_ " << endl;
    cout << "| |\\/| | | '_ \\| | | __| | __|" << endl;
    cout << "| |  | | | | | | | |_\\ \\ | |_ " << endl;
    cout << "\\_|  |_/_|_| |_|_|\\____/_|\\__|" << endl;
    cout << "                              " << endl;
}

void menu() {
    printMINIGIT();
    cout << "===============================" << endl;
    cout << "[1] Initialize" << endl;
    cout << "[2] Add" << endl;
    cout << "[3] Remove" << endl;
    cout << "[4] Commit" << endl;
    cout << "[5] Checkout" << endl;
    cout << "[6] Print current commit" << endl;
    cout << "[7] Remote (Start online server)" << endl;
    cout << "[8] Quit" << endl;
    cout << "===============================" << endl;
    cout << "[*] Select a numerical option:" << endl;
    cout << ">> ";
}


int main() {
    Git* GIT;
    GIT = new Git();
   
    bool initialized = false;
    bool quit = false;
    string s_input;
    int input;



    while(!quit) {
        menu();

        cin >> s_input;

        try {
            input = stoi(s_input);
        }

        catch (exception& e) {
            input = 99;
        }

        switch(input) {
            case 1: {
                if(!initialized) { // creates a hidden folder called .minigit  && exists(".minigit") == false
                    initialized = true;
                    GIT->createDirectory();
                    cout << "[+] Initialization successfull\n" << endl;
                }
                else cout << "No need to initialize. Directory already exists.\n" << endl;
                break;
            }

            case 2: {
                string fileName;
                bool first = true;
                bool found = false;

                while(!found || first) { // file does not exist = prompt again
                    cout << "[*] Please enter the file name" << endl;
                    cout << ">>";
                    cin >> fileName;

                    if(exists(fileName)) found = true; 
                    first = false;
                    GIT->addFile(fileName);
                }
               
                break;
            }

            case 3: {
                string fileName;
               
                if(!GIT->checkCommitList()) {
                    cout << "[-] Operation unsuccessfull. Commit list empty. \n" << endl;
                    break;
                }
            
                cout << "[*] Enter the file name" << endl;
                GIT->printCurrCommit();
                cout << ">> ";
                cin >> fileName;
                if(!GIT->exist(fileName)) {
                    cout << "[-] The file was not found in the current version of repository.\n" << endl;
                }
                
                else
                GIT->removeCommit(fileName);

                break;
            }

            case 4: {
                GIT->commit();
                break;
            }

            case 5: {
                int commitNUmber;
                cout << "[*] Enter a commit number" << endl;
                cin >> commitNUmber;

                GIT->checkOut(commitNUmber);

                break;
            }

            case 6:
                GIT->printCurrCommit();
                break;

            case 7: {
                GIT->startServer();
                break;
            }

            case 8:
                GIT->~Git();
                quit = true;
                break;

            default:
                cout << "Invalid input" << endl;
                break;
        }

    }


    return 0;
}