#include <iostream>
#include <string>
#include <vector>
#include<cstdio>
#include<iostream>
#include<ctime>
#include <chrono>
#include <thread>
#include<cstdlib>
#include<fstream>
#include <algorithm>
#include <list>
#include <random>
#include <conio.h>

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

int inpValue() {
    int x;
    cout << "Type here: ";
    cin >> x;
    return x;
}

void sleep(int delay) {
    delay *= CLOCKS_PER_SEC;
    clock_t now = clock();
    while (clock() - now < delay);
}

double randomNum(double min, double max, default_random_engine ranEng) {
    std::uniform_real_distribution<double> num(min, max);
    return num(ranEng);
}

bool mathsQuestion(string sign) {
    bool success;
    int delay = 10;
    int answer;
    delay *= CLOCKS_PER_SEC;
    int ranNum1 = (rand() % 11);
    int ranNum2 = (rand() % 11);
    clock_t now = clock();
    if (sign == "+") {
        answer = ranNum1 + ranNum2;
    }
    else if (sign == "-") {
        answer = ranNum1 - ranNum2;
    }
    else {
        answer = ranNum1 * ranNum2;
    }
    cout << "What is " << ranNum1 << " " << sign << " " << ranNum2 << "\n";
    int y = inpValue();
    if (y == answer && clock() - now <= delay) {
        cout << "Correct\n";
        success = true;
    }
    else if (y != answer) {
        cout << "Wrong\n";
        success = false;
    }
    else {
        cout << "Too Slow\n";
        success = false;
    }
    return success;
}

struct Item {
    int index;
    string name;
    int cost;
};

struct Weapon : Item {
    double damage = 0;
};

list<Weapon> weapons;

Weapon GetWeapon(list<Weapon> weapons1, int x) {
    list<Weapon>::iterator y = weapons1.begin();
    for (int i = 0; i < x; i++) {
        ++y;
    }
    return *y;
}

struct Armour : Item {
    double defence = 0;
};

list<Armour> armours;

Armour GetArmour(list<Armour> armours1, int x) {
    list<Armour>::iterator y = armours1.begin();
    for (int i = 0; i < x; i++) {
        ++y;
    }
    return *y;
}

struct Player {
    string name;
    double maxHP = 100;
    double currentHP = 100;
    Weapon weapon;
    Armour armour;
    int money = 0;
};

struct Enemy {
    string name;
    double difficulty = 0.0;
    double health = 40;
};

double calculateDifficulty(int score, Player mc, default_random_engine ran) {
    double difficulty = score * 0.1;
    difficulty += mc.weapon.damage * 0.1;
    difficulty += mc.armour.defence * 0.1;
    difficulty = randomNum(difficulty * 0.75, difficulty * 1.5, ran);
    return difficulty;
}

Enemy createEnemy(double dificil, default_random_engine ran) {
    vector<string> allNames;
    vector<string> allTitles;
    string line;
    string fullName;
    int random;

    ifstream fileReader("names.txt");

    int fileLength = 0;
    while (getline(fileReader, line)) {
        fileLength++;
        allNames.push_back(line);
    }
    random = rand() % fileLength;
    fullName = allNames[random];

    if (rand() % 2 == 0) {
        ifstream fileReader("titles1.txt");
        fileLength = 0;
        while (getline(fileReader, line)) {
            fileLength++;
            allTitles.push_back(line);
        }
        random = rand() % fileLength;
        fullName = allTitles[random] + fullName;
    }
    else {
        ifstream fileReader("titles2.txt");
        fileLength = 0;
        while (getline(fileReader, line)) {
            fileLength++;
            allTitles.push_back(line);
        }
        random = rand() % fileLength;
        fullName = fullName + allTitles[random];
    }

    Enemy newEnemy;
    newEnemy.name = fullName;
    newEnemy.difficulty = dificil;
    newEnemy.health = 40.0 * dificil;

    fileReader.close();

    return newEnemy;
}

void createWeapons() {
    Weapon temp;
    string line;
    int lineNum = 0;
    ifstream fileReader("weapons.txt");
    string split = "\t";

    while (getline(fileReader, line)) {
        size_t position = 0;
        string token;
        while ((position = line.find(split)) != string::npos) {
            token = line.substr(0, position);
            temp.index = lineNum;
            temp.name = token;
            line.erase(0, position + split.length());
            position = line.find(split);
            temp.damage = stod(line.substr(0, position));
            line.erase(0, position + split.length());
        }
        temp.cost = stod(line);
        weapons.push_back(temp);
        lineNum++;
    }
}

