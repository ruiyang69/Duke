#include "query_funcs.h"


void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
     work txn(*C);
     string query = "";
     query += "INSERT INTO PLAYER (team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg) VALUES (";
     query += to_string(team_id) + ", " + to_string(jersey_num) + ", " + txn.quote(first_name) + ", ";
     query += txn.quote(last_name) + ", " + to_string(mpg) + ", " + to_string(ppg) + ", " + to_string(rpg) + ", ";
     query += to_string(apg) + ", " + to_string(spg) + ", " + to_string(bpg) + "); ";
     txn.exec(query);
     txn.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
     work txn(*C);
     string query = "";
     query += "INSERT INTO TEAM (name, state_id, color_id, wins, losses) VALUES (";
     query += txn.quote(name) + ", " + to_string(state_id) + ", " + to_string(color_id) + ", ";
     query += to_string(wins) + ", " + to_string(losses) + "); ";
     txn.exec(query);
     txn.commit();
}


void add_state(connection *C, string name)
{
     work txn(*C);
     string query = "";
     query += "INSERT INTO STATE (name) VALUES (" + txn.quote(name) + "); ";
     txn.exec(query);
     txn.commit();
}


void add_color(connection *C, string name)
{
     work txn(*C);
     string query = "";
     query += "INSERT INTO COLOR (name) VALUES (" + txn.quote(name) + "); ";
     txn.exec(query);
     txn.commit();
}


void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
     int use[6] = {use_mpg, use_ppg, use_rpg, use_apg, use_spg, use_bpg};
     double min[6] = {min_mpg, min_ppg, min_rpg, min_apg, min_spg, min_bpg};
     double max[6] = {max_mpg, max_ppg, max_rpg, max_apg, max_spg, max_bpg};
     const char* name[6] = {"MPG", "PPG", "RPG", "APG", "SPG", "BPG"};
     stringstream ss;
     ss << "SELECT * FROM PLAYER";
     for(int i=0; i<6; i++) {
          if(use[i]) {
               if(i == 0) {
                    ss << " WHERE ";
               }
               else {
                    ss << " AND ";
               }
               ss << "( " << name[i] << " >= " << min[i] << " AND " << name[i] << " <= " << max[i] << " )";
          }
     }
     ss << ";";
     
     nontransaction *ptr; 
     result * res;
     try{
          ptr = new nontransaction(*C);
          res = new result(ptr->exec(ss.str()));
     }
     catch (std::exception &e) {
          cout << e.what() << endl;
          return;
     }
     cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << endl;
     for(result::iterator it = res->begin(); it != res->end(); it++) {
          cout << it[0].as<int>() << " " << it[1].as<int>() << " " << it[2].as<int>() << " " 
               << it[3].as<string>() << " " << it[4].as<string>() << " " << it[5].as<int>() << " " 
               << it[6].as<int>() << " " << it[7].as<int>() << " " << it[8].as<int>() << " " 
               << it[9].as<float>() << " " << it[10].as<float>() << endl;
     }
     ptr->commit();

}


void query2(connection *C, string team_color)
{
     stringstream ss;
     ss << "SELECT TEAM.NAME FROM COLOR, TEAM WHERE COLOR.NAME = '" << team_color 
          << "' AND COLOR.COLOR_ID = TEAM.COLOR_ID;";

     nontransaction *ptr; 
     result * res;
     try{
          ptr = new nontransaction(*C);
          res = new result(ptr->exec(ss.str()));
     }
     catch (std::exception &e) {
          cout << e.what() << endl;
          return;
     }
     cout << "NAME" << endl;
     for(result::iterator it = res->begin(); it != res->end(); it++) {
          cout << it[0].as<string>() << endl;
     }
     ptr->commit();
}


void query3(connection *C, string team_name)
{
     stringstream ss;

	ss << "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME FROM PLAYER, TEAM " \
	"WHERE TEAM.NAME = '" << team_name << "' AND PLAYER.TEAM_ID = TEAM.TEAM_ID ORDER BY PPG DESC;";

     nontransaction *ptr; 
     result * res;
     try{
          ptr = new nontransaction(*C);
          res = new result(ptr->exec(ss.str()));
     }
     catch (std::exception &e) {
          cout << e.what() << endl;
          return;
     }
     cout << "FIRST_NAME LAST_NAME" << endl;
     for(result::iterator it = res->begin(); it != res->end(); it++) {
          cout << it[0].as<string>() << " " << it[1].as<string>() << endl;
     }
     ptr->commit();
}


void query4(connection *C, string team_state, string team_color)
{
     stringstream ss;

	ss << "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, PLAYER.UNIFORM_NUM FROM PLAYER, TEAM, STATE, COLOR " \
	"WHERE STATE.NAME = '" << team_state << "' AND COLOR.NAME = '" << team_color << "' "\
	"AND TEAM.COLOR_ID = COLOR.COLOR_ID AND TEAM.STATE_ID = STATE.STATE_ID AND PLAYER.TEAM_ID = TEAM.TEAM_ID;";

     nontransaction *ptr; 
     result * res;
     try{
          ptr = new nontransaction(*C);
          res = new result(ptr->exec(ss.str()));
     }
     catch (std::exception &e) {
          cout << e.what() << endl;
          return;
     }
     cout << "FIRST_NAME LAST_NAME UNIFORM_NUM" << endl;
     for(result::iterator it = res->begin(); it != res->end(); it++) {
          cout << it[0].as<string>() << " " << it[1].as<string>() <<" " << it[2].as<string>() << endl;
     }
     ptr->commit();
}


void query5(connection *C, int num_wins)
{
     stringstream ss;

	ss << "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, TEAM.NAME, TEAM.WINS FROM PLAYER, TEAM" \
     " WHERE TEAM.WINS > " << num_wins << " AND TEAM.TEAM_ID = PLAYER.TEAM_ID;";

     nontransaction *ptr; 
     result * res;
     try{
          ptr = new nontransaction(*C);
          res = new result(ptr->exec(ss.str()));
     }
     catch (std::exception &e) {
          cout << e.what() << endl;
          return;
     }
     cout << "FIRST_NAME LAST_NAME NAME WINS" << endl;
     for(result::iterator it = res->begin(); it != res->end(); it++) {
          cout << it[0].as<string>() << " " << it[1].as<string>() << " " << it[2].as<string>() 
               << " " << it[3].as<string>() << endl;
     }
     ptr->commit();
}
