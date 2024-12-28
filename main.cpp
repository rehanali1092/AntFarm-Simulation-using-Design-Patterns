#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <random>
#include <sstream>
using namespace std;

class Ant;
class AntRoom;
template <typename T>
class AntFarm;
class Drone;
class Warrior;
class Queen;

enum class Species
{
    DRONE,
    WARRIOR,
    QUEEN,
    KILLER,
    PANSY
};

class Ant
{
protected:
    int health;
    int attack;
    int defense;
    Species species;
    int activeTicks;
    bool resting;

public:
    Ant(Species s, int h = 100, int atk = 10, int def = 5)
        : species(s), health(h), attack(atk), defense(def), activeTicks(0), resting(false) {}

    virtual ~Ant() = default;
    virtual void act() = 0;

    bool isAlive() const { return health > 0; }
    Species getSpecies() const { return species; }
    int getHealth() const { return health; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    int getActiveTicks() const { return activeTicks; }

    void rest()
    {
        resting = true;
        health -= 5;
        cout << "Ant is resting. Health after resting: " << health << "\n";
    }

    void tick()
    {
        if (resting)
        {
            resting = false;
            health += 10;
            cout << "Ant finished resting. Health: " << health << "\n";
        }
        else
        {
            activeTicks++;
            cout << "Ant is active. Active ticks: " << activeTicks << "\n";
        }
    }

    string getSpeciesName() const
    {
        switch (species)
         {
            case Species::DRONE: return "Drone";
            case Species::WARRIOR: return "Warrior";
            case Species::QUEEN: return "Queen";
            case Species::KILLER: return "Killer";
            case Species::PANSY: return "Pansy";
            default: return "Unknown";
        }
    }

    virtual bool battle(shared_ptr<Ant> opponent)
    {
        if (!opponent || !opponent->isAlive() || !isAlive())
        {
            return false;
        }

        int damageToOpponent = max(0, this->attack - opponent->defense);
        int damageToSelf = max(0, opponent->attack - this->defense);

        opponent->health -= damageToOpponent;
        this->health -= damageToSelf;

        return opponent->health <= 0;
    }
};

class Drone : public Ant
{
public:
    Drone() : Ant(Species::DRONE, 80, 5, 3) {}
    void act() override
    {
        cout << "Drone is looking for food. Health: " << getHealth() << "\n";
    }
};

class Warrior : public Ant
{
public:
    Warrior() : Ant(Species::WARRIOR, 120, 15, 8) {}
    void act() override
    {
        cout << "Warrior is hunting. Health: " << getHealth() << "\n";
    }
};

class Queen : public Ant
{
public:
    Queen() : Ant(Species::QUEEN, 200, 20, 15) {}
    void act() override {
        cout << "Queen is spawning an egg. Health: " << getHealth() << "\n";
    }
};


class AntRoom
{
private:
    vector<shared_ptr<Ant>> ants;
    vector<shared_ptr<AntRoom>> connectedRooms;
    int maxRestingCapacity;

    void removeDeadAnts()
    {
        ants.erase(remove_if(ants.begin(), ants.end(),
            [](const shared_ptr<Ant>& ant) { return !ant->isAlive(); }),
            ants.end());
    }

public:
    AntRoom(int capacity) : maxRestingCapacity(capacity) {}

    void addAnt(shared_ptr<Ant> ant)
    {
        ants.push_back(ant);
    }

    shared_ptr<Ant> createAnt(Species species)
    {
        switch (species) {
            case Species::DRONE: return make_shared<Drone>();
            case Species::WARRIOR: return make_shared<Warrior>();
            case Species::QUEEN: return make_shared<Queen>();
            default: return nullptr;
        }
    }

    const vector<shared_ptr<Ant>>& getAnts() const
    {
        return ants;
    }

    void interact()
    {
        for (auto& ant : ants)
        {
            if (ant->isAlive())
            {
                ant->act();
            }
        }
    }

