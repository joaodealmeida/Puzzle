//
//  game.h
//  Puzzle
//

#include "table.hpp"
#include "io.h"
#include "functions.hpp"

class game {
    table *atable;
    io theIo;
    
    void Create_Game();
    void TRAINING_MODE();
    void COMPETITION_MODE();
    void Next_Move();
	void LoadGame(int table_size , int gameid, table *atable);

	int _handleArrowKeyInput();
    
public:
    
    void MainMenu();
    

};

