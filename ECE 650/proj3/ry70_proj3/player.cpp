#include "potato.h"

void player::connect_to_server(const char* host, const char* port_num, int &fd){
     //hint init
     memset(&hints, 0, sizeof(hints));
     hints.ai_family = AF_UNSPEC;         // sockaddr_in family requirement
     hints.ai_socktype = SOCK_STREAM;     // tcp reliable connetion
     hints.ai_protocol = 0;

     if (getaddrinfo(host, port_num, &hints, &head) != 0) {
          error_print("getaddrinfo error");
     }

     fd = socket(head->ai_family, head->ai_socktype, head->ai_protocol);

     if (fd == -1){
          error_print("socket creation failed");
     }

     if(connect(fd, head->ai_addr, head->ai_addrlen) == -1) {
          error_print("connection to server failed");
     }

     freeaddrinfo(head);
}


void player::connect_to_master(const char* master_addr, const char* master_port) {
     // build connection
     connect_to_server(master_addr, master_port, master_fd);

     // cout << "connect to server succeeds" << endl;
     // receive current id
     recv(master_fd, &id, sizeof(id), MSG_WAITALL);

     // receive number of players
     recv(master_fd, &players, sizeof(players), MSG_WAITALL);

     // tell ringmaster current player is online
     socket_setup(master_port);

     port = get_port();

     send(master_fd, &port, sizeof(port), 0);
     std::cout << "Connected as player " << id << " out of " << players << " total players" << endl;
}


void player::connect_to_neighbor() {
     neighbor_info  n_right;

     // retrive right neighbor info from master
     
     recv(master_fd, &n_right, sizeof(n_right), MSG_WAITALL);

     char port_num_right[10];
     sprintf(port_num_right, "%d", n_right.port);

     // actively connect to right neighbor
     if(players == 2 && id == 1){}
     else {
          connect_to_server(n_right.ip_addr, port_num_right, right_fd);
     }
     // assert(send(right_fd, &id, sizeof(id), 0) > 0);
     // char msg[100] = "hi, right neighbor";
     // assert(send(right_fd, &msg, sizeof(msg), 0) != -1);

     // passively connect to left
     string left_ip;
     if(players == 2 && id == 0) {}
     else{
          accept_setup(left_ip);
     }
     
     // cout << "right neighbor has ip: " << n_right.ip_addr << " with port number: " << port_num_right << endl;
     // cout << "left neighbor has ip: " << left_ip << endl;
     // assert(recv(accept_fd, &msg, sizeof(msg), MSG_WAITALL) != -1);
     // cout << "connected to right neighbor, msg: " << msg  << endl;

     // // now have connected to right, need to accept connection request from left neighbor
     // int left_id = 100;
     // int right_id = 100;

     // assert(send(accept_fd, &id, sizeof(id), 0) == sizeof(id));
     // assert(recv(accept_fd, &left_id, sizeof(left_id), MSG_WAITALL) == sizeof(id));
     // assert(recv(right_fd, &right_id, sizeof(left_id), MSG_WAITALL) == sizeof(id));

     // tell master neighbor connection finished;
     char msg_1[100] = "neighbor connection completed";
     assert(send(master_fd, &msg_1, sizeof(msg_1), 0) != -1);
     // cout << "told master" << endl;
}

void player::keep_waiting() {
     
     srand((unsigned int)time(NULL)+id); // random seed
     fd_set readfds;

     while(1){
          // fd set
          potato p; // needs to be filled from master 
          memset(&p, 0, sizeof(p));
          FD_ZERO(&readfds);
          FD_SET(accept_fd, &readfds);
          FD_SET(master_fd, &readfds);
          FD_SET(right_fd,  &readfds);
          
          int max_fd = accept_fd > master_fd ? accept_fd : master_fd;
          max_fd = max_fd > right_fd ? max_fd : right_fd;
          
          //timeout
          // struct timeval tv;
          // tv.tv_sec = 10;
          // tv.tv_usec = 0;
          assert(select(max_fd+1, &readfds, NULL, NULL, NULL) != -1);

          // now check who sends potato to current player
          if (FD_ISSET(accept_fd, &readfds)){
               assert(recv(accept_fd, &p, sizeof(p), MSG_WAITALL) > 0);
               // cout << "received potato from left neighbor" << endl;
          }
          else if (FD_ISSET(master_fd, &readfds)) {
               assert(recv(master_fd, &p, sizeof(p), MSG_WAITALL) > 0);
               // cout << "received potato from master" << endl;
          }
          else if (FD_ISSET(right_fd, &readfds)) {
               assert(recv(right_fd, &p, sizeof(p), MSG_WAITALL) > 0) ;
               // cout << "received potato from right neighbor" << endl;
          }
          
          if(p.flag == 1) {
               assert(p.hops==0);
               return;
          } 
          else {
               p.hops--;
               p.path[p.count++] = id;
          }


          if(p.hops == 0) { // the player is "it"
               assert(send(master_fd, &p, sizeof(p), 0) > 0); // send potato back to master
               cout << "I'm it" << endl;
               continue;
          }
          else { // send to a random neighbor
               int next = rand() % 2;
               if(players == 2 ) {
                    if(id == 1) {
                         assert(send(accept_fd, &p, sizeof(p), 0) > 0);
                         cout << "Sending potato to 0" << endl;
                    }
                    else {
                         assert(send(right_fd, &p, sizeof(p), 0) > 0);
                         cout << "Sending potato to 1" << endl;
                    }
               }
               else if (next == 0) { // send to left neighbor
                    assert(send(accept_fd, &p, sizeof(p), 0) > 0);
                    if (id == 0) {
                         cout << "Sending potato to " << players - 1 << endl;
                    }
                    else {
                         cout << "Sending potato to " << id - 1 << endl;
                    }
               }
               else { // send to right neighbor
                    assert(send(right_fd, &p, sizeof(p), 0) > 0);
                    if (id == players - 1) {
                         cout << "Sending potato to " << 0 << endl;
                    }
                    else {
                         cout << "Sending potato to " << id + 1 << endl;
                    }
               }
          }
     }
}


void player::start() {
     connect_to_neighbor();
     keep_waiting();
     sleep(1);
}

int main(int argc, char** argv) {
     if(argc != 3) {
          perror("invalid invoke command");
     }
     player* p = new player(argv);
     p->start();
     delete p;
     return EXIT_SUCCESS;
}
