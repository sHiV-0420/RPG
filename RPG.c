#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

enum Rarity { COMMON, UNCOMMON, RARE, LEGENDARY };
enum Skill { NONE, FIREBALL, HEALING_TOUCH, STRENGTH_BOOST };

class Item {
public:
    string name;
    int value;
    Rarity rarity;

    Item(string name, int value, Rarity rarity) : name(name), value(value), rarity(rarity) {}

    virtual void use() {
        cout << "Using " << name << "\n";
    }

    virtual void display() {
        string rarityStr;
        switch (rarity) {
            case COMMON: rarityStr = "Common"; break;
            case UNCOMMON: rarityStr = "Uncommon"; break;
            case RARE: rarityStr = "Rare"; break;
            case LEGENDARY: rarityStr = "Legendary"; break;
        }
        cout << name << " (Value: " << value << ", Rarity: " << rarityStr << ")\n";
    }
};

class Weapon : public Item {
public:
    int attackPower;

    Weapon(string name, int value, Rarity rarity, int attackPower)
        : Item(name, value, rarity), attackPower(attackPower) {}

    void use() override {
        cout << "Equipping weapon: " << name << " (Attack Power: " << attackPower << ")\n";
    }

    void display() override {
        Item::display();
        cout << "Attack Power: " << attackPower << "\n";
    }
};

class Armor : public Item {
public:
    int defensePower;

    Armor(string name, int value, Rarity rarity, int defensePower)
        : Item(name, value, rarity), defensePower(defensePower) {}

    void use() override {
        cout << "Equipping armor: " << name << " (Defense Power: " << defensePower << ")\n";
    }

    void display() override {
        Item::display();
        cout << "Defense Power: " << defensePower << "\n";
    }
};

class Potion : public Item {
public:
    int healthRestore;

    Potion(string name, int value, Rarity rarity, int healthRestore)
        : Item(name, value, rarity), healthRestore(healthRestore) {}

    void use() override {
        cout << "Using potion: " << name << " to restore " << healthRestore << " health.\n";
    }

    void display() override {
        Item::display();
        cout << "Restores Health: " << healthRestore << "\n";
    }
};

class Scroll : public Item {
public:
    Skill skill;

    Scroll(string name, int value, Rarity rarity, Skill skill)
        : Item(name, value, rarity), skill(skill) {}

    void use() override {
        switch (skill) {
            case FIREBALL:
                cout << "Casting Fireball!\n";
                break;
            case HEALING_TOUCH:
                cout << "Casting Healing Touch!\n";
                break;
            case STRENGTH_BOOST:
                cout << "Using Strength Boost!\n";
                break;
            default:
                cout << "Unknown scroll effect.\n";
        }
    }

    void display() override {
        Item::display();
        string skillStr;
        switch (skill) {
            case FIREBALL: skillStr = "Fireball"; break;
            case HEALING_TOUCH: skillStr = "Healing Touch"; break;
            case STRENGTH_BOOST: skillStr = "Strength Boost"; break;
            default: skillStr = "Unknown"; break;
        }
        cout << "Skill: " << skillStr << "\n";
    }
};

class Trap : public Item {
public:
    int damage;

    Trap(string name, int value, Rarity rarity, int damage)
        : Item(name, value, rarity), damage(damage) {}

    void use() override {
        cout << "Setting trap: " << name << " (Damage: " << damage << ")\n";
    }

    void display() override {
        Item::display();
        cout << "Damage: " << damage << "\n";
    }
};

class Artifact : public Item {
public:
    string effect;

    Artifact(string name, int value, Rarity rarity, string effect)
        : Item(name, value, rarity), effect(effect) {}

    void use() override {
        cout << "Using artifact: " << name << " (" << effect << ")\n";
    }

    void display() override {
        Item::display();
        cout << "Effect: " << effect << "\n";
    }
};

