#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
using namespace std;

const float PROBABILITY = 0.5;
const int MAX_LEVEL = 16;

struct Node {
    int key; // Represents the score
    string value; // Represents the player's name
    vector<Node*> forward;

    Node(int key, string value, int level) : key(key), value(value), forward(level, nullptr) {}
};

class SkipList {
private:
    Node* head;
    int level;

    int randomLevel() {
        int lvl = 1;
        while ((float)rand() / RAND_MAX < PROBABILITY && lvl < MAX_LEVEL)
            lvl++;
        return lvl;
    }

public:
    SkipList() {
        head = new Node(-1, "", MAX_LEVEL);
        level = 1;
    }

    ~SkipList() {
        Node* current = head;
        while (current) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
    }

    void insert(int key, const string& value) {
        vector<Node*> update(MAX_LEVEL, nullptr);
        Node* current = head;

        for (int i = level - 1; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
            update[i] = current;
        }

        current = current->forward[0];

        if (current == nullptr || current->key != key) {
            int newLevel = randomLevel();

            if (newLevel > level) {
                for (int i = level; i < newLevel; i++)
                    update[i] = head;
                level = newLevel;
            }

            Node* newNode = new Node(key, value, newLevel);

            for (int i = 0; i < newLevel; i++) {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
        } else {
            current->value = value; // Update existing key
        }
    }

    void erase(int key) {
        vector<Node*> update(MAX_LEVEL, nullptr);
        Node* current = head;

        for (int i = level - 1; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
            update[i] = current;
        }

        current = current->forward[0];

        if (current != nullptr && current->key == key) {
            for (int i = 0; i < level; i++) {
                if (update[i]->forward[i] != current)
                    break;
                update[i]->forward[i] = current->forward[i];
            }

            delete current;

            while (level > 1 && head->forward[level - 1] == nullptr)
                level--;
        }
    }

    Node* search(int key) {
        Node* current = head;

        for (int i = level - 1; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
        }

        current = current->forward[0];
        if (current && current->key == key)
            return current;
        return nullptr;
    }

    void display() {
        for (int i = level - 1; i >= 0; i--) {
            Node* node = head->forward[i];
            cout << "Level " << i + 1 << ": ";
            while (node) {
                cout << "(" << node->key << ", " << node->value << ") ";
                node = node->forward[i];
            }
            cout << endl;
        }
    }

    Node* getHead() {
        return head;
    }
};

class GameScores {
private:
    SkipList skipList;

public:
    void addPlayer(int score, const string& name) {
        skipList.insert(score, name);
    }

    void removePlayer(int score) {
        skipList.erase(score);
    }

    void updateScore(int oldScore, int newScore, const string& name) {
        skipList.erase(oldScore);
        skipList.insert(newScore, name);
    }

    void displayLeaderboard(int topN) {
        Node* current = skipList.getHead()->forward[0];
        int count = 0;
        while (current && count < topN) {
            cout << "Player Score: " << current->key << ", Name: " << current->value << endl;
            current = current->forward[0];
            count++;
        }
    }

    void viewPlayer(int score) {
        Node* player = skipList.search(score);
        if (player)
            cout << "Player Score: " << player->key << ", Name: " << player->value << endl;
        else
            cout << "Player not found." << endl;
    }
};

int main() {
    srand(time(0));

    GameScores game;
    game.addPlayer(100, "Alice");
    game.addPlayer(200, "Bob");
    game.addPlayer(150, "Charlie");

    cout << "Leaderboard:" << endl;
    game.displayLeaderboard(10);

    game.updateScore(200, 250, "Bob");
    cout << "\nAfter Updating Bob's Score:" << endl;
    game.displayLeaderboard(10);

    game.removePlayer(100);
    cout << "\nAfter Removing Alice:" << endl;
    game.displayLeaderboard(10);

    cout << "\nViewing Charlie's Details:" << endl;
    game.viewPlayer(150);

    return 0;
}
