//
//  game.cpp
//  Game Functions
//

#include "game.h"

#include <iostream>
#include <ctype.h>
#include <string>

#ifdef WIN32
#include <windows.h>
#include <conio.h>
#include <wincon.h>
#else
#warning "This source file won't compile correctly under any OS other than Windows. As much as we'd like to make it so (as all the other source files are), this was not viable considering the timeframe of the project. Maybe one day."
#endif

#include "piece.hpp"
#include "table.hpp"
#include "io.h"

#include "functions.hpp"

using namespace std;

void game::MainMenu() {
	functions::clearConsole();
	char gametype;

	cout << "Welcome to Puzzle!" << endl << endl;

	cout << "Instructions: Just play with your arrow or numkeys!" << endl << endl;

	cout << "Please choose an option:" << endl << endl;

	cout << "(T)raining Mode // (C)ompetition Mode // (E)xit Game: " << endl;
	cin >> gametype;
	gametype = toupper(gametype);
	switch (gametype) { // Choose the gametype
		case 'T':
			functions::clearConsole();
			TRAINING_MODE();

			break;

		case 'C':
			functions::clearConsole();
			COMPETITION_MODE();

			break;

		case 'E':
			exit (0);

			break;

		default:
			cout << "Invalid Option \n\n";
            functions::clearConsole();
            MainMenu();

			break;
    }

}

void game::Create_Game() {
	int tablesize = 0;

	cout << endl << "Insert the size of the table. Valid Range: 3-10: ";

	cin >> tablesize;
	functions::clearConsole();

	while ( tablesize < 3 ||  tablesize > 10) {
		// Seriously, people should read. And not try to mess with the program. :P
		
		cout << "An invalid size was detected. Please retry. Valid Range: 3-10: ";
		cin >> tablesize;
	}

	atable = new table(tablesize); // Creates the puzzle with the selected size.

	atable->shufflePuzzle(); // Shuffle Puzzle. This will also start our "timer".
}

void game::TRAINING_MODE() {
	Create_Game();

	cout << endl << endl;

	atable->_printTable();

	cout << endl << endl;
	
	while (!atable->puzzleIsSolved()) {
        Next_Move();
	}
	
	char option;
	bool done = false;
	functions::clearConsole();
	
	do {
		cout  << " (P)lay Again // (R)eturn to Main Menu // (E)xit Game" << endl;
		cin >> option;
		option = toupper(option);
		switch (option) { // Choose the gametypecase :
			case 'P':
				functions::clearConsole();
				TRAINING_MODE();

				done = true;

				break;

			case 'R':
				functions::clearConsole();
				MainMenu();

				done = true;

				break;

			case 'E':
				exit (0);

				done = true;

				break;

			default:
				functions::clearConsole();
				cout << " Opcao invalida \n\n";
				
				break;
		}
	} while (!done);
}

void game::COMPETITION_MODE() {
	char option;
	bool done = false;

	do {
		functions::clearConsole();
		cout  << endl << " (N)ew Game // (L)oad Table // (E)xit Competition Mode" << endl;
		cin >> option;
		option = toupper(option);
		int gameid = 0;
		int table_size = 0;

		switch (option){ // Choose the gametypecase :
			case 'N':
				functions::clearConsole();
				Create_Game();
				
				while (!atable->puzzleIsSolved()){
					Next_Move();
				}

				if (atable->puzzleIsSolved()){
					functions::clearConsole();
					string name;
					int age;
					string tempage;
					char gender;

					cout << "Name: " << endl;
					cin >> name ;
					
					while (atoi(tempage.c_str()) == 0) {
						cout << "Age: " << endl;
						cin >> tempage ;
					}

					age = atoi(tempage.c_str());

					bool finished = false;

						do { 
							cout << "Gender? (M/F)" << endl;
							cin >> gender;
							gender = toupper(gender);

							if (gender == 'M' || gender == 'F'){
								finished = true;
								break;
							} else {
								cout << " Type M or F:" << endl;
							}
						} while (!finished); // Saves the puzzle and information of the player
					atable->revertPuzzle();
				
					theIo.saveNewTable(atable, name.c_str(), age, (gender == 'M' ? kSexMale : kSexFemale), (int) atable->timeSinceStart()); // Creates a file with Puzzle, name , age, sex and timer. Also increments number of played games in the map size played.
				 }
				break;

			case 'L':
				functions::clearConsole();
				cout << endl << "Insert the size of the table. Valid Range: 3-10: ";
				cin >> table_size;
				while ( table_size < 3 ||  table_size > 10) {
					functions::clearConsole();
		// Seriously, people should read. And not try to mess with the program. :P
					cout << "An invalid size was detected. Please retry. Valid Range: 3-10: ";
					cin >> table_size;
	}
				while (true)
					{
						functions::clearConsole();
				atable = theIo.loadTable(table_size , gameid);
				atable->_printTable();
				char puzzle_option;
				cout << endl << "(C)hallenge // (N)ext Puzzle // (P)revious Puzzle // (L)eave to Main Menu" << endl;
				cin >> puzzle_option ;
				puzzle_option = toupper(puzzle_option);

					
				switch (puzzle_option)
					{
					case 'C':
						functions::clearConsole();
						LoadGame( table_size , gameid , atable);
						break;
					case 'N':
							gameid = gameid + 1;

						break;
						
					case 'P':
						gameid = gameid -1;
						if (gameid < 0){
							cout << "There are no previous puzzles available to play " << endl; 
							gameid = 0;
							functions::clearConsole();
							cout << endl;
							atable = theIo.loadTable(table_size , gameid);
							atable->_printTable();

						}
						break;
						
					case 'L':
						MainMenu();
						break;

					default:
						cout << "Invalid Option" << endl;
					}

				}
				
				done = true;
            
				break;

			case 'E':
				MainMenu();
            
				done = true;
            
				break;

			default:
				cout << "Invalid Option \n\n";
            
		}
	} while (!done);
}

