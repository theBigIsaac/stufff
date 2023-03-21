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

double randomNum(double min, double max) {
    std::default_random_engine ranEng(time(0));
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

struct Weapon {
    string name;
    double damage = 0;
    int cost = 0;
};

list<Weapon> weapons;

Weapon GetWeapon(list<Weapon> weapons1, int x) {
    list<Weapon>::iterator y = weapons1.begin();
    for (int i = 0; i < x; i++) {
        ++y;
    }
    return *y;
}

struct Armour {
    string name;
    double defence = 0;
    int cost = 0;
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

double calculateDifficulty(int score, Player mc) {
    double difficulty = score * 0.1;
    difficulty += mc.weapon.damage * 0.1;
    difficulty += mc.armour.defence * 0.1;
    difficulty = randomNum(difficulty * 0.75, difficulty * 1.5);
    return difficulty;
}

Enemy createEnemy(double dificil) {
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
    ifstream fileReader("weapons.txt");
    string split = "\t";

    while (getline(fileReader, line)) {
        size_t position = 0;
        string token;
        while ((position = line.find(split)) != string::npos) {
            token = line.substr(0, position);
            temp.name = token;
            line.erase(0, position + split.length());
            position = line.find(split);
            temp.damage = stod(line.substr(0, position));
            line.erase(0, position + split.length());
        }
        temp.cost = stod(line);
        weapons.push_back(temp);
    }
}

void createArmours() {
    Armour temp;
    string line;
    ifstream fileReader("armours.txt");
    string split = "\t";

    while (getline(fileReader, line)) {
        size_t position = 0;
        string token;
        while ((position = line.find(split)) != string::npos) {
            token = line.substr(0, position);
            temp.name = token;
            line.erase(0, position + split.length());
            position = line.find(split);
            temp.defence = stod(line.substr(0, position));
            line.erase(0, position + split.length());
        }
        temp.cost = stod(line);
        armours.push_back(temp);
    }
}

int healthCheck(Player mc, Enemy enemy) {
    if (mc.currentHP <= 0) {
        return 0;
    }
    else if (enemy.health <= 0) {
        return 1;
    }
    else { return 2; }
}

void ShopContents(int score) {

}

Player Shop(Player mc, int score) {
    system("cls");
    cout << "Current Weapon: " << mc.weapon.name << "\t Damage: " << mc.weapon.damage;
    cout << "\nCurrent Armour: " << mc.armour.name << "\t Defence: " << mc.armour.defence;
    cout << "\n\nMoney: " << mc.money;
    sleep(2);
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
        sleep(1);
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
        sleep(2);
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

void game(Player mc) {
    system("cls");
    int score = 0;
    while (mc.currentHP > 0) {
        double difficulty = calculateDifficulty(score, mc);
        Enemy mainEnemy = createEnemy(difficulty);
        mc = Battle(mc, mainEnemy);
        score++;
        if (score % 2 == 0) {
            Shop(mc, score);
        }
    }
}

int main()
{
    srand((unsigned)time(NULL));
    createWeapons();
    createArmours();

    Player mainCharacter;
    mainCharacter.maxHP = 100;
    mainCharacter.weapon = GetWeapon(weapons, 0);
    mainCharacter.armour = GetArmour(armours, 0);

    int choice;
    cout << "Main Menu\n1. Play the Game\n";
    cin >> choice;

    if (choice == 1) {
        game(mainCharacter);
    }

    return 0;
}


