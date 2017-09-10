/*
 uxload
 written by Jared Bruni
 
 License: GNU LESSER GENERAL PUBLIC LICENSE Version 3
 
*/

#include"program_source.hpp"
#include<iostream>
#include<fstream>

int main(int argc, char **argv) {
    if(argc == 2) {
        loader::Loader l;
        l.loadIp("ip.cfg");
        if(!l.startListen(argv[1])) {
            std::cerr << "uxload: Error could not listen..\n";
            exit(EXIT_FAILURE);
        }
    } else {
        
        if(argc == 4) {
            std::fstream file;
            std::string host = argv[1];
            std::string port = argv[2];
            std::string filename = argv[3];
            file.open(filename, std::ios::in | std::ios::binary);
            if(!file.is_open()) {
                std::cerr << "uxload: Error could not open file: " << filename << "\n";
                exit(EXIT_FAILURE);
            }
            net::Socket socket_;
            if(socket_.connectToHost(host, port, net::SocketType::STREAM) == -1) {
                std::cerr << "uxload: Error on connection to: " << host << " " << port << " ..\n";
                exit(EXIT_FAILURE);
            }
            while(!file.eof()) {
                char buf[1024*4];
                file.read(buf, (1024*4));
                socket_.sendData(buf, file.gcount());
            }
            file.close();
            socket_.closeSocket();
            exit(EXIT_SUCCESS);
        } else {
            std::cerr << "uxload: for listening use:\n" << argv[0] << " port\n";
            std::cerr << "uxload: for connecting requires three arguments.\n" << argv[0] << " target port program\n";
        }
        
    }
    
    
    return 0;
}