void createArmours() {
    Armour temp;
    string line;
    int lineNum = 0;
    ifstream fileReader("armours.txt");
    string split = "\t";

    while (getline(fileReader, line)) {
        size_t position = 0;
        string token;
        while ((position = line.find(split)) != string::npos) {
            token = line.substr(0, position);
            temp.index = lineNum;
            temp.name = token;
            line.erase(0, position + split.length());
            position = line.find(split);
            temp.defence = stod(line.substr(0, position));
            line.erase(0, position + split.length());
        }
        temp.cost = stod(line);
        armours.push_back(temp);
        lineNum++;
    }
}

int healthCheck(Player mc, Enemy enemy) {
    if (mc.currentHP < 1) {
        return 0;
    }
    else if (enemy.health < 1) {
        return 1;
    }
    else { return 2; }
}

list<Weapon> WeaponShopContents(int score, default_random_engine ran) {
    list<Weapon> contents;
    int amount = randomNum(2, 5, ran);
    int location = score - 5;
    for (int x = 0; x < amount; x++) {
        if (ran() % 100 < 70 && location < weapons.size()) {
            contents.push_back(GetWeapon(weapons, location));
        }
        location++;
    }
    return contents;
}

list<Armour> ArmourShopContents(int score, default_random_engine ran) {
    list<Armour> contents;
    int amount = randomNum(1, 3, ran);
    int location = score - 5;
    for (int x = 0; x < amount; x++) {
        if (ran() % 100 < 70 && location < armours.size()) {
            contents.push_back(GetArmour(armours, location));
        }
        location++;
    }
    return contents;
}

void ShopContents(Player mc, list<Weapon> displayWeapons, list<Armour> displayArmours) {
    cout << "Current Weapon: " << mc.weapon.name << "\t Damage: " << mc.weapon.damage;
    cout << "\nCurrent Armour: " << mc.armour.name << "\t Defence: " << mc.armour.defence;
    cout << "\n\nMoney: " << mc.money << "\n\n";
    for (int i = 0; i < displayWeapons.size(); i++) {
        Weapon temp = GetWeapon(displayWeapons, i);
        cout << i + 1 << ": " << temp.name << "\t Damage: " << temp.damage << "\t Cost: " << temp.cost << "\n";
    }
    for (int i = displayWeapons.size(); i < displayArmours.size() + displayWeapons.size(); i++) {
        Armour temp = GetArmour(displayArmours, i - displayWeapons.size());
        cout << i + 1 << ": " << temp.name << "\t Damage: " << temp.defence << "\t Cost: " << temp.cost << "\n";
    }
    cout << "\nTo buy an item, type the corresponding number, or 0 to leave the shop.\n";
}

Player Shop(Player mc, int score, default_random_engine ran) {
    system("cls");
    list<Weapon> weaponStuff = WeaponShopContents(score, ran);
    list<Armour> armourStuff = ArmourShopContents(score, ran);
    ShopContents(mc, weaponStuff, armourStuff);
    int choice = 1;
    string yn;
    while (choice != 0) {
        cin >> choice;
        if (choice == 0) {
            cout << "saving...";
            ofstream saveFile("save.txt");
            saveFile << mc.name << "\n" << score << "\n" << mc.money << "\n" << mc.weapon.index << "\n" << mc.armour.index;
            saveFile.close();
        }
        else if (choice <= weaponStuff.size()) {
            Weapon temp = GetWeapon(weaponStuff, choice - 1);
            cout << "Do you want to purchase the " << temp.name << "? (y/n)\n";
            cin >> yn;
            if (yn == "y" && mc.money >= temp.cost) {
                cout << "SOLD";
                mc.weapon = GetWeapon(weapons, temp.index);
                mc.money -= temp.cost;
            }
            else if (yn == "y" && mc.money < temp.cost) {
                cout << "You can't afford this";
            }
            else if (yn == "n") {
                cout << "Transaction cancelled";
            }
            else {
                cout << "Input not understood, cancelling transaction";
            }
        }
        else if (choice <= weaponStuff.size() + armourStuff.size()) {
            Armour temp = GetArmour(armourStuff, choice - weaponStuff.size() - 1);
            cout << "Do you want to purchase the " << temp.name << "? (y/n)\n";
            cin >> yn;
            if (yn == "y" && mc.money >= temp.cost) {
                cout << "SOLD";
                mc.armour = GetArmour(armours, temp.index);
                mc.money -= temp.cost;
            }
            else if (yn == "y" && mc.money < temp.cost) {
                cout << "You can't afford this";
            }
            else if (yn == "n") {
                cout << "Transaction cancelled";
            }
            else {
                cout << "Input not understood, cancelling transaction";
            }

        }
        else {
            cout << "Invalid number";
        }
        sleep(3);
        system("cls");
        ShopContents(mc, weaponStuff, armourStuff);
    }
    return mc;
}

