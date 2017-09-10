/*
 uxload
 written by Jared Bruni
 
 License: GNU LESSER GENERAL PUBLIC LICENSE Version 3
 
*/

#ifndef __PROGRAM_SOURCE_H__
#define __PROGRAM_SOURCE_H__

#include "socket.hpp"
#include<vector>
#include<string>
#include<fstream>
#include<unistd.h>
#include<sys/stat.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<errno.h>

namespace loader {
    
    class Loader {
    public:
        Loader();
        bool startListen(const std::string &port);
        void acceptIp(const std::string &ip);
        void loadIp(const std::string &text);
        void close();
    private:
        std::string pw;
        std::vector<std::string> ip_address;
        net::Socket socket_;
        
        void procCode(const std::string &ip, net::Socket &s);
        int System(const std::string &command);
    };
    
    
}

#endif
