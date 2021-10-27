#ifndef potato_h
#define potato_h

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <assert.h>
#include <vector>
#include <algorithm> 

#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>

using namespace std;

struct neighbor_info {
  char ip_addr[100];
  int port;
};

struct potato{
     int count, hops, flag;
     int path[512];
};

class node{
public:
     struct addrinfo hints, *head;
     int socket_fd, accept_fd, port;

     void socket_setup(const char* port_num) {
          //hint init
          
          memset(&hints, 0, sizeof(hints));
          hints.ai_family = AF_UNSPEC;         // sockaddr_in family requirement
          hints.ai_socktype = SOCK_STREAM;   // tcp reliable connetion
          hints.ai_flags = AI_PASSIVE;       // system will pick

          if (getaddrinfo(NULL, port_num, &hints, &head) != 0) {
               cout << port_num << endl;
               error_print("getaddrinfo error");
          }

          socket_fd = socket(head->ai_family, head->ai_socktype, head->ai_protocol);
          if (socket_fd == -1){
               error_print("socket creation failed");
          }

          // ref Beej's page 24
          // usage: to reuse a port
          int yes = 1;
          if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
               error_print("setsockopt failed");
          }     

          // bind
          bind(socket_fd, head->ai_addr, head->ai_addrlen);

          // listen for connection
          
          if (listen(socket_fd, 10) == -1){
               error_print("listen failed");
          }

          freeaddrinfo(head);
     }
     
     int get_port() {
          struct sockaddr_in sin;
          socklen_t len = sizeof(sin);
          if (getsockname(socket_fd, (struct sockaddr *)&sin, &len) == -1){
               error_print("get sock name error");
          }
          port = ntohs(sin.sin_port);
          return ntohs(sin.sin_port);
     }

     void accept_setup(string &ip_addr) {
          struct sockaddr_in sock_addr_str;
          socklen_t t = sizeof(sock_addr_str);
          
          // accept calling convention
          accept_fd = accept(socket_fd, (struct sockaddr*)&sock_addr_str, &t);
          if(accept_fd == -1) {
               error_print("accpet failed");
          }
          ip_addr = inet_ntoa(sock_addr_str.sin_addr);
     }

     void error_print(const string err_msg){
          cout << err_msg << endl;
          exit(EXIT_FAILURE);
     }

     node():socket_fd(0),accept_fd(0),port(0){}
     ~node() {
          close(socket_fd);
          close(accept_fd);
     }
};

class ringmaster : public node{
public:
     int players, hops;
     char* port_string;
     vector<int> player_fd, player_port;
     vector<string> player_ip;

     ringmaster(char** input){
          startup(input);
     }
     ~ringmaster(){
          for(int fd : player_fd){
               close(fd);
          }
     }

     void startup(char** input){
          port_string = input[1];
          port = atoi(input[1]);
          players = atoi(input[2]);
          if(players <= 1){
               error_print("there must be at least two players");
          }
          hops = atoi(input[3]);
          if(hops < 0 || hops > 512){
               error_print("number of hops is incorrect");
          }
          player_fd.resize(players);
          player_port.resize(players);
          player_ip.resize(players);
     }

     void run_game();
     void player_setup();
     void play_game();
};

class player : public node{
public:
     int id, players, master_fd, right_fd, right_port;
     char*  right_ip;

     player(char** argv){
          connect_to_master(argv[1], argv[2]);
     }
     ~player() {
          close(master_fd);
          close(right_fd);
     }

     void connect_to_master(const char* master_ip, const char* master_port);
     void connect_to_neighbor();
     void connect_to_server(const char* host, const char* port_num, int& fd);
     void start();
     void keep_waiting();
};

#endif
