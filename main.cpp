// --------------------------------- PRE-PROCESSING ---------------------------------
#include <iostream>
#include <cstdlib> // we need this for the rand() function
#include <chrono> // we need this for system_clock (current time in milliseconds)
#include <unistd.h> // we need this for the sleep() function
#include <string> // we need this for the getline() function

using namespace std;
using namespace std::chrono; // Helps us simplify our code for time in ms

// --------------------------------- CONFIGURATION DATA ---------------------------------
#define NO_ERRORS 0
#define LEVEL_DAMAGE_MULTIPLIER 2
#define CHARACTER_BASE_HP 100
#define BATTLE_NO_ONE_WON 0
#define BATTLE_FIRST_WON 1
#define BATTLE_SECOND_WON (-1)
#define BATTLE_MINIMUM_SLEEP_MS 100
#define BATTLE_MAXIMUM_SLEEP_MS 500
#define MICROSECOND_TO_MILLISECONDS 1000
#define MAGE_MANA_REQUIREMENT 5
#define MAGE_MIN_INTELLIGENCE 0
#define MAGE_MAX_INTELLIGENCE 100
#define MAGE_BONUS_MULTIPLIER 5
#define MAGE_DEFENSE_MULTIPLIER 0.7
#define MAGE_MIRAGE_ACTIVATION_THRESHOLD 2
#define WARRIOR_FURY_ACTIVATION_THRESHOLD 40
#define WARRIOR_FURY_CHAIN_ATTACK_AMOUNT 2
#define WARRIOR_BIND_CHAIN_ATTACK_AMOUNT 1
#define WARRIOR_LEVEL_DAMAGE_MULTIPLIER 3
#define WARRIOR_MIN_STRENGTH 0
#define WARRIOR_MAX_STRENGTH 100
#define WARRIOR_MAX_ARMOR 100
#define WARRIOR_CHAIN_WAIT_MS 300
#define ROGUE_MIN_AGILITY 0
#define ROGUE_MAX_AGILITY 100
#define ROGUE_MIN_EVASION 0
#define ROGUE_MAX_EVASION 100

// --------------------------------- FORWARD DECLARATION AND PROTOTYPING ---------------------------------
class Character;
void PrintDivider(); int Battle(Character&, Character&);
int getRand(int, int); void ShowBattleResult(const int&, const string&, const string&);


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
        health_points = CHARACTER_BASE_HP;
    }
    virtual void display_info();
    virtual void take_damage(const int&); // We will extend this for each subclass
    virtual int get_damage(const int&); // "Virtual" keyword helps us extend functions for subclasses
    string get_name();
    friend int Battle(Character&, Character&);
};
class Mage : public Character{
private:
    int intelligence, mana, mirage_counter;
public:
    Mage(const string& _name, const int& _level, const int& _intelligence, const int& _mana) :
    Character(_name, _level), intelligence(_intelligence), mana(_mana), mirage_counter(0){};
    int get_damage(const int&) override;
    void display_info() override;
    void take_damage(const int&) override;
};
class Warrior : public Character{
private:
    int strength, armor;
    bool fury, do_not_chain;
public:
    Warrior(const string& _name, const int& _level, const int& _strength, const int& _armor) :
    Character(_name, _level), strength(_strength), armor(_armor), fury(false), do_not_chain(false) {};

    int get_damage(const int&) override;
    void take_damage(const int&) override;
    void display_info() override;
};
class Rogue : public Character{
private:
    int agility, evasion, remaining_poison_damage;
public:
    Rogue(const string& _name, const int& _level, const int& _agility, const int& _evasion) :
    Character(_name, _level), agility(_agility), evasion(_evasion), remaining_poison_damage(0) {};

    int get_damage(const int&) override;
    void take_damage(const int&) override;
    void display_info() override;
};

