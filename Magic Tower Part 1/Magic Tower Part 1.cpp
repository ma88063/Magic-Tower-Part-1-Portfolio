// Magic Tower Part 1 Adam Morales
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class player {

protected:

    int strength;
    int health;
    int defense;
    int keyTotal;


    int coloumn;
    int row;

public:
    const int startStrength = 5; //Base stats
    const int startHealth = 5;
    const int startDefense = 5;
    const int startKeys = 0;



    void resetPlayer() { //Resetting player stats 
        health = startHealth;
        strength = startStrength;
        defense = startDefense;
        keyTotal = startKeys;
    }



    player(int startRow, int startColoumn) { //Constructor

        resetPlayer();

        row = startRow; //Setting starting position
        coloumn = startColoumn;
    }


    void setPosition(int newRow, int newColumn) {
        row = newRow;
        coloumn = newColumn;
    }



    //The Getters so I can read the protected variables
    int getStrength() {
        return strength;
    }


    int gainStrength(int addStrength) { //whenever potion is picked up and Strength gets boosted

        strength += addStrength;

        return strength;
    }



    int getDefense() {
        return defense;
    }
    int gainDefense(int addDefense) { //whenever potion is picked up and defense gets boosted

        defense += addDefense;

        return defense;
    }


    int getHealth() {
        return health;
    }

    int takeDamage(int damage) { //Subtracting Health

        int damageTaken;

        damageTaken = damage - defense; //Finding out how much the attack affects the player

        if (damageTaken < 0) { // Making sure damage does not give a player health
            damageTaken = 0;
        }


        health -= damageTaken; //Subtracting damage from health

        if (health < 0) { // If health falls below zero setting it to zero to make sure my is alive function works correctly
            health = 0;
        }

        return damageTaken;

    }

    int gainHealth(int addHealth) { //whenever potion is picked up and health gets boosted

        health += addHealth;

        return health;
    }



    int getKey() {
        return keyTotal;
    }

    void gainKey() {
        keyTotal++; //Adds key to inventory
    }

    bool useKey() {
        if (keyTotal > 0) {  // Check if player has any keys
            keyTotal--;       // Use one key
            return true;      // Success
        }
        return false;         // Player had no keys
    }


    int getColumn() {
        return coloumn;
    }

    int getRow() {
        return row;
    }







    bool isAlive() { //Checks if player is alive

        return (health > 0);
    }


};


class enemy {
protected:

    int strength;
    int health;
    int defense;
    int row;
    int column;

public:

    enemy() {

        strength = 2;
        health = 2;
        defense = 2;

    }


    int getDefense() { //Getting Stats for battle
        return defense;
    }
    int getStrength() {
        return strength;
    }
    int getHealth() {
        return health;
    }

    int getRow() {
        return row;
    }

    int getColumn() {
        return column;
    }


    int takeDamage(int damage) { //Subtracting Health

        int damageTaken;

        damageTaken = damage - defense; //Finding out how much the attack affects the player

        if (damageTaken < 0) { // Making sure damage does not give a player health
            damageTaken = 0;
        }


        health -= damageTaken; //Subtracting damage from health

        if (health < 0) { // If health falls below zero setting it to zero to make sure my is alive function works correctly
            health = 0;
        }

        return damageTaken;

    }

    bool isAlive() { //Checks if enemy is alive

        return (health > 0);
    }

};


class Dungeon {

private:
    vector<vector<char>> grid; //Grid of Dungeon
    player hero; //Player object inside dungeon

    int rows; //Rows and Columns of dungeon
    int columns;

public:

    Dungeon(vector<vector<char>> mapData)
        :grid(mapData), hero(findStartRow(mapData), findStartCol(mapData)) //Initializer list Placing Starting Player point
    {
        rows = grid.size();//Setting rows and columns
        columns = grid[0].size();

        // Remove '@' from grid since player position is stored separately
        grid[hero.getRow()][hero.getColumn()] = '+';
    }


    bool dungeonOver = false; //Automatically set to false once player dies or wins switches to true and goes back to main menu


    static int findStartRow(const vector<vector<char>>& mapData) {// Helper function to find the starting row 

        for (int r = 0; r < mapData.size(); r++) {  // Loop through each row

            for (int c = 0; c < mapData[r].size(); c++) {

                if (mapData[r][c] == '@')

                    return r;// Return the row index where '@' was found
            }
        }

    }

