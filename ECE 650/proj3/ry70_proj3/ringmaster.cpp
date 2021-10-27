#include "potato.h"

void ringmaster::run_game(){
     cout << "Potato Ringmaster" << endl;
     cout << "Players = " << players << endl;
     cout << "Hopes = " << hops << endl;
     
     // set up master server
     socket_setup(port_string);

     get_port();
     // master is ready listening, need to build tcp connections to players
     player_setup();

     if(hops == 0) return;

     // palyers are online and neighbor info sent, time to play
     play_game();
}

void print_trace(potato p) {
          cout << "Trace of potato:" << endl;
          for(int i=0; i<p.count-1; i++){
               cout << p.path[i] << ",";
          }
          cout << p.path[p.count-1] << endl;
     }

void ringmaster::player_setup(){
     for (int i=0; i<players; i++) {
          accept_setup(player_ip[i]);
          player_fd[i] = accept_fd;
          // send out info for player program to print out
          assert(send(player_fd[i], (void*)&i, sizeof(int), 0) == sizeof(int));
          assert(send(player_fd[i], (void*)&players, sizeof(int), 0) == sizeof(int));

          // player responds if ready
          assert(recv(player_fd[i], &player_port[i], sizeof(player_port[i]), MSG_WAITALL)==sizeof(int));
          cout << "Player " << i << " is ready to play" << endl;
          // cout << "new player has ip: " << player_ip[i] << " and port: " << player_port[i] << endl;
     }
     // if (players == 2) {
     //      neighbor_info n_right;
     //      n_right.port = player_port[1];
     //      strcpy(n_right.ip_addr, player_ip[1].c_str());
     //      assert(send(player_fd[0], &n_right, sizeof(n_right), 0) == sizeof(n_right));
     //      return;
     // }

     // now all players are online, master sends out right neighbor infomation
     for (int i=0; i<players; i++) {
          neighbor_info  n_right;
          if(i == players - 1) {
               n_right.port = player_port[0];
               strcpy(n_right.ip_addr, player_ip[0].c_str());
          }
          else {
               n_right.port = player_port[i+1];
               strcpy(n_right.ip_addr, player_ip[i+1].c_str());
          }

          // send out
          // note: only right neighbor info is sent as right neighbor is acting as a client 
          // to any current player and player is a client to its left
          assert(send(player_fd[i], &n_right, sizeof(n_right), 0) == sizeof(n_right));
     }
     // after sending out neighbor info, wait for all players to tell master that they have
     // connected with neighbors

     for(int i=0; i<players; i++) {
          char msg[100] = "neighbor connection completed";
          assert(recv(player_fd[i], &msg, sizeof(msg), MSG_WAITALL) > 0);
     }
     // cout << "all players have connected" << endl;
}

void ringmaster::play_game() {
     // potato set up
     potato p;
     memset(&p, 0, sizeof(p));
     p.hops = hops;
     // start game by selecting a random player
     srand((unsigned int)time(NULL));
     int first_player = rand() % players;
     cout << "Ready to start the game, sending potato to player " << first_player << endl;
     if(send(player_fd[first_player], &p, sizeof(p), 0) != sizeof(p)){
          error_print("sending potato failed");
     }

     // game ends if receives potato back, send out info for players to shut down
     fd_set readfds;
     FD_ZERO(&readfds);
     for(int i=0; i<players; i++) {
          FD_SET(player_fd[i], &readfds); //put all fds into set
     }
     int max_fd = *max_element(player_fd.begin(), player_fd.end());
     select(max_fd+1, &readfds, NULL, NULL, NULL);
     
     // now check any status change, i.e. a potato has been sent back
     for(int i=0; i<players; i++) {
          if(FD_ISSET(player_fd[i], &readfds)) {
               assert(recv(player_fd[i], &p, sizeof(p), MSG_WAITALL) > 0);
               // cout << "receive potato back" << endl;
               // double check the hops is zero
               p.flag = 1;
               for(int j=0; j<players; j++) {
                    // send a potato with zero hops to players so that they will shut down gracefully
                    assert(send(player_fd[j], &p, sizeof(p), 0) == sizeof(p));
               }
               break; //no long checking status
          }
     }

     // print trace and sleep
     print_trace(p);
     sleep(1);
}



int main(int argc, char** argv) {
     if (argc != 4) {
          perror("Please re-enter ringmaster info\n");
          return EXIT_FAILURE;
     }
     ringmaster* rm = new ringmaster(argv);
     rm->run_game();
     delete rm;
     return EXIT_SUCCESS;
}