void game::LoadGame(int table_size, int gameid , table *atable){
	leaderboardEntry lbe;
	atable->_printTable();
	atable->startTimer();
	while (!atable->puzzleIsSolved()){
					Next_Move();
				}

				if (atable->puzzleIsSolved()){
					functions::clearConsole();
					string name;
					int age;
					char gender;

					cout << "Name: " << endl;
					cin >> name ;
					cout << "Age: " << endl;
					cin >> age ;

					bool finished = false;

						do { 
							cout << "Gender? (M/F)" << endl;
							cin >> gender;
							gender = toupper(gender);

							if (gender == 'M' || gender == 'F'){
								finished = true;
								break;
							} else {
								cout << " Type M or F:" << endl;
							}
						} while (!finished); // Saves the puzzle and information of the player
						sprintf(lbe.name, name.c_str());
						//lbe.name = name.c_str();
						lbe.age = age;
						lbe.sex = (gender == 'M' ? kSexMale : kSexFemale);
						lbe.timeSpent = atable->timeSinceStart();
					theIo.updateTable(table_size,gameid, lbe);
				 }
		
}

void game::Next_Move() {
	functions::clearConsole();
	atable->_printTable();
	switch (_handleArrowKeyInput()) {
		case -1:
			cout << "ESC key pressed." << endl;

			cout << "The ESC key was pressed." << endl;

			break;

		case 1:
			atable->movePiece(atable->pieceAtPosition(atable->blank_space.x,atable->blank_space.y+1));

			atable->_printTable();

			break;

		case 2:
			atable->movePiece(atable->pieceAtPosition(atable->blank_space.x+1,atable->blank_space.y));

			atable->_printTable();

			break;

		case 3:
			atable->movePiece(atable->pieceAtPosition(atable->blank_space.x,atable->blank_space.y-1));

			atable->_printTable();

			break;

		case 4:
			atable->movePiece(atable->pieceAtPosition(atable->blank_space.x-1,atable->blank_space.y));

			atable->_printTable();

			break;
	}

	cout << endl;
}


int game::_handleArrowKeyInput() {
#ifdef WIN32
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD NumInputs = 0;
    DWORD InputsRead = 0;

    bool gotCorrectKey = false;
    
    INPUT_RECORD irInput;
    
    GetNumberOfConsoleInputEvents(hInput, &NumInputs);
    
    while(!gotCorrectKey) {
        ReadConsoleInput(hInput, &irInput, 1, &InputsRead);

		if (!irInput.Event.KeyEvent.bKeyDown)
			return 0;

		gotCorrectKey = true;
        
        switch (irInput.Event.KeyEvent.wVirtualKeyCode) {
            case VK_ESCAPE:
                return -1;

				break;

            case VK_LEFT:
				return 2;

				break;

			case VK_NUMPAD4:
				return 2;

				break;

			case VK_UP:
				return 1;

				break;

			case VK_NUMPAD8:
				return 1;

				break;

			case VK_RIGHT:
				return 4;

				break;

			case VK_NUMPAD6:
				return 4;

				break;

			case VK_DOWN:
				return 3;

				break;

			case VK_NUMPAD2:
				return 3;

				break;

			default:
				gotCorrectKey = false;
        }    
    }
#else
    return 0;
#endif
}