Player Battle(Player mc, Enemy enemy) {
    mc.currentHP = mc.maxHP;
    string choice = "";
    int winner = 2;
    while (winner == 2) {
        system("cls");
        cout << "You are facing " << enemy.name << "\n\n";
        cout << "Player Health: " << (int)mc.currentHP << "\n";
        cout << enemy.name << " Health: " << (int)enemy.health << "\n\n";
        sleep(0.5);
        bool noAttack = true;
        while (noAttack) {
            cout << "What type of attack: \nLight   Heavy   Special\n";
            string choice;
            cin >> choice;
            transform(choice.begin(), choice.end(), choice.begin(), ::tolower);
            if (choice == "light") {
                if (mathsQuestion("+")) { enemy.health -= mc.weapon.damage * 1.25; }
                else { mc.currentHP -= (enemy.difficulty * 10.0 - mc.armour.defence); }
                noAttack = false;
            }
            else if (choice == "heavy") {
                if (mathsQuestion("-")) { enemy.health -= (mc.weapon.damage * 1.75); }
                else { mc.currentHP -= (enemy.difficulty * 15.0 - mc.armour.defence); }
                noAttack = false;
            }
            else if (choice == "special") {
                if (mathsQuestion("*")) { enemy.health -= (mc.weapon.damage * 2.25); }
                else { mc.currentHP -= (enemy.difficulty * 20.0 - mc.armour.defence); }
                noAttack = false;
            }
        }
        sleep(1);
        winner = healthCheck(mc, enemy);
        switch (winner) {
        case 0:
            cout << "You died";
            sleep(2);
            break;
        case 1:
            int reward = enemy.difficulty * 10;
            cout << "You survived and earned " << reward << " coins";
            mc.money += reward;
            sleep(2);
            break;
        }
        cout << "\n";
    }
    return mc;
}

int GetScore(list<int> scores, int x) {
    list<int>::iterator y = scores.begin();
    for (int i = 0; i < x; i++) {
        ++y;
    }
    return *y;
}

void Dead(Player mc, int score) {
    ifstream readFile("score.txt");
    list<int> scores;
    list<string> names;
    string line;
    int lineNum = 0;
    string split = "\t";

    while (getline(readFile, line)) {
        size_t position = 0;
        string token;
        while ((position = line.find(split)) != string::npos) {
            token = line.substr(0, position);
            scores.push_back(stod(token));
            line.erase(0, position + split.length());
        }
        names.push_back(line);
        lineNum++;
    }

    bool replaced = false;
    list<int>::iterator it1 = scores.begin();
    list<string>::iterator it2 = names.begin();
    for (int x = 0; x < scores.size(); x++)
    {
        if (score > GetScore(scores, x) && !replaced) {
            scores.insert(it1, score);
            names.insert(it2, mc.name);
            replaced = true;
        }
        if (x < scores.size() - 1) {
            advance(it1, 1); advance(it2, 1);
        }
    }

    if (replaced) {
        ofstream updateFile("score.txt");
        for (int y = 0; y < 10; y++) {
            updateFile << scores.front() << "\t" << names.front() << "\n";
            scores.pop_front(); names.pop_front();
        }
        updateFile.close();
    }
}

