#include <iostream>
#include <pqxx/pqxx>

#include "exerciser.h"



using namespace std;
using namespace pqxx;


void create_table(string fname, connection *C) {
    string query, line;
    ifstream f;
    f.open(fname.c_str(), ifstream::in);
    while(getline(f, line)) {
      query += line;
    }
    f.close();
    work txn(*C);
    txn.exec(query);
    // cout << "create database succeeds" << endl;
    txn.commit();
}

void load_player(string fname, connection *C) {
    string line, first_name, last_name;
    int player_id, team_id, uniform_num, mpg, ppg, rpg, apg;
    double spg, bpg;
    ifstream f;
    f.open(fname.c_str(), ifstream::in);
    while (std::getline(f, line)) {
      stringstream ss;
      ss << line;
      ss >> player_id >> team_id >> uniform_num >> first_name >> last_name >> mpg >> ppg >> rpg >> apg >> spg >> bpg;
      add_player(C, team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg);
    }
    f.close();
}

void load_team(string fname, connection *C)
{
    string team_name, line;
    int team_id, state_id, color_id, wins, losses;
    ifstream f;
    f.open(fname.c_str(), ifstream::in);
    while(getline(f,line)) {
      stringstream ss;
      ss << line;
      ss >> team_id >> team_name >> state_id >> color_id >> wins >> losses;
      add_team(C, team_name, state_id, color_id, wins, losses);
    }
    f.close();
}

void load_state(string fnmae, connection *C) {
    string state_name, line;
    int state_id;
    ifstream f;
    f.open(fnmae.c_str(), ifstream::in);
    while(getline(f, line)) {
      stringstream ss;
      ss << line;
      ss >> state_id >> state_name;
      add_state(C, state_name);
    }
    f.close();
}

void load_color(string fname, connection *C) {
    string color_name, line;
    int color_id;
    ifstream f;
    f.open(fname.c_str(), ifstream::in);
    while(getline(f, line)) {
      stringstream ss;
      ss << line;
      ss >> color_id >> color_name;
      add_color(C, color_name);
    }
    f.close();
}

int main(int argc, char *argv[])
{

  //Allocate & initialize a Postgres connection object
  connection *C;

  try
  {
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open())
    {
      cout << "Opened database successfully: " << C->dbname() << endl;
    }
    else
    {
      cout << "Can't open database" << endl;
      return 1;
    }
  }
  catch (const std::exception &e)
  {
    cerr << e.what() << std::endl;
    return 1;
  }

  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files

  try {
    create_table("init.sql", C);
    
    load_state("state.txt", C);
    load_color("color.txt", C);
    load_team("team.txt", C);
    load_player("player.txt", C);
  

    exercise(C);
  }
  catch (exception &e) {
    cout << e.what() << endl;
    return 1;
  }

  //Close database connection
  C->disconnect();

  return 0;
}
