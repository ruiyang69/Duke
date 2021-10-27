import os
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "orm.settings")

import django
django.setup()

from acc.models import Color, State, Team, Player

def add_color():
     f = open('color.txt')
     l = []
     for line in f:
          color_id, name = line.split(' ')
          l.append(Color(color_id = color_id, name = name[:-1]))
     f.close()
     Color.objects.bulk_create(l)


def add_state():
     f = open('state.txt')
     l = []
     for line in f:
          state_id, name = line.split(' ')
          l.append(State(state_id = state_id, name = name[:-1]))
     f.close()
     State.objects.bulk_create(l)


def add_team():
     f = open('team.txt')
     l = []
     for line in f:
          team_id, name, state_id, color_id, wins, losses= line.split(' ')
          l.append(Team(team_id = team_id, name = name, state_id = State.objects.get(state_id = state_id), 
               color_id = Color.objects.get(color_id = color_id), wins = wins, losses = losses))
     f.close()
     Team.objects.bulk_create(l)

def add_player():
     f = open('player.txt')
     l = []
     for line in f:
          player_id, team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg = line.split(' ')
          l.append(Player(player_id=player_id,team_id=Team.objects.get(team_id = team_id),
               uniform_num=uniform_num,first_name=first_name,last_name=last_name,
               mpg=mpg,ppg=ppg,rpg=rpg,apg=apg,spg=spg,bpg=bpg))
     f.close()
     Player.objects.bulk_create(l)


def query1(use_mpg, min_mpg, max_mpg, use_ppg, min_ppg, max_ppg, use_rpg, 
     min_rpg, max_rpg, use_apg, min_apg, max_apg, use_spg, min_spg, max_spg, use_bpg, min_bpg, max_bpg):

     res = Player.objects.all()
     if use_mpg:
          res = res.filter(mpg__gte = min_mpg, mpg__lte = max_mpg)
     if use_ppg:
          res = res.filter(ppg__gte = min_ppg, ppg__lte = max_ppg)
     if use_rpg:
          res = res.filter(rpg__gte = min_rpg, rpg__lte = max_rpg)
     if use_apg:
          res = res.filter(apg__gte = min_apg, apg__lte = max_apg)
     if use_spg:
          res = res.filter(spg__gte = min_spg, spg__lte = max_spg)
     if use_bpg:
          res = res.filter(bpg__gte = min_bpg, bpg__lte = max_bpg)

     print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")
     for player in res:
          print(player.player_id, player.team_id.team_id, player.uniform_num, player.first_name, player.last_name,
               player.mpg, player.ppg, player.rpg, player.apg, player.spg, player.bpg)


def query2(team_color):
     res = Team.objects.filter(color_id__name = team_color)
     print("NAME")
     for team in res:
          print(team.name)


def query3(team_name):
     res = Player.objects.filter(team_id__name = team_name).order_by('-ppg')

     print("FIRST_NAME LAST_NAME")
     for player in res:
          print(player.first_name, player.last_name)


def query4(team_state, team_color):
     state_id = State.objects.filter(name = team_state).first()
     color_id = Color.objects.filter(name = team_color).first()
     res = Player.objects.filter(team_id__state_id = state_id, team_id__color_id = color_id).reverse()
     print("FIRST_NAME LAST_NAME UNIFORM_NUM")
     for player in res:
          print(player.first_name, player.last_name, player.uniform_num)


def query5(num_wins):
     res = Player.objects.filter(team_id__wins__gt = num_wins)
     print("FIRST_NAME LAST_NAME NAME WINS")
     for player in res:
          print(player.first_name, player.last_name, player.team_id.name, player.team_id.wins)


def create_table():
     add_color()
     add_state()
     add_team()
     add_player()


def run():
     # query1(1, 10, 15, 0, 0, 20, 0, 0, 6, 0, 0, 5, 0, 0.0, 10.0, 0, 0.0, 10.0)
     # query1(0, 10, 15, 1, 0, 0, 0, 0, 6, 0, 0, 5, 0, 0.0, 10.0, 0, 0.0, 10.0)

     # query2('bla')
     # query2('Red')

     # query3('USA')
     # query3('NCSU')

     # query4("NC", "bla")
     # query4("NC", "DarkBlue")

     query5(-1)
     query5(13)


def main():
     create_table()
     run()



if __name__ == '__main__':
    main()