// --------------------------------- GLOBAL VARS ---------------------------------
int PlayerWinCount = 0;
int PlayerDamageDealt = 0;
int PlayerDamageTaken = 0;
string PlayerName;
const string WarriorDescription = "Warrior (W): Focuses on combos";
const string MageDescription = "Mage (M): Specialist in spells";
const string RogueDescription = "Rogue (R): Jack of all trades";
// --------------------------------- FUNCTION DEFINITIONS ---------------------------------
int main() {
    char userChoice;
    // First we take basic info from the player
    cout << "Please enter your name: ";
    getline(cin, PlayerName);
    cin.clear(); //cin.ignore(); // clear the entire input buffer

    cout << WarriorDescription <<
    endl << MageDescription <<
    endl << RogueDescription <<
    endl << "Above are the available character types, please choose one (W, M, or R): ";
    cin >> userChoice; cin.ignore(); // ignore the \n character

    // Finally, we show a simple game over screen
    system("cls");
    PrintDivider(); PrintDivider();
    cout << "GAME OVER - " << PlayerName
    << endl << "You won " << PlayerWinCount << " games"
    << endl << "You dealt " << PlayerDamageDealt << " hit points in damage"
    << endl << "You received " << PlayerDamageTaken << " hit points in damage" << endl;
    PrintDivider(); PrintDivider();
    cout << "Press enter to exit.";
    getline(cin, PlayerName); // doesn't matter what value we put here because it will be deleted anyway,
                                    // we just want to wait for user to press enter
    return NO_ERRORS;
}

/**
 * @brief helper function to output a divider line to the screen
 */
void PrintDivider(){
    cout << "---------------------------------" << endl;
}

/**
 * @brief Simulates a battle between two characters TO THE DEATH!
 *
 * @param First One of the characters involved in the battle
 * @param Second The other character involved in the battle
 * @return >0 if first character wins, <0 if second character wins, 0 if both did same damage
 * @warning This function DEALS DAMAGE AND WILL KILL THE CHARACTER
 */
int Battle(Character& First, Character& Second){
    // Deal damages
    // this helps keep the game engaging and fight feel realistic
    usleep(getRand(BATTLE_MINIMUM_SLEEP_MS,BATTLE_MAXIMUM_SLEEP_MS)
            *MICROSECOND_TO_MILLISECONDS
            ); // Sleep from one hundred milliseconds to one second
    Second.take_damage(First.get_damage(First.level));
    // this helps keep the game engaging and fight feel realistic
    usleep(getRand(BATTLE_MINIMUM_SLEEP_MS,BATTLE_MAXIMUM_SLEEP_MS)
           *MICROSECOND_TO_MILLISECONDS
    ); // Sleep from one hundred milliseconds to one second
    First.take_damage(Second.get_damage(Second.level));

    // Check if anyone died (base case)
    if(First.health_points <= 0 && Second.health_points <= 0){ return BATTLE_NO_ONE_WON; }
    else if(First.health_points <= 0){ return BATTLE_SECOND_WON; }
    else if(Second.health_points <= 0){ return BATTLE_FIRST_WON; }

    // Continue the battle (recursive logic)
    return Battle(First, Second);
}
void ShowBattleResult(const int& Result, const string& Name1, const string& Name2){
    if(Result == BATTLE_FIRST_WON)
    {
        cout << Name1 << " won this battle!" << endl;
        return;
    }
    if(Result == BATTLE_SECOND_WON)
    {
        cout << Name2 << " won this battle!" << endl;
        return;
    }

    cout << "As with all real battles, no one won :(" << endl;
}

/**
 * @brief helper function for "true" random variables
 * @param min the lowest possible number that can be outputted by the function
 * @param max the higher possible number that can be outputted by the function
 * @return a random value
 */
int getRand(int min, int max){
    // sleep upto 20 milliseconds, helps improve randomness
    usleep((1 + rand() % (20))*1000);
    // First we get a reasonable seed, one of the methods of this is just to get the current time in ms
    // source: https://stackoverflow.com/questions/19555121/how-to-get-current-timestamp-in-milliseconds-since-1970-just-the-way-java-gets
    milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    srand(ms.count()); // ms.count() just returns the amount of milliseconds since epoch

    // Now we can use this to get a random number from min to max
    // source: https://cplusplus.com/reference/cstdlib/rand/
    return (min + (rand()%(max + 1)));
}