    void restAnts()
    {
        int restingCount = 0;
        for (auto& ant : ants)
        {
            if (!ant->isAlive()) continue;
            if (restingCount < maxRestingCapacity)
            {
                ant->rest();
                restingCount++;
            }
        }
        removeDeadAnts();
    }

    string summary()
    {
        stringstream ss;
        for (const auto& ant : ants) {
            ss << "Species: " << ant->getSpeciesName() << "\n";
            ss << "Health: " << ant->getHealth() << "\n";
            ss << "Active Ticks: " << ant->getActiveTicks() << "\n";
            ss << "Status: " << (ant->isAlive() ? "Alive" : "Dead") << "\n";
        }
        return ss.str();
    }
};

template <typename T>
class AntFarm
{
private:
    vector<shared_ptr<AntRoom>> rooms;
    unordered_map<int, shared_ptr<AntRoom>> colonies;
    int colonyCount = 0;

public:
    shared_ptr<AntRoom> addRoom(int capacity)
    {
        auto room = make_shared<AntRoom>(capacity);
        rooms.push_back(room);
        return room;
    }

    int spawnColony(Species species, int x, int y)
    {
        auto room = addRoom(10);
        colonies[++colonyCount] = room;
        room->addAnt(room->createAnt(species));
        return colonyCount;
    }

    void tick(int times = 1)
    {
        for (int i = 0; i < times; ++i)
        {
            for (const auto& room : rooms)
            {
                room->restAnts();
                room->interact();
            }
        }
    }

    string summary(int id)
    {
        if (colonies.find(id) == colonies.end())
        {
            return "Colony not found.";
        }
        return colonies[id]->summary();
    }
};

class Meadow
{
public:
    static Meadow& getInstance()
    {
        static Meadow instance;
        return instance;
    }

    shared_ptr<AntFarm<Ant>> createAntFarm()
    {
        return make_shared<AntFarm<Ant>>();
    }
private:
    Meadow() = default;
};

void executeCommand(const string& command, shared_ptr<AntFarm<Ant>>& farm)
{
    stringstream ss(command);
    string action;
    ss >> action;

    try
    {
        if (action == "spawn")
        {
            int x, y;
            string speciesStr;
            ss >> x >> y >> speciesStr;

            Species species;
            if (speciesStr == "Killer")
            {
                species = Species::KILLER;
            }
            else if (speciesStr == "Pansy")
            {
                species = Species::PANSY;
            }
            else
            {
                cout << "Unknown species.\n";
                return;
            }

            int colonyId = farm->spawnColony(species, x, y);
            cout << "Colony created with ID: " << colonyId << endl;
        }
        else if (action == "give")
        {
            int id;
            string resource;
            int amount;
            ss >> id >> resource >> amount;
            cout << "Gave " << amount << " of " << resource << " to colony " << id << endl;
        }
        else if (action == "tick")
        {
            int T = 1;
            if (ss >> T)
            {
                farm->tick(T);
            }
            else
            {
                farm->tick();
            }
            cout << "Tick executed for " << T << " cycles.\n";
        }
        else if (action == "summary")
        {
            int id;
            ss >> id;
            cout << farm->summary(id) << endl;
        }
        else
        {
            cout << "Unknown command.\n";
        }
    }
    catch (const exception& e)
    {
        cerr << "Error while processing command: " << e.what() << endl;
    }
}

int main()
{
    Meadow& meadow = Meadow::getInstance();
    auto antFarm = meadow.createAntFarm();

    string command;
    cout << "Welcome to the Ant Farm Simulator!\n";
    cout << "Available commands:\n";
    cout << "  spawn X Y T\n";
    cout << "  give I R A\n";
    cout << "  tick [T]\n";
    cout << "  summary I\n";
    cout << "Type 'exit' to quit.\n";

    while (true)
    {
        cout << "> ";
        getline(cin, command);
        if (command == "exit")
        {
            break;
        }
        if (!command.empty())
        {
            executeCommand(command, antFarm);
        }
    }

    return 0;
}