    static int findStartCol(const vector<vector<char>>& mapData) {

        for (int r = 0; r < mapData.size(); r++) {

            for (int c = 0; c < mapData[r].size(); c++) {

                if (mapData[r][c] == '@')

                    return c;
            }
        }

    }


    void display() {// display function to show the current state of the dungeon and player stats

        cout << "\n";

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {

                if (r == hero.getRow() && c == hero.getColumn())
                    cout << "@ ";
                else
                    cout << grid[r][c] << " ";
            }
            cout << endl;
        }

        cout << "\nHealth: " << hero.getHealth()
            << "  Strength: " << hero.getStrength()
            << "  Defense: " << hero.getDefense()
            << "  Keys: " << hero.getKey() << endl;
    }


    void movePlayer(string move) {// function to move the player 
        int newRow = hero.getRow(); int newCol = hero.getColumn(); 
       
        for (char& c : move) c = tolower(static_cast<unsigned char>(c));//Setting everything to lowercase to make sure input is valid no matter what 
      
        if (move == "u" || move == "up") newRow--; 
        else if (move == "d" || move == "down") newRow++; 
        else if (move == "l" || move == "left") newCol--; 
        else if (move == "r" || move == "right") newCol++; 
        
        else { cout << "\nInvalid Input Try Again!\n"; } 
        if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= columns) {//Checking bounds 
           
            cout << "You can't move there"<<endl; return ; //Returns player to move somewhere else 
        } 
        
        char tile = grid[newRow][newCol]; //New tile position
        
        tileChanger(tile, newRow, newCol); //Making tile changes 
        return; 
    }


    void tileChanger(char tile, int newRow, int newCol) {//function to process what happens when the player moves onto a new tile, if item or enemy is encountered replaced with +

        if (tile == '#') { //Wall
            cout << "You hit a wall!\n";
            return;
        }

        if (tile == '+') { //free space
            hero.setPosition(newRow, newCol);
            return;
        }

        if (tile == 'K') { //Key
            hero.gainKey();
            cout << "You picked up a key!\n";
            grid[newRow][newCol] = '+';
            hero.setPosition(newRow, newCol);
            return;
        }

        if (tile == 'D') { //Door
            if (hero.useKey()) {
                cout << "You unlocked the door!\n";
                grid[newRow][newCol] = '+';
                hero.setPosition(newRow, newCol);
            }
            else {
                cout << "The door is locked. You need a key.\n";
            }
            return;
        }

        if (tile == 'H') { //Health potion
            hero.gainHealth(5);
            cout << "You gained 5 health!" << endl;
            grid[newRow][newCol] = '+';
            hero.setPosition(newRow, newCol);
            return;
        }

        if (tile == 'S') { //Strength potion
            hero.gainStrength(5);
            cout << "You gained 5 strength!" << endl;
            grid[newRow][newCol] = '+';
            hero.setPosition(newRow, newCol);
            return;
        }

        if (tile == 'P') { //Defense potion
            hero.gainDefense(5);
            cout << "You gained 5 defense!" << endl;
            grid[newRow][newCol] = '+';
            hero.setPosition(newRow, newCol);
            return;
        }

        if (tile == 'E') { //Enemy Symbol
            cout << "An enemy appears!" << endl;
            battle(newRow, newCol);

        }

        if (tile == 'X') { //Goal of level
            cout << "You beat the Game!" << endl;
            dungeonOver = true; //Once it switches to true breaks the bool condition and takes back to main menu
            return;
        }
    }

    // battle function to handle battles
    void battle(int enemyRow, int enemyCol) {

        enemy e;

        while (hero.isAlive() && e.isAlive()) {

            e.takeDamage(hero.getStrength());

            if (e.isAlive())
                hero.takeDamage(e.getStrength());
        }

        if (!hero.isAlive()) {
            cout << "You were defeated! Game Over.\n";
            dungeonOver = true;//Once it switches to true breaks the bool condition and takes back to main menu
            return;
        }

        cout << "You defeated the enemy!\n";
        grid[enemyRow][enemyCol] = '+';
        hero.setPosition(enemyRow, enemyCol);

    }
};


class dungeonMaker { //Allows player to create their own dungeon
private:

    vector<vector<char>> grid; //2D Dungeon Grid
    int rows; //Storing rows and columns 
    int columns;

