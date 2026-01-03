#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>

// ----------------------
// Item structure 
// ----------------------
struct Item {
    std::string code;
    std::string name;
    int pricePence;  // money in pence
    int stock;
};

// ----------------------
// Allowed coins (pence) 
// ----------------------
const int COINS[] = {200, 100, 50, 20, 10, 5};
const int NUM_COINS = static_cast<int>(sizeof(COINS) / sizeof(COINS[0]));

// ----------------------
// Trim spaces
// ----------------------
std::string trim(const std::string& s) {
    const size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    const size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

// ----------------------
// Check if string is digits only
// ----------------------
bool isDigits(const std::string& s) {
    if (s.empty()) return false;
    for (unsigned char c : s) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

// ----------------------
// Convert to uppercase
// ----------------------
std::string toUpper(std::string s) {
    for (char& ch : s) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return s;
}

// ----------------------
// Format pence as "150p"
// ----------------------
std::string penceStr(int pence) {
    return std::to_string(pence) + "p";
}

// ----------------------
// Print pence directly
// ----------------------
void printPence(int pence) {
    std::cout << pence << "p";
}

// ----------------------
// Show menu 
// ----------------------
void showMenu(const std::vector<Item>& items, int balancePence) {
    std::cout << "\n========== VENDING MACHINE ==========\n";
    std::cout << "Balance: ";
    printPence(balancePence);
    std::cout << "\n-------------------------------------\n";
    std::cout << std::left << std::setw(6) << "Code"
              << std::left << std::setw(18) << "Item"
              << std::right << std::setw(8) << "Price"
              << std::right << std::setw(8) << "Stock" << "\n";
    std::cout << "-------------------------------------\n";

    for (const auto& it : items) {
        std::cout << std::left << std::setw(6) << it.code
                  << std::left << std::setw(18) << it.name
                  << std::right << std::setw(8) << penceStr(it.pricePence)
                  << std::right << std::setw(8) << it.stock << "\n";
    }
    std::cout << "-------------------------------------\n";
    std::cout << "Options: I) Insert money   S) Select item   R) Return change   Q) Quit\n";
}

// ----------------------
// Insert coins 
// ----------------------
void insertMoney(int& balancePence) {
    std::cout << "\nInsert coins (in pence): 5, 10, 20, 50, 100, 200\n";
    std::cout << "Type value in pence (e.g., 50) or 'done' to finish.\n";

    std::string input;
    while (true) {
        std::cout << "Coin (pence) or 'done': ";
        if (!std::getline(std::cin, input)) return;

        input = toUpper(trim(input));
        if (input == "DONE") break;

        if (!isDigits(input)) {
            std::cout << "  Invalid input. Please enter 5, 10, 20, 50, 100, or 200.\n";
            continue;
        }

        int p = 0;
        try { p = std::stoi(input); }
        catch (...) {
            std::cout << "  Invalid number.\n";
            continue;
        }

        bool allowed = false;
        for (int i = 0; i < NUM_COINS; ++i) {
            if (COINS[i] == p) { allowed = true; break; }
        }

        if (!allowed) {
            std::cout << "  Unsupported coin: " << p << "p\n";
            continue;
        }

        balancePence += p;
        std::cout << "  Added " << p << "p. New balance ";
        printPence(balancePence);
        std::cout << "\n";
    }
}

// ----------------------
// Find item by code
// ----------------------
int findItemIndexByCode(const std::vector<Item>& items, const std::string& code) {
    const std::string target = toUpper(trim(code));
    for (size_t i = 0; i < items.size(); ++i) {
        if (toUpper(items[i].code) == target) return static_cast<int>(i);
    }
    return -1;
}

// ----------------------
// Make change 
// ----------------------
std::vector<int> makeChange(int amountPence) {
    std::vector<int> counts(NUM_COINS, 0);
    int remaining = amountPence;

    for (int i = 0; i < NUM_COINS; ++i) {
        const int coin = COINS[i];
        const int take = remaining / coin;
        counts[i] = take;
        remaining -= take * coin;
    }
    return counts;
}

// ----------------------
// Return change
// ----------------------
void returnChange(int& balancePence) {
    if (balancePence <= 0) {
        std::cout << "No change to return.\n";
        return;
    }

    const int total = balancePence;
    const auto counts = makeChange(balancePence);
    balancePence = 0;

    std::cout << "\n*** RETURNING CHANGE: ";
    printPence(total);
    std::cout << " ***\n";

    for (int i = 0; i < NUM_COINS; ++i) {
        const int coin = COINS[i];
        const int cnt  = counts[i];
        if (cnt > 0) {
            std::cout << "  " << coin << "p x " << cnt << "\n";
        }
    }
    std::cout << "***************************************\n";
}

// ----------------------
// Select and dispense 
// ----------------------
void selectItem(std::vector<Item>& items, int& balancePence) {
    std::cout << "\nEnter item code (e.g., A1). Type 'back' to cancel.\n";
    std::cout << "Code: ";

    std::string code;
    if (!std::getline(std::cin, code)) return;
    if (toUpper(trim(code)) == "BACK") return;

    const int idx = findItemIndexByCode(items, code);
    if (idx == -1) {
        std::cout << "  Unknown code. Please try again.\n";
        return;
    }

    Item& it = items[static_cast<size_t>(idx)];

    if (it.stock <= 0) {
        std::cout << "  Sorry, " << it.name << " is out of stock.\n";
        return;
    }

    if (balancePence < it.pricePence) {
        const int needed = it.pricePence - balancePence;
        std::cout << "  Insufficient funds. Need " << penceStr(needed) << " more.\n";
        return;
    }

    balancePence -= it.pricePence;
    it.stock -= 1;

    std::cout << "\n*** DISPENSING: " << it.name << " (" << it.code << ") ***\n";
    std::cout << "Price: " << penceStr(it.pricePence)
              << " | Remaining balance: " << penceStr(balancePence) << "\n";

    if (balancePence > 0) {
        std::cout << "Return change now? (y/n): ";
        std::string ans;
        if (std::getline(std::cin, ans)) {
            ans = toUpper(trim(ans));
            if (!ans.empty() && ans[0] == 'Y') {
                returnChange(balancePence);
            }
        }
    }
}

// ----------------------
// Main 
// ----------------------
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<Item> items = {
        {"A1", "Coffee",        150, 5},
        {"A2", "Tea",           120, 5},
        {"B1", "Cola",          130, 6},
        {"B2", "Orange Soda",   120, 6},
        {"C1", "Chocolate Bar", 100, 4},
        {"D1", "Crisps (Salt)",  90, 5},
        {"E1", "Biscuits",      110, 4}
    };

    int balancePence = 0;

    bool running = true;
    while (running) {
        showMenu(items, balancePence);
        std::cout << "Choose [I/S/R/Q]: ";

        std::string choice;
        if (!std::getline(std::cin, choice)) break;
        choice = trim(choice);
        if (choice.empty()) continue;

        const char c = static_cast<char>(std::toupper(static_cast<unsigned char>(choice[0])));

        if (c == 'I') {
            insertMoney(balancePence);
        } else if (c == 'S') {
            selectItem(items, balancePence);
        } else if (c == 'R') {
            returnChange(balancePence);
        } else if (c == 'Q') {
            if (balancePence > 0) {
                std::cout << "\nYou have a remaining balance.\n";
                returnChange(balancePence);
            }
            std::cout << "Goodbye!\n";
            running = false;
        } else {
            std::cout << "Unknown option. Please choose I, S, R or Q.\n";
        }
    }

    return 0;
}
