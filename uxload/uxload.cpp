/*
 uxload
 written by Jared Bruni
 
 License: GNU LESSER GENERAL PUBLIC LICENSE Version 3
 
 */

#include"program_source.hpp"
#include<iostream>
#include<fstream>

enum class Mode { mode_null, mode_listen, mode_send };

int main(int argc, char **argv) {
    std::string listen_port;
    std::string send_name;
    const std::string ip_config_file = "ip.cfg";
    Mode mode = Mode::mode_null;
    if(argc > 1) {
        int opt = 0;
        while((opt = getopt(argc, argv, "l:s:")) != -1) {
            if(mode != Mode::mode_null) break;
            
            switch(opt) {
                case 'l':
                    listen_port = optarg;
                    mode = Mode::mode_listen;
                    break;
                case 's':
                    send_name = optarg;
                    mode = Mode::mode_send;
                    break;
            }
        }
    }
    
    if(mode == Mode::mode_listen) {
        loader::Loader l;
        const char *cfg = getenv("UXLOAD_CFG");
        if(cfg == 0)
            l.loadIp(ip_config_file);
        else
            l.loadIp(cfg);
        
        if(!l.startListen(listen_port)) {
            std::cerr << "uxload: Error could not listen..\n";
            exit(EXIT_FAILURE);
        }
    } else if(mode == Mode::mode_send) {
        const char *ip = getenv("UXLOAD_NET");
        if(ip == NULL) {
            std::cerr << "uxload: First export UXLOAD_NET with ip:port as envoriment variable.\n\n";
            exit(EXIT_FAILURE);
        }
        std::string ip_a = ip;
        auto pos = ip_a.find(":");
        if(pos == std::string::npos) {
            std::cerr << "uxload: Error invalid ip address format. Use ip:port\n";
            exit(EXIT_FAILURE);
        }
        std::fstream file;
        std::string host = ip_a.substr(0, pos);
        std::string port = ip_a.substr(pos+1, ip_a.length()-pos);
        std::string filename = send_name;
        std::cout << "uxload: ip " << host << " port " << port << "\n";
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
        ssize_t sent_bytes = 0;
        while(!file.eof()) {
            char buf[1024*4];
            file.read(buf, (1024*4));
            sent_bytes += socket_.sendData(buf, file.gcount());
        }
        file.close();
        socket_.closeSocket();
        std::cout << "uxload: Sent " << sent_bytes << " bytes..\n";
        exit(EXIT_SUCCESS);
    }  else {
        std::cout << "usage: uxload [-l port] [-s program]\n";
        std::cerr << "uxload: for listening use:\n" << argv[0] << " -l port\n";
        std::cerr << "uxload: export UXLOAD_CFG with filename to specify config file with different name than ip.cfg\n";
        std::cerr << "uxload: First for connecting export target ip and port like this in bash:\nexport UXLOAD_NET=\"192.168.8.85:10241\"\n Ip:port\n";
        std::cerr << "uxload: for connecting use:\n" << argv[0] << "  -s program\n";
        
    }
    return 0;
}
