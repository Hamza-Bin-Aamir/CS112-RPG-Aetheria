// --------------------------------- PRE-PROCESSING ---------------------------------
#include <iostream>
#define NO_ERRORS 0
#define LEVEL_DAMAGE_MULTIPLIER 10
#define BATTLE_NO_ONE_WON 0
#define BATTLE_FIRST_WON 1
#define BATTLE_SECOND_WON 2
using namespace std;

// --------------------------------- FORWARD DECLARATION AND PROTOTYPING ---------------------------------
class Character;
void PrintDivider(); int Battle(Character&, Character&); int BattleCheck(const Character&, const Character&);


// --------------------------------- CLASSES ---------------------------------
class Character{
// Private data, can be accessed by children too
protected:
    string name;
    int level;
    int health_points;
//
public:
    Character(const string& name, const int& level)
    {
        this->name = name;
        this->level = level;
        health_points = 100;
    }
    void display_info();
    void take_damage(const int&); // We will extend this for each subclass
    int get_damage(const int&);
    friend int Battle(Character&, Character&);
};

// --------------------------------- FUNCTION DEFINITIONS ---------------------------------
int main() {
    Character P1("Hamza", 2);
    P1.display_info();
    return NO_ERRORS;
}

void PrintDivider(){
    cout << "---------------------------------" << endl;
}

/**
 * @brief Simulates a battle between two characters TO THE DEATH!
 *
 * @param First One of the characters involved in the battle
 * @param Second The other character involved in the battle
 * @return >0 if first character wins, <0 if second character wins, 0 if both did same damage
 */
int Battle(Character& First, Character& Second){
    // Deal damages
    First.take_damage(Second.get_damage(Second.level));
    Second.take_damage(First.get_damage(First.level));

    // Check if anyone died (base case)
    if(First.health_points <= 0 && Second.health_points <= 0){ return BATTLE_NO_ONE_WON; }
    else if(First.health_points <= 0){ return BATTLE_SECOND_WON; }
    else if(Second.health_points <= 0){ return BATTLE_FIRST_WON; }

    // Continue the battle (recursive logic)
    return Battle(First, Second);
}

// --------------------------------- CLASS FUNCTION DEFINITIONS ---------------------------------

/**
 * @brief helper function for showing data about a character
 *
 * displays all the stats: for this type of character, these are: \n
 * 1. Name \n
 * 2. Level \n
 * 3. Health_Points
 */
void Character::display_info()
{
    PrintDivider();
    cout << "Information for " << name << endl;
    cout << "Level: " << level << endl;
    cout << "HP: " << health_points << endl;
    PrintDivider();
}

/**
 * @brief setter function for health_points
 *
 * we will have different take_damage functions for characters with things like armor or evasion
 *
 * @param received_damage the raw value for received damage
 */
void Character::take_damage(const int& received_damage) {
    health_points -= received_damage;
}

/**
 * @brief getter function for damage
 *
 * we will have different get_damage functions for characters with things like strength or agility
 */
int Character::get_damage(const int& outputted_damage) {
    return outputted_damage * LEVEL_DAMAGE_MULTIPLIER;
}