
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

struct Node {
    string word;
    Node* next;

    Node(string w) {
        word = w;
        next = NULL;
    }
};

class Dictionary {
private:
    Node* alphabet[26];

    string toLowerCase(string word) {
        for (int i = 0; i < (int)word.length(); i++) {
            word[i] = tolower((unsigned char)word[i]);
        }
        return word;
    }

    int getIndex(string word) {
        if (word.empty())
            return -1;

        char ch = toupper((unsigned char)word[0]);

        if (ch < 'A' || ch > 'Z')
            return -1;

        return ch - 'A';
    }

    bool isValidWord(string word) {
        if (word.empty())
            return false;

        for (int i = 0; i < (int)word.length(); i++) {
            if (!isalpha((unsigned char)word[i]))
                return false;
        }

        return true;
    }

public:
    Dictionary() {
        for (int i = 0; i < 26; i++) {
            alphabet[i] = NULL;
        }
    }

    // Prevent copying linked-list pointers.
    Dictionary(const Dictionary&) = delete;
    Dictionary& operator=(const Dictionary&) = delete;

    ~Dictionary() {
        for (int i = 0; i < 26; i++) {
            Node* current = alphabet[i];

            while (current != NULL) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }

            alphabet[i] = NULL;
        }
    }

    // Insert a word in alphabetical order.
    bool insertWord(string word) {
        if (!isValidWord(word)) {
            cout << "Invalid word! Use English letters only.\n";
            return false;
        }

        int index = getIndex(word);
        string key = toLowerCase(word);

        Node* current = alphabet[index];
        Node* previous = NULL;

        while (current != NULL &&
               toLowerCase(current->word) < key) {
            previous = current;
            current = current->next;
        }

        if (current != NULL &&
            toLowerCase(current->word) == key) {
            cout << "Word already exists!\n";
            return false;
        }

        Node* newNode = new Node(word);
        newNode->next = current;

        if (previous == NULL)
            alphabet[index] = newNode;
        else
            previous->next = newNode;

        cout << "Word inserted successfully.\n";
        return true;
    }

    // Search for a word.
    bool searchWord(string word) {
        if (!isValidWord(word)) {
            cout << "Invalid word!\n";
            return false;
        }

        int index = getIndex(word);
        string key = toLowerCase(word);

        Node* current = alphabet[index];

        while (current != NULL) {
            string currentKey = toLowerCase(current->word);

            if (currentKey == key) {
                cout << "Word found: " << current->word << '\n';
                return true;
            }

            if (currentKey > key)
                break;

            current = current->next;
        }

        cout << "Word not found.\n";
        return false;
    }

    // Delete a word.
    bool deleteWord(string word) {
        if (!isValidWord(word)) {
            cout << "Invalid word!\n";
            return false;
        }

        int index = getIndex(word);
        string key = toLowerCase(word);

        Node* current = alphabet[index];
        Node* previous = NULL;

        while (current != NULL &&
               toLowerCase(current->word) < key) {
            previous = current;
            current = current->next;
        }

        if (current == NULL ||
            toLowerCase(current->word) != key) {
            cout << "Word not found.\n";
            return false;
        }

        if (previous == NULL)
            alphabet[index] = current->next;
        else
            previous->next = current->next;

        delete current;

        cout << "Word deleted successfully.\n";
        return true;
    }

    // Display all words.
    void displayAll() {
        bool empty = true;

        cout << "\n========== DICTIONARY ==========\n";

        for (int i = 0; i < 26; i++) {
            if (alphabet[i] != NULL) {
                empty = false;

                cout << char('A' + i) << ": ";

                Node* current = alphabet[i];

                while (current != NULL) {
                    cout << current->word;

                    if (current->next != NULL)
                        cout << " -> ";

                    current = current->next;
                }

                cout << " -> NULL\n";
            }
        }

        if (empty)
            cout << "Dictionary is empty.\n";

        cout << "================================\n";
    }

    // Display words beginning with a specific letter.
    void displayByLetter(char letter) {
        letter = toupper((unsigned char)letter);

        if (letter < 'A' || letter > 'Z') {
            cout << "Invalid letter!\n";
            return;
        }

        int index = letter - 'A';
        Node* current = alphabet[index];

        cout << "\nWords beginning with " << letter << ":\n";

        if (current == NULL) {
            cout << "No words found for this letter.\n";
            return;
        }

        while (current != NULL) {
            cout << current->word << '\n';
            current = current->next;
        }
    }

    // Count all words.
    int countWords() {
        int count = 0;

        for (int i = 0; i < 26; i++) {
            Node* current = alphabet[i];

            while (current != NULL) {
                count++;
                current = current->next;
            }
        }

        return count;
    }
};

int main() {
    Dictionary dictionary;
    int choice = -1;
    string word;
    char letter;

    do {
        cout << "\n===== ENGLISH DICTIONARY =====\n";
        cout << "1. Insert a word\n";
        cout << "2. Search for a word\n";
        cout << "3. Delete a word\n";
        cout << "4. Display all words\n";
        cout << "5. Display words by letter\n";
        cout << "6. Count all words\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
        case 1:
            cout << "Enter a word: ";
            cin >> word;
            dictionary.insertWord(word);
            break;

        case 2:
            cout << "Enter a word to search: ";
            cin >> word;
            dictionary.searchWord(word);
            break;

        case 3:
            cout << "Enter a word to delete: ";
            cin >> word;
            dictionary.deleteWord(word);
            break;

        case 4:
            dictionary.displayAll();
            break;

        case 5:
            cout << "Enter a letter (A-Z): ";
            cin >> letter;
            dictionary.displayByLetter(letter);
            break;

        case 6:
            cout << "Total words: "
                 << dictionary.countWords() << '\n';
            break;

        case 0:
            cout << "Exiting dictionary. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
