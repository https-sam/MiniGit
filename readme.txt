MiniGit
Creater: Sam Goto


[*] Requires c++17 to compile
    g++ -std=c++17 miniGit.cpp driver.cpp

[*] For online server, it requires apache2/miniupnpc installed (For miniupnpc, you have an option to install in the program)


Main Menu

[1] Initialize
[2] Add
[3] Remove
[4] Commit
[5] Checkout
[6] Print current commit
[7] Remote (Start online server)
[8] Quit


[1] Initialize - creates a hidden folder named .minigit in the current directory, which will store all committed files

[2] Add - adds files to the commit list

[3] Remove - removes files from the current commit list

[4] Commit - stores the files in the currrent commit list into the hidden directory

[5] checkout - copies desired version of committed files into your current working directory (This will OVERWRITE the files)

[6] Print current commit - prints all the files in your current commit list 

[7] Remote (Start online server) - creates local/online server and stores files in a webserver

        [1] Local server - copies desired version of committed files into local webserver

        [2] Public server - copies desired version of committed files into local webserver and performs fort forwarding, 
                            this will let you access to your files anyware in the world.
                            [*] Requires miniupnpc (if you don't have it installed, this program will automatically install it)
                            [*] Opens port 80, it will automatically close in about 10-20 hours.

        [*] When you exit out the program, server will automatically be terminated

[8] Quit - terminates the program, memories are freed