// --------------------------------- CLASS FUNCTION DEFINITIONS ---------------------------------

/**
 * simple getter for name
 * @return string name
 */
string Character::get_name() { return name; }

/**
 * @brief helper function for showing data about a character
 */
void Character::display_info()
{
    PrintDivider();
    cout << "Information for " << name << endl;
    cout << "Level: " << level << endl;
    cout << "HP: " << health_points << endl;
    PrintDivider();
}
void Mage::display_info() {
    PrintDivider();
    cout << "Information for " << name << ": \"Mage\"" << endl;
    cout << "Level: " << level << endl;
    cout << "HP: " << health_points << endl;
    cout << "Intelligence: " << intelligence << endl;
    cout << "Mana: " << mana << endl;
    cout << "Special Ability (MIRAGE): Places a mirage to trick opponent into hitting it instead if spells fail " << MAGE_MIRAGE_ACTIVATION_THRESHOLD << " times" << endl;
    PrintDivider();
}
void Warrior::display_info() {
    PrintDivider();
    cout << "Information for " << name << ": \"Warrior\"" << endl;
    cout << "Level: " << level << endl;
    cout << "HP: " << health_points << endl;
    cout << "Armor: " << armor << endl;
    cout << "Strength: " << strength << endl;
    cout << "Special Ability (FURY): Throws several attacks in a row if damage received in a single attack is above "
    << WARRIOR_FURY_ACTIVATION_THRESHOLD << endl;
    PrintDivider();
}
void Rogue::display_info() {
    PrintDivider();
    cout << "Information for " << name << ": \"Rogue\"" << endl;
    cout << "Level: " << level << endl;
    cout << "HP: " << health_points << endl;
    cout << "Agility: " << agility << endl;
    cout << "Evasion: " << evasion << endl;
    cout << "Special Ability (POISON): Attacks from the rogue deal half damage on the next turn" << endl;
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
void Mage::take_damage(const int& received_damage) {
    // Check if we are eligible for the mirage skill
    if(mirage_counter >= MAGE_MIRAGE_ACTIVATION_THRESHOLD){
        cout << name << ": Used MIRAGE, all incoming damage was hit on the wrong target" << endl;
        mirage_counter = 0; // reset the mirage skill
        return;
    }

    // If we don't have enough mana for defense, simply take the damage
    if(mana < MAGE_MANA_REQUIREMENT){
        health_points -= received_damage;
        return;
    }

    // If we have mana, we might not succeed in creating a spell shield
    if(intelligence <= getRand(MAGE_MIN_INTELLIGENCE, MAGE_MAX_INTELLIGENCE)){
        cout << name << ": Spell shield failed! have to take damage" << endl;
        mirage_counter++;
        health_points -= received_damage;
        return;
    }

    // We have suceeded in blocking it with spell shield
    health_points -= (int)((double)received_damage * (double)MAGE_DEFENSE_MULTIPLIER);
    mirage_counter = 0;
}
void Warrior::take_damage(const int& received_damage) {
    // If we receive too much damage, activate fury
    if(received_damage > WARRIOR_FURY_ACTIVATION_THRESHOLD){
        fury = true;
    }
    // Reduce received damage linearly with armor
    // We also add a minimum value for damage in-case armor is too effective (cannot tank more than 75% the damage)
    health_points -= max((int)((double)received_damage/(double)4), (int)((double)received_damage * ((double)armor/(double)WARRIOR_MAX_ARMOR)));
}
void Rogue::take_damage(const int& received_damage) {
    // The rogue can either dodge the hit or not
    if(evasion < getRand(ROGUE_MIN_EVASION, ROGUE_MAX_EVASION)){
        health_points -= received_damage;
        return;
    }

    cout << name << ": Dashed aside just in time! no damage taken" << endl;
}

/**
 * @brief getter function for damage
 *
 * we will have different get_damage functions for characters with things like strength or agility
 * @param outputted_damage how much the *base damage stat* is
 * @return how much the damage should be based on the base stat (usually level)
 */
int Character::get_damage(const int& outputted_damage) {
    cout << name << ": Used basic attack!" << endl;
    return outputted_damage * LEVEL_DAMAGE_MULTIPLIER;
}
int Mage::get_damage(const int& outputted_damage) {
    // If mana is less than required, do a basic attack
    if(mana < MAGE_MANA_REQUIREMENT){
        cout << name << ": Used basic attack!" << endl;
        return outputted_damage * LEVEL_DAMAGE_MULTIPLIER;
    }

    // Otherwise, attempt a spell attack and if it fails, do a basic attack
    mana -= MAGE_MANA_REQUIREMENT;
    if(intelligence < getRand(MAGE_MIN_INTELLIGENCE, MAGE_MAX_INTELLIGENCE)){
        cout << name << ": Spell failed! Using basic attack!" << endl;
        mirage_counter++; // counts up if we fail the spells for the mirage skill
        return outputted_damage * LEVEL_DAMAGE_MULTIPLIER;
    }

    cout << name << ": Spell succeeded!" << endl;
    // We have succeeded in a spell attack, deal the elevated damage
    mirage_counter = 0; // the mirage skill only activates if we fail spells thrice in a row
    return outputted_damage * LEVEL_DAMAGE_MULTIPLIER * MAGE_BONUS_MULTIPLIER * level;
}
int Warrior::get_damage(const int& outputted_damage) {
    int damage = 0;

    // If we are doing multiple attacks within a bind or fury, a basic attack is guaranteed
    if(do_not_chain){
        cout << name << ": Basic attack!" << endl;
        return outputted_damage * WARRIOR_LEVEL_DAMAGE_MULTIPLIER;
    }

    // If warrior is furious, chain attack
    if(fury){
        fury = false;
        do_not_chain = true; // We don't want to nest another chain attack with the first chain attack
        cout << name << " IS FURIOUS" << endl;
        for(int i = 0; i < WARRIOR_FURY_CHAIN_ATTACK_AMOUNT*level; i++){
            damage += get_damage(outputted_damage);
            usleep(WARRIOR_CHAIN_WAIT_MS * 1000);
        }
        do_not_chain = false;
        return damage;
    }

    // Otherwise, if the warrior didn't land a bind, do a basic attack
    if(strength < getRand(WARRIOR_MIN_STRENGTH, WARRIOR_MAX_STRENGTH)){
        cout << name << ": Failed bind! Doing a basic attack" << endl;
        return outputted_damage * WARRIOR_LEVEL_DAMAGE_MULTIPLIER;
    }

    cout << name << ": Succeeded bind!" << endl;
    do_not_chain = true; // this prevents us from binding several times in a row
    // If the warrior landed a bind, attack multiple times
    for(int i = 0; i < WARRIOR_BIND_CHAIN_ATTACK_AMOUNT*level; i++){
        damage += get_damage(outputted_damage);
        usleep(WARRIOR_CHAIN_WAIT_MS * 1000);
    }
    do_not_chain = false;
    return damage;
}
int Rogue::get_damage(const int& outputted_damage) {
    int damage = 0;
    // The rogue will apply poison on the previous attack
    damage += remaining_poison_damage;
    cout << name << ": Applied poison effect" << endl;
    remaining_poison_damage = 0;

    // After applying poison, the rogue can either hit a basic attack or place a poison dagger (double damage on next turn's poison)
    if(agility < getRand(ROGUE_MIN_AGILITY, ROGUE_MAX_AGILITY)){
        cout << name << ": Failed to attach poison dagger, doing basic attack and only half damage will be done by poison next turn" << endl;
        remaining_poison_damage = (outputted_damage*LEVEL_DAMAGE_MULTIPLIER) / 2;
        damage += outputted_damage;
    }
    else{
        cout << name << ": Poison dagger attached, next move will do double poison damage" << endl;
        remaining_poison_damage = (outputted_damage*LEVEL_DAMAGE_MULTIPLIER) * 2;
        damage += (outputted_damage*LEVEL_DAMAGE_MULTIPLIER);
    }
    return damage;
}