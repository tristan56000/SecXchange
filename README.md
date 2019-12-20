# SecXchange

Development of an exchanging encrypted messages application in language C++. 
RSA protocol used to encrypt the shared messages. 
Qt Creator 5.13 or later used for compiling and executing the source code.


### Description

SecXchange is an exchanging encrypted messages application I and Grégoire Philippe had developped in C++ language, using Qt Creator.

This application is based on a client/server model in order to share messages between two users on the same local network.

Every messages are encrypted using RSA asymetric protocol to prevent a man-in-the-middle attacker to read clear messages by listening the network frame with tools such as WireShark.


### How to use it ?

Both users had to use the deployment version according to their OS.
Two files must be added in the executable folder in order to make it work : "public_RSA_keys.txt", listing all users public keys, and "private_RSA_keys.txt", file containing the private RSA key of the user encrypted with a symetric protocol.

When launching the application, you will have to enter both your username and password.
Then, you got to choose the client or server side.

The server user must choose an ip and a port to open a connection on.

The client user must choose the ip server and connection port.

They can chat securely after that.


### Windows version

For now on, the executable is in the 'debug' folder, in 'buildWindows'.
Make sure you got all the needed ddls to run the application.

Make sure you firewall isn't blocking the connection, and that your network allows connections on the choosen port.


### Linux version

For now on, the executable is in the 'buildLinux' folder.