    void displayGrid() {

        cout << "\n   ";//prints number on top of grid
        for (int c = 0; c < columns; c++)
            cout << c << " ";
        cout << "\n";

        for (int r = 0; r < rows; r++) { //prints number on sides
            cout << r << "  ";
            for (int c = 0; c < columns; c++) {
                cout << grid[r][c] << " "; //prints cells
            }
            cout << "\n";
        }
    }

public:

    vector<vector<char>> createDungeon() {

        while (true) {
            cout << "Welcome to the Dungeon Maker!\nPlease enter the number of rows (1-6): "; //Gettong number of rows
            cin >> rows;

            if (cin.fail()) {               // User typed a letter or invalid input
                cin.clear();                 // Clear the error
                cin.ignore(1000, '\n');      // Ignore the invalid input
                cout << "Invalid input. Please enter a number.\n";
                continue; //continue allows to skip onto next part
            }

            if (rows <= 0 || rows >= 7) {   // Check if number is out of range
                cout << "Invalid size. Please enter a number between 1 and 6.\n";
                continue;
            }

            break; // Valid input
        }


        while (true) { //Loop with same logic just for columns
            cout << "Enter number of columns (1-6): ";
            cin >> columns;

            if (cin.fail()) { // User typed a letter or invalid input
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            if (columns <= 0 || columns >= 7) {
                cout << "Invalid size. Please enter a number between 1 and 6.\n";
                continue;
            }

            break;
        }


        grid.resize(rows); //Resizing grid
        for (int i = 0; i < rows; i++) {
            grid[i].resize(columns);
            for (int j = 0; j < columns; j++) {
                grid[i][j] = '+'; // Inputting + as an empty path
            }
        }

        int choice = 0;

        while (choice != 2) { //Will keep looping until 2 is selected

            displayGrid(); //Displaying grid

            cout << "\n1) Add Object" << endl;
            cout << "\n2) Save Dungeon" << endl;


            while (true) {// Validating menu choice
                cout << "Choose option: ";
                cin >> choice;

                if (cin.fail()) { //Making sure user does not input letter 
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input. Please enter 1 or 2.\n";
                    continue;
                }

                if (choice != 1 && choice != 2) {
                    cout << "Invalid option. Please enter 1 or 2.\n";
                    continue;
                }

                break; // Valid input
            }

            if (choice == 1) {
                addObject(); // Add object to the grid
            }
            else if (choice == 2) {
                int endCount = 0;
                int startCount = 0;// Check there is exactly one starting point @

                for (int r = 0; r < rows; r++) //goes through rows and colummns to find @

                    for (int c = 0; c < columns; c++)

                        if (grid[r][c] == '@') {
                            startCount++;
                        }


                if (startCount != 1) {
                    cout << "Dungeon must have exactly one starting point @ You have " << startCount << "\n" << endl;
                    choice = 0; // Reset choice to 0 and not 2 so loop does not break
                    continue; //This makes dungedon creator keep looping
                }
            }
        }



        return grid; // Return the completed dungeon
    }

    void addObject() { //Function Menu That Places objects in dungeon

        cout << "\nObjects:\n";
        cout << "Press 0 for Path (+)\n";
        cout << "Press 1 for Wall (#)\n";
        cout << "Press 2 for Player (@)\n";
        cout << "Press 3 for Goal (X)\n";
        cout << "Press 4 for Key (K)\n";
        cout << "Press 5 for Door (D)\n";
        cout << "Press 6 for Enemy (E)\n";
        cout << "Press 7 for Health Potion (H)\n";
        cout << "Press 8 for Strength Potion (S)\n";
        cout << "Press 9 Defense Potion (P)\n";

        int objChoice;
        cout << "Select object: ";

        while (!(cin >> objChoice) || objChoice < 0 || objChoice > 9) { //Validator to make sure no letter or out of range number gets put in
            cout << "Invalid selection. \nEnter a number between 1 and 9: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        char symbol = '+'; //Establishing the char input

        if (objChoice == 0) {//If statements for each option selected
            symbol = '+';
        }
        else if (objChoice == 1) {
            symbol = '#';
        }
        else if (objChoice == 2) {
            symbol = '@';
        }
        else if (objChoice == 3) {
            symbol = 'X';
        }
        else if (objChoice == 4) {
            symbol = 'K';
        }
        else if (objChoice == 5) {
            symbol = 'D';
        }
        else if (objChoice == 6) {
            symbol = 'E';
        }
        else if (objChoice == 7) {
            symbol = 'H';
        }
        else if (objChoice == 8) {
            symbol = 'S';
        }
        else if (objChoice == 9) {
            symbol = 'P';
        }



        int r, c;
        cout << "Enter row (Y-Coord): ";
        cin >> r; //auto breaks out if letter is selected

        cout << "Enter column (X-Coord): ";
        cin >> c;

        if (r < 0 || r >= rows || c < 0 || c >= columns) { //Validating object location
            cout << "Invalid coordinates\n"; //if location does not exist resets
            return;
        }

        grid[r][c] = symbol;//Inputs symbol into selected coordinate
    }

};

int main()
{
    vector<vector<vector<char>>> dungeonSelection; //Vector that stores all my dungeons
    vector<string> dungeonNames;//Different names of all my dungeons

    //PreMade Dungeons

    vector<vector<char>> dungeon1 = {
       {'@', '+', 'K', 'D', 'X'},
    };

    vector<vector<char>> dungeon2 = {
        {'#', '#', '#', '#', '#', '#'},
        {'#', '@', 'S', 'E', 'H', '#'},
        {'#', '+', 'E', '+', '+', '#'},
        {'#', 'K', '+', 'D', 'E', '#'},
        {'#', '+', 'S', '+', 'X', '#'},
    };

    dungeonSelection.push_back(dungeon1);
    dungeonNames.push_back("Intro");

    dungeonSelection.push_back(dungeon2);
    dungeonNames.push_back("Dungeon 1");




    while (true) { //Loop runs until user chooses an option or 3

        cout << "\n\nWelcome to Magic Tower Portfolio Part 1" << endl;
        cout << "1) Enter a dungeon" << endl;
        cout << "2) Design a dungeon" << endl;
        cout << "3) Exit" << endl;

        int choice;
        while (!(cin >> choice) || choice < 1 || choice > 3) { //Validator to make sure no letter or out of range number or char gets put in
            cout << "Invalid selection. \nEnter a number between 1 and 3: ";
            cin.clear(); //Clears input 
            cin.ignore(100, '\n'); //Clears up to 100 characters
        }


        if (choice == 1) { //Player chose to play a dungeon

            cout << "Available Dungeons (Enter Number or Name): " << endl;

            for (int i = 0; i < dungeonNames.size(); i++) { //Displaying all Dungeons and their names
                cout << i + 1 << ") " << dungeonNames[i] << endl;
            }

			cin.ignore(); // Clear the newline character 

            string selection;
            getline(cin,selection); //Player selecting dungeon they want to play

			bool isNumber = true; //Assuming input is a number until proven otherwise by checking each character
            for(char c :selection) { //Checking if selection is a number or name
				if (!isdigit(c)) { //if a character is not a digit then it is not a number and must be a name
                    isNumber = false;
                    break;
                }
			}

			int index = -1;// Will store the dungeon position, if it stays at -1 no dungeon found


            if (isNumber) {
				int num = stoi(selection); //Converting string to int if number was selected
                if (num >= 1 && num <= dungeonSelection.size()) {
                    index = num - 1; // Adjusting for 0-based index
                }
            } 

            if (index == -1) { //If not number then checking if name was selected
                for (int i = 0; i < dungeonNames.size(); i++) { // Loop through names
                    if (dungeonNames[i] == selection) {// Compare typed name to stored name
                        index = i;// Store matching index
                        break;// Stop once found
                    }
                }
			}

            if (index == -1) { //if no matches then returns to main menu
                cout << "Invalid selection. Returning to main menu.\n";
                continue;
            }

			Dungeon d(dungeonSelection[index]); //Creating dungeon object with selected dungeon

            while (!d.dungeonOver) {
                d.display(); 

                string move;
				cout << "Move: \nU or Up \n D or Down \nL or Left \nR or Right\nEnter move: ";
                cin >> move;

                d.movePlayer(move);
            }



        }

        else if (choice == 2) { //Choice to make a dungeon

            dungeonMaker maker;// Create a dungeonMaker object to handle dungeon creation.

            vector<vector<char>> newDungeon = maker.createDungeon();

            string name;
            cout << "Enter name for dungeon: ";
            cin >> name; //Player naming dungeon

            dungeonSelection.push_back(newDungeon);//Using Pushback to make space in array for new dungeon name and the actual dungeon itself
            dungeonNames.push_back(name);
        }


        else if (choice == 3) { //exiting program
            return 0;
        }
    }
}