class Character {
protected:
    string name;
    int health;
    int maxHealth;
    int attackPower;
    int defensePower;
    int level;
    int experience;
    vector<Item*> inventory;
    vector<Item*> equippedItems;
    Item* equippedWeapon;
    Item* equippedArmor;
    Item* equippedPotion;
    Skill currentSkill;

public:
    Character(string name, int health, int attackPower)
        : name(name), health(health), maxHealth(health), attackPower(attackPower), defensePower(0), level(1), experience(0), currentSkill(NONE), equippedWeapon(nullptr), equippedArmor(nullptr), equippedPotion(nullptr) {}

    string getName() { return name; }
    int getHealth() { return health; }
    int getAttackPower() { return attackPower + (equippedWeapon ? static_cast<Weapon*>(equippedWeapon)->attackPower : 0); }
    int getDefensePower() { return defensePower + (equippedArmor ? static_cast<Armor*>(equippedArmor)->defensePower : 0); }
    int getLevel() { return level; }
    int getExperience() { return experience; }

    void heal(int amount) {
        health = min(maxHealth, health + amount);
    }

    void takeDamage(int damage) {
        int damageTaken = max(0, damage - getDefensePower());
        health = max(0, health - damageTaken);
    }

    void addItem(Item* item) {
        inventory.push_back(item);
    }

    void equipWeapon(Weapon* weapon) {
        equippedWeapon = weapon;
        attackPower += weapon->attackPower;
    }

    void equipArmor(Armor* armor) {
        equippedArmor = armor;
        defensePower += armor->defensePower;
    }

    void equipPotion(Potion* potion) {
        equippedPotion = potion;
    }

    void equipScroll(Scroll* scroll) {
        currentSkill = scroll->skill;
    }

    void showInventory() {
        cout << "Inventory:\n";
        for (size_t i = 0; i < inventory.size(); ++i) {
            cout << i + 1 << ". ";
            inventory[i]->display();
        }
    }

    void showInventoryPage(int page) {
        const int itemsPerPage = 5;
        int start = page * itemsPerPage;
        int end = min(start + itemsPerPage, (int)inventory.size());

        cout << "Page " << page + 1 << ":\n";
        for (int i = start; i < end; ++i) {
            cout << i + 1 << ". ";
            inventory[i]->display();
        }
        cout << "Page " << page + 1 << " of " << (inventory.size() / itemsPerPage) + 1 << endl;
    }

    void useItem(int index) {
        if (index < 0 || index >= inventory.size()) {
            cout << "Invalid item index!\n";
            return;
        }
        inventory[index]->use();
    }

    void showStats() {
        cout << name << " Stats:\n";
        cout << "Level: " << level << "\n";
        cout << "Experience: " << experience << "\n";
        cout << "Health: " << health << "/" << maxHealth << "\n";
        cout << "Attack Power: " << getAttackPower() << "\n";
        cout << "Defense Power: " << getDefensePower() << "\n";
    }

    void gainExperience(int exp) {
        experience += exp;
        if (experience >= 100) {
            level++;
            experience = 0;
            maxHealth += 20;
            attackPower += 5;
            defensePower += 3;
            cout << "Congratulations! You've leveled up to level " << level << "!\n";
        }
    }
};

class Enemy {
private:
    string name;
    int health;
    int attackPower;
    vector<Item*> loot;

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

    void addLoot(Item* item) {
        loot.push_back(item);
    }

    void dropLoot() {
        cout << "Loot dropped by " << name << ":\n";
        for (auto& item : loot) {
            item->display();
        }
    }
};

class Quest {
public:
    string title;
    string description;
    bool isCompleted;

    Quest(string title, string description) : title(title), description(description), isCompleted(false) {}

    void complete() {
        isCompleted = true;
        cout << "Quest Completed: " << title << "\n";
    }

    void show() {
        cout << "Quest: " << title << "\n";
        cout << description << "\n";
        if (isCompleted) {
            cout << "Status: Completed\n";
        } else {
            cout << "Status: In Progress\n";
        }
    }
};

class Game {
private:
    Character* player;
    vector<Enemy> enemies;
    vector<Quest> quests;
    bool isRunning;

public:
    Game() : player(nullptr), isRunning(true) {}

