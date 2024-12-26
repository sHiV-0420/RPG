#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

class Item {
public:
    string name;
    int value;

    Item(string name, int value) : name(name), value(value) {}

    void display() {
        cout << name << " (Value: " << value << ")\n";
    }
};

class Weapon : public Item {
public:
    int attackPower;

    Weapon(string name, int value, int attackPower) : Item(name, value), attackPower(attackPower) {}

    void display() {
        cout << name << " (Value: " << value << ", Attack Power: " << attackPower << ")\n";
    }
};

class Character {
protected:
    string name;
    int health;
    int maxHealth;
    int attackPower;
    vector<Item> inventory;

public:
    Character(string name, int health, int attackPower) : name(name), health(health), maxHealth(health), attackPower(attackPower) {}

    string getName() { return name; }
    int getHealth() { return health; }
    int getAttackPower() { return attackPower; }

    void heal(int amount) {
        health = min(maxHealth, health + amount);
    }

    void takeDamage(int damage) {
        health = max(0, health - damage);
    }

    void addItem(Item item) {
        inventory.push_back(item);
    }

    void showInventory() {
        cout << "Inventory:\n";
        for (auto& item : inventory) {
            item.display();
        }
    }

    void showStats() {
        cout << name << " Stats:\n";
        cout << "Health: " << health << "/" << maxHealth << "\n";
        cout << "Attack Power: " << attackPower << "\n";
    }
};

class Enemy {
private:
    string name;
    int health;
    int attackPower;

public:
    Enemy(string name, int health, int attackPower) : name(name), health(health), attackPower(attackPower) {}

    string getName() { return name; }
    int getHealth() { return health; }
    int getAttackPower() { return attackPower; }

    void takeDamage(int damage) {
        health = max(0, health - damage);
    }

    bool isAlive() {
        return health > 0;
    }

    void showStats() {
        cout << name << " Stats:\n";
        cout << "Health: " << health << "\n";
        cout << "Attack Power: " << attackPower << "\n";
    }
};

class Game {
private:
    Character* player;
    vector<Enemy> enemies;
    bool isRunning;

public:
    Game() : player(nullptr), isRunning(true) {}

    void start() {
        cout << "Welcome to the RPG Game!\n";
        string playerName;
        cout << "Enter your character's name: ";
        cin >> playerName;

        player = new Character(playerName, 100, 20);  // Starting health 100, attack power 20

        // Sample enemies
        enemies.push_back(Enemy("Goblin", 50, 10));
        enemies.push_back(Enemy("Troll", 120, 15));

        gameLoop();
    }

    void gameLoop() {
        while (isRunning) {
            cout << "\nWhat would you like to do?\n";
            cout << "1. View Stats\n";
            cout << "2. View Inventory\n";
            cout << "3. Fight Enemy\n";
            cout << "4. Heal\n";
            cout << "5. Save Game\n";
            cout << "6. Exit Game\n";

            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    player->showStats();
                    break;
                case 2:
                    player->showInventory();
                    break;
                case 3:
                    battle();
                    break;
                case 4:
                    healPlayer();
                    break;
                case 5:
                    saveGame();
                    break;
                case 6:
                    isRunning = false;
                    break;
                default:
                    cout << "Invalid choice, try again.\n";
                    break;
            }
        }

        cout << "Game Over!\n";
    }

    void battle() {
        if (enemies.empty()) {
            cout << "No enemies left to fight.\n";
            return;
        }

        cout << "Select an enemy to fight:\n";
        for (int i = 0; i < enemies.size(); ++i) {
            cout << i + 1 << ". " << enemies[i].getName() << "\n";
        }

        int choice;
        cin >> choice;

        if (choice < 1 || choice > enemies.size()) {
            cout << "Invalid choice!\n";
            return;
        }

        Enemy& enemy = enemies[choice - 1];
        cout << "You are fighting " << enemy.getName() << "!\n";

        while (player->getHealth() > 0 && enemy.getHealth() > 0) {
            cout << "Your turn:\n";
            enemy.takeDamage(player->getAttackPower());
            cout << "You attacked " << enemy.getName() << " for " << player->getAttackPower() << " damage!\n";

            if (enemy.getHealth() <= 0) {
                cout << enemy.getName() << " has been defeated!\n";
                enemies.erase(enemies.begin() + (choice - 1));  // Remove defeated enemy
                break;
            }

            cout << enemy.getName() << "'s turn:\n";
            player->takeDamage(enemy.getAttackPower());
            cout << enemy.getName() << " attacked you for " << enemy.getAttackPower() << " damage!\n";

            if (player->getHealth() <= 0) {
                cout << "You have been defeated.\n";
                break;
            }
        }
    }

    void healPlayer() {
        cout << "Healing...\n";
        player->heal(30);  // Heal player by 30 points
    }

    void saveGame() {
        ofstream outFile("savegame.txt");
        outFile << player->getName() << endl;
        outFile << player->getHealth() << endl;
        outFile << player->getAttackPower() << endl;
        outFile.close();
        cout << "Game saved.\n";
    }

    void loadGame() {
        ifstream inFile("savegame.txt");
        if (!inFile) {
            cout << "No saved game found.\n";
            return;
        }
        string playerName;
        int health, attackPower;
        inFile >> playerName >> health >> attackPower;
        player = new Character(playerName, health, attackPower);
        inFile.close();
        cout << "Game loaded.\n";
    }
};

int main() {
    srand(time(0));  // Initialize random seed
    Game game;
    game.start();
    return 0;
}
