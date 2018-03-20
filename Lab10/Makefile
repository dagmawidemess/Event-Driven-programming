all:rpg
Player.o:Player.c Player.h UNIXBOARD.h
	gcc -c Player.c
Game.o: Game.c Game.h
	gcc -c Game.c
rpg.o:rpg.c Game.h Player.h
	gcc -c rpg.c
rpg: rpg.o Game.o Player.o
	gcc rpg.o Game.o Player.o -o rpg
	  
clean:
	rm *.o rpg
