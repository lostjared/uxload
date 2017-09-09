#include "program_source.hpp"

namespace loader {
    
    Loader::Loader() {
        
    }
    
    bool Loader::startListen(const std::string &port) {
        int s = socket_.listenAt(port, net::SocketType::STREAM, 5);
        if(s == -1) {
            std::cerr << "uxload: Error on listen..\n";
            return false;
        }
        std::cout << "uxload: Listening..\n";
        while(1) {
        	net::Socket sp;
            int rt_val = socket_.acceptSocket(sp);
            if(rt_val == -1) {
                std::cerr << "uxload: Error on accept.\n";
                continue;
            }
            socklen_t len;
            struct sockaddr_storage addr;
            char ipstr[INET6_ADDRSTRLEN];
            int port;
            len = sizeof(addr);
            getpeername(sp.fd(), (struct sockaddr*)&addr, &len);
            if (addr.ss_family == AF_INET) {
                struct sockaddr_in *s = (struct sockaddr_in *)&addr;
                port = ntohs(s->sin_port);
                inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof(ipstr));
            } else { // AF_INET6
                struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
                port = ntohs(s->sin6_port);
                inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof(ipstr));
            }
            std::string ip;
            ip = ipstr;
            std::string fip;
            auto pos = ip.rfind(":");
            if(pos != std::string::npos) {
                ip = ip.substr(pos+1, ip.length());
            }
            procCode(ip, sp);
        }
    }
    
    void Loader::procCode(const std::string &ip, net::Socket &s) {
        bool found = false;
        for(unsigned int i = 0; i < ip_address.size(); ++i) {
            if(ip_address[i] == ip) {
                found = true;
                break;
            }
        }
        if(found == false) {
            std::cerr << "Error ip address: " << ip << " not on the accepted list.\n";
            return;
        }
        std::cout << "uxload: Receiving program..\n";
        std::fstream file;
        file.open("program", std::ios::out | std::ios::binary);
        char buf[1024*4];
        while(1) {
            ssize_t len = s.receive(buf, 1024*4);
            if(len > 0)
            file.write(buf, len);
            else break;
        }
        file.close();
        std::cout << "uxload: Loading program..\n";
        chmod("program", 755);
        System("./program");
    }
    
    void Loader::acceptIp(const std::string &ip) {
        ip_address.push_back(ip);
    }
    
    void Loader::loadIp(const std::string &text) {
        std::fstream file;
        file.open(text, std::ios::in);
        if(!file.is_open()) {
            std::cerr << "uxload: Error put list of accepted ip addresses in: " << text << "\n";
            exit(EXIT_FAILURE);
        }
        
        while(!file.eof()) {
            std::string ip;
            std::getline(file, ip);
            if(file) {
                ip_address.push_back(ip);
                std::cout << "uxload: Added accepted ip: "<< ip << "\n";
            }
        }
        file.close();
    }
    
    void Loader::close() {
        
    }
    
    int Loader::System(const std::string &command) {
        sigset_t bmask, omask;
        struct sigaction sa_ignore, sa_oquit, sa_origint, sa_default;
        pid_t id;
        int status, serrno;
        
        if(command == "") return System(":") == 0;
        
        sigemptyset(&bmask);
        sigaddset(&bmask, SIGCHLD);
        sigprocmask(SIG_BLOCK, &bmask, &omask);
        sa_ignore.sa_handler = SIG_IGN;
        sa_ignore.sa_flags = 0;
        sigemptyset(&sa_ignore.sa_mask);
        sigaction(SIGINT, &sa_ignore, &sa_origint);
        sigaction(SIGQUIT, &sa_ignore, &sa_oquit);
        
        switch((id = fork())) {
            case -1:
                status = -1;
                break;
            case 0:
                sa_default.sa_handler = SIG_DFL;
                sa_default.sa_flags = 0;
                sigemptyset(&sa_default.sa_mask);
                if(sa_origint.sa_handler != SIG_IGN)
                    sigaction(SIGINT, &sa_default, NULL);
                if(sa_oquit.sa_handler != SIG_IGN)
                    sigaction(SIGQUIT, &sa_default, NULL);
                
                execl("/bin/sh", "sh", "-c", command.c_str(), (char*)NULL);
                _exit(127);
                break;
            default:
                while(waitpid(id, &status, 0) == -1) {
                    if(errno != EINTR) {
                        status = -1;
                        break;
                    }
                }
                break;
        }
        serrno = errno;
        sigprocmask(SIG_SETMASK, &omask, NULL);
        sigaction(SIGINT, &sa_origint, NULL);
        sigaction(SIGQUIT, &sa_oquit, NULL);
        errno = serrno;
        return status;
    }
    
}