void leaderBoard() {
    ifstream readFile("score.txt");
    list<int> scores;
    list<string> names;
    string line;
    int lineNum = 0;
    string split = "\t";

    while (getline(readFile, line)) {
        size_t position = 0;
        string token;
        while ((position = line.find(split)) != string::npos) {
            token = line.substr(0, position);
            scores.push_back(stod(token));
            line.erase(0, position + split.length());
        }
        names.push_back(line);
        lineNum++;
    }

    for (int x = 0; x < 10; x++)
    {
        cout << x + 1 << ". " << names.front() << "\t" << scores.front() << "\n";
        names.pop_front(); scores.pop_front();
    }
    cout << "\n";
    system("pause");
}

void tutorial() {
    system("cls");
    cout << "The goal of this game is to defeat as many enemies as possible and reach a new highscore\n";
    cout << "You will automatically go from battle to battle\n\n";
    cout << "To fight your enemy, you must choose either a Light, Heavy or Special attack, you will type the name of the attack you wish to perform\n";
    cout << "In order to perform your attack, you must solve a simple maths equation, which will be based on the type of attack you perform\n";
    cout << "Light - Addition - Low damage\tHeavy - Subtraction - Medium damage\tSpecial - Multiplication - High damage\n";
    cout << "If you get the answer wrong, or take too long to answer, you will take damage corresponding to the type of attack you chose\n";
    cout << "If you get the answer right, in the right amount of time, the enemy will take damage corresponding to the type of attack you chose\n";
    cout << "You will continue this process until either your health, or the enemy's health is 0\n\n";
    cout << "Every 5 enemies you defeat, you are transported to a shop where you can buy armour or weapons with the gold you earn from defeating enemies\n";
    cout << "Armour reduces the damage you take from enemies, weapons increase the damage you deal to enemies\n";
    cout << "The game will also save when you leave the shop, so you can close it and return at a later time\n\n";
    cout << "Reminder: When you die, your character is permanently DEAD, and you will have to start again at level 1, with the starting armour and weapon\n";
    cout << "However, your score will be saved and added to the leaderboard\n\n";
    system("pause");
}

void game(default_random_engine ran, Player mainCharacter, int score) {
    system("cls");
    while (mainCharacter.currentHP > 0) {
        double difficulty = calculateDifficulty(score, mainCharacter, ran);
        Enemy mainEnemy = createEnemy(difficulty, ran);
        mainCharacter = Battle(mainCharacter, mainEnemy);
        if (mainCharacter.currentHP > 0) {
            score++;
            if (score % 5 == 0) {
                mainCharacter = Shop(mainCharacter, score, ran);
            }
        }
    }
    Dead(mainCharacter, score);
}

void newGame(default_random_engine ran) {
    Player mainCharacter;
    cout << "What is your explorer's name?\n";
    cin >> mainCharacter.name;
    mainCharacter.maxHP = 100;
    mainCharacter.weapon = GetWeapon(weapons, 0);
    mainCharacter.armour = GetArmour(armours, 0);
    game(ran, mainCharacter, 0);
}

void loadGame(default_random_engine ran) {
    ifstream saveFile("save.txt");
    string line;
    Player mainCharacter;
    mainCharacter.maxHP = 100;
    int score;
    int x = 0;
    while (getline(saveFile, line)) {
        switch (x) {
        case 0:
            mainCharacter.name = line;
            break;
        case 1:
            score = stod(line);
            break;
        case 2:
            mainCharacter.money = stod(line);
            break;
        case 3:
            mainCharacter.weapon = GetWeapon(weapons, stod(line));
            break;
        case 4:
            mainCharacter.armour = GetArmour(armours, stod(line));
            break;
        }
        x++;
    }
    game(ran, mainCharacter, score);
}

void main()
{
    srand((unsigned)time(NULL));
    std::default_random_engine ranEng(time(0));
    createWeapons();
    createArmours();

    int choice = 0;
    while (choice != 5) {
        system("cls");
        cout << "Main Menu\n1. New Game\n2. Load Game\n3. View Leaderboard\n4. Read Tutorial\n5. Quit Game\n";
        cin >> choice;

        if (choice == 1) {
            newGame(ranEng);
        }
        else if (choice == 2) {
            loadGame(ranEng);
        }
        else if (choice == 3) {
            leaderBoard();
        }
        else if (choice == 4) {
            tutorial();
        }
        else if (choice == 5) {
            cout << "Quitting";
            sleep(1);
        }
        else {
            cout << "That option is not listed, please try again";
            sleep(2);
        }
    }
    
}