    void start() {
        cout << "Welcome to the RPG Game!\n";
        string playerName;
        cout << "Enter your character's name: ";
        cin >> playerName;

        player = new Character(playerName, 100, 20);

        // Sample enemies
        enemies.push_back(Enemy("Goblin", 50, 10));
        enemies.push_back(Enemy("Troll", 120, 15));

        // Sample items
        player->addItem(new Weapon("Sword", 30, COMMON, 15));
        player->addItem(new Armor("Leather Armor", 25, UNCOMMON, 5));
        player->addItem(new Potion("Health Potion", 10, COMMON, 30));
        player->addItem(new MagicalItem("Ring of Strength", 50, RARE, 10));
        player->addItem(new Scroll("Fireball Scroll", 40, LEGENDARY, FIREBALL));

        // Sample traps and artifacts
        player->addItem(new Trap("Poison Trap", 15, RARE, 20));
        player->addItem(new Artifact("Amulet of Wisdom", 100, LEGENDARY, "Increases magic power"));

        // Sample quests
        quests.push_back(Quest("Defeat the Goblin", "Defeat the Goblin and receive a reward"));
        quests.push_back(Quest("Defeat the Troll", "Defeat the Troll and receive a rare item"));

        gameLoop();
    }

    void gameLoop() {
        while (isRunning) {
            cout << "\nWhat would you like to do?\n";
            cout << "1. View Stats\n";
            cout << "2. View Inventory\n";
            cout << "3. Fight Enemy\n";
            cout << "4. Heal\n";
            cout << "5. Equip Item\n";
            cout << "6. Use Item\n";
            cout << "7. Complete Quest\n";
            cout << "8. Save Game\n";
            cout << "9. Exit Game\n";

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
                    equipItem();
                    break;
                case 6:
                    useItem();
                    break;
                case 7:
                    completeQuest();
                    break;
                case 8:
                    saveGame();
                    break;
                case 9:
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
                enemy.dropLoot();  // Drop loot
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

    void equipItem() {
        cout << "Choose item to equip:\n";
        for (int i = 0; i < player->inventory.size(); ++i) {
            cout << i + 1 << ". ";
            player->inventory[i]->display();
        }

        int choice;
        cin >> choice;

        if (choice < 1 || choice > player->inventory.size()) {
            cout << "Invalid choice!\n";
            return;
        }

        Item* item = player->inventory[choice - 1];
        if (Weapon* weapon = dynamic_cast<Weapon*>(item)) {
            player->equipWeapon(weapon);
        } else if (Armor* armor = dynamic_cast<Armor*>(item)) {
            player->equipArmor(armor);
        } else if (Scroll* scroll = dynamic_cast<Scroll*>(item)) {
            player->equipScroll(scroll);
        } else {
            cout << "Cannot equip this item.\n";
        }
    }

    void useItem() {
        cout << "Choose item to use:\n";
        for (int i = 0; i < player->inventory.size(); ++i) {
            cout << i + 1 << ". ";
            player->inventory[i]->display();
        }

        int choice;
        cin >> choice;

        if (choice < 1 || choice > player->inventory.size()) {
            cout << "Invalid choice!\n";
            return;
        }

        player->useItem(choice - 1);
    }

    void completeQuest() {
        cout << "Choose quest to complete:\n";
        for (int i = 0; i < quests.size(); ++i) {
            cout << i + 1 << ". ";
            quests[i].show();
        }

        int choice;
        cin >> choice;

        if (choice < 1 || choice > quests.size()) {
            cout << "Invalid choice!\n";
            return;
        }

        Quest& quest = quests[choice - 1];
        if (!quest.isCompleted) {
            quest.complete();
            player->gainExperience(50);  // Give XP for completing quest
        } else {
            cout << "Quest already completed.\n";
        }
    }

    void saveGame() {
        ofstream outFile("savegame.txt");
        outFile << player->getName() << endl;
        outFile << player->getHealth() << endl;
        outFile << player->getAttackPower() << endl;
        outFile << player->getDefensePower() << endl;
        outFile << player->getLevel() << endl;
        outFile << player->getExperience() << endl;
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
        int health, attackPower, defensePower, level, experience;
        inFile >> playerName >> health >> attackPower >> defensePower >> level >> experience;
        player = new Character(playerName, health, attackPower);
        player->heal(0);  // Initialize max health after loading
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
