#include <iostream>
#include <random>
#include <fstream>
#include <string>
#include <unordered_set>
using namespace std;

bool getYesNoAnswer(const string& question) {
    char choice;
    do {
        cout << question << " (y/n): ";
        cin >> choice;
        choice = tolower(choice);
        if (choice != 'y' && choice != 'n') {
            cout << "Ошибка! Введите 'y' для да или 'n' для нет.\n";
        }
    } while (choice != 'y' && choice != 'n');
    return choice == 'y';
}

string generatePassword(int length, bool useNumbers, bool useLetters, bool useSymbols) {
    string numbers = "0123456789";
    string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string symbols = "!@#$%^&*()-_=+[]{}|;:'\",.<>?/";

    string characters = "";
    if (useNumbers) characters += numbers;
    if (useLetters) characters += letters;
    if (useSymbols) characters += symbols;

    if (characters.empty()) {
        return "Ошибка: Не выбран ни один тип символов!";
    }

    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(0, characters.size() - 1);

    string password = "";
    unordered_set<char> usedChars;

    // Гарантируем хотя бы один символ каждого выбранного типа
    if (useNumbers) {
        password += numbers[distribution(generator) % numbers.size()];
        usedChars.insert(password.back());
    }
    if (useLetters) {
        password += letters[distribution(generator) % letters.size()];
        usedChars.insert(password.back());
    }
    if (useSymbols) {
        password += symbols[distribution(generator) % symbols.size()];
        usedChars.insert(password.back());
    }

    // Генерация оставшихся символов
    while (password.size() < length) {
        char randomChar = characters[distribution(generator)];
        if (usedChars.find(randomChar) == usedChars.end() || password.size() >= 3) {
            password += randomChar;
        }
    }

    return password;
}

// Функция для оценки надежности пароля
string evaluateStrength(const string &password) {
    bool hasLower = false, hasUpper = false, hasDigit = false, hasSpecial = false;

    for (char c : password) {
        if (islower(c)) hasLower = true;
        else if (isupper(c)) hasUpper = true;
        else if (isdigit(c)) hasDigit = true;
        else hasSpecial = true;
    }

    int criteriaMet = hasLower + hasUpper + hasDigit + hasSpecial;

    if (password.length() >= 12 && criteriaMet == 4) return "Сильный";
    if (password.length() >= 8 && criteriaMet >= 3) return "Средний";
    return "Слабый";
}

void savePasswordsToFile(const vector<string> &passwords) {
    ofstream file("passwords.txt");
    if (file.is_open()) {
        file << "Сгенерированные пароли:\n";
        for (const string &password : passwords) {
            file << password << " (" << evaluateStrength(password) << ")\n";
        }
        file.close();
        cout << "Пароли сохранены в passwords.txt\n";
    } else {
        cout << "Ошибка: не удалось открыть файл!\n";
    }
}

int main() {
    int length, count;
    vector<string> passwords;

    cout << "=== Генератор паролей ===" << endl;
    cout << "Введите длину пароля: ";
    cin >> length;

    bool useNumbers = getYesNoAnswer("Использовать цифры?");
    bool useLetters = getYesNoAnswer("Использовать буквы?");
    bool useSymbols = getYesNoAnswer("Использовать спецсимволы?");

    cout << "Сколько паролей сгенерировать?: ";
    cin >> count;

    cout << "\nСгенерированные пароли:\n";
    for (int i = 0; i < count; i++) {
        string password = generatePassword(length, useNumbers, useLetters, useSymbols);
        cout << i + 1 << ": " << password << " (" << evaluateStrength(password) << ")\n";
        passwords.push_back(password);
    }

    savePasswordsToFile(passwords);

    return 0;
}