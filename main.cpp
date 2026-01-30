#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>

// ----------------------------------------------------
// STRUCT: Represents a vending machine item
// ----------------------------------------------------
struct Item {
    std::string code;      
    std::string name;       
    int pricePence;         
    int stock;              
};

// ----------------------------------------------------
// Allowed coin values (in pence)
// ----------------------------------------------------
const int COINS[] = {200, 100, 50, 20, 10, 5};
const int NUM_COINS = static_cast<int>(sizeof(COINS) / sizeof(COINS[0]));

// ----------------------------------------------------
// Helper: Trim whitespace from start and end
// ----------------------------------------------------
std::string trim(const std::string& s) {
    const size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    const size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

// ----------------------------------------------------
// Helper: Check if string contains only digits
// ----------------------------------------------------
bool isDigits(const std::string& s) {
    if (s.empty()) return false;
    for (unsigned char c : s) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

// ----------------------------------------------------
// Helper: Convert string to uppercase
// ----------------------------------------------------
std::string toUpper(std::string s) {
    for (char& ch : s) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return s;
}

// ----------------------------------------------------
// Converts pence integer to a formatted string (e.g. 150 -> "150p")
// ----------------------------------------------------
std::string penceStr(int pence) {
    return std::to_string(pence) + "p";
}

// ----------------------------------------------------
// Helper: Print pence without converting to string
// ----------------------------------------------------
void printPence(int pence) {
    std::cout << pence << "p";
}

// ----------------------------------------------------
// Display the vending machine menu
// Shows balance + list of all items
// ----------------------------------------------------
void showMenu(const std::vector<Item>& items, int balancePence) {
    std::cout << "\n========== VENDING MACHINE ==========\n";
    std::cout << "Balance: ";
    printPence(balancePence);
    std::cout << "\n-------------------------------------\n";

    std::cout << std::left << std::setw(6)  << "Code"
              << std::left << std::setw(18) << "Item"
              << std::right << std::setw(8) << "Price"
              << std::right << std::setw(8) << "Stock" << "\n";
    std::cout << "-------------------------------------\n";

    // Print each item
    for (const auto& it : items) {
        std::cout << std::left << std::setw(6)  << it.code
                  << std::left << std::setw(18) << it.name
                  << std::right << std::setw(8) << penceStr(it.pricePence)
                  << std::right << std::setw(8) << it.stock << "\n";
    }

    std::cout << "-------------------------------------\n";
    std::cout << "Options: I) Insert money   S) Select item   R) Return change   Q) Quit\n";
}

// ----------------------------------------------------
// Insert coins into the machine
// Only accepts valid coin values
// ----------------------------------------------------
void insertMoney(int& balancePence) {
    std::cout << "\nInsert coins (5, 10, 20, 50, 100, 200)\n";
    std::cout << "Type a value in pence or 'done' to finish.\n";

    std::string input;
    while (true) {
        std::cout << "Coin (or 'done'): ";
        if (!std::getline(std::cin, input)) return;

        input = toUpper(trim(input));

        if (input == "DONE") break;

        // Check digits
        if (!isDigits(input)) {
            std::cout << "  Invalid input.\n";
            continue;
        }

        int p = std::stoi(input);

        // Check if coin is allowed
        bool allowed = false;
        for (int i = 0; i < NUM_COINS; ++i)
            if (COINS[i] == p) allowed = true;

        if (!allowed) {
            std::cout << "  Unsupported coin: " << p << "p\n";
            continue;
        }

        // Add coin to balance
        balancePence += p;
        std::cout << "  Added " << p << "p. Balance now ";
        printPence(balancePence);
        std::cout << "\n";
    }
}

// ----------------------------------------------------
// Find the index of an item by its code (case-insensitive)
// ----------------------------------------------------
int findItemIndexByCode(const std::vector<Item>& items, const std::string& code) {
    const std::string target = toUpper(trim(code));
    for (size_t i = 0; i < items.size(); ++i) {
        if (toUpper(items[i].code) == target)
            return static_cast<int>(i);
    }
    return -1;
}

// ----------------------------------------------------
// Make change using the largest coins first
// Greedy algorithm (works for UK coin system)
// ----------------------------------------------------
std::vector<int> makeChange(int amountPence) {
    std::vector<int> counts(NUM_COINS, 0);
    int remaining = amountPence;

    for (int i = 0; i < NUM_COINS; ++i) {
        counts[i] = remaining / COINS[i];  // number of coins
        remaining -= counts[i] * COINS[i];
    }
    return counts;
}

// ----------------------------------------------------
// Return all remaining change to the customer
// ----------------------------------------------------
void returnChange(int& balancePence) {
    if (balancePence <= 0) {
        std::cout << "No change to return.\n";
        return;
    }

    int total = balancePence;
    auto counts = makeChange(balancePence);
    balancePence = 0;

    std::cout << "\n*** RETURNING CHANGE: ";
    printPence(total);
    std::cout << " ***\n";

    for (int i = 0; i < NUM_COINS; ++i) {
        if (counts[i] > 0)
            std::cout << "  " << COINS[i] << "p x " << counts[i] << "\n";
    }
}

// ----------------------------------------------------
// Handle selecting an item from the vending machine:
// - Validate code
// - Check stock
// - Check if enough money
// - Reduce balance and stock
// ----------------------------------------------------
void selectItem(std::vector<Item>& items, int& balancePence) {
    std::cout << "\nEnter item code or 'back'.\n";
    std::cout << "Code: ";

    std::string code;
    if (!std::getline(std::cin, code)) return;
    if (toUpper(trim(code)) == "BACK") return;

    // Find item
    int idx = findItemIndexByCode(items, code);
    if (idx == -1) {
        std::cout << "  Unknown code.\n";
        return;
    }

    Item& it = items[idx];

    // Check stock
    if (it.stock <= 0) {
        std::cout << "  Out of stock.\n";
        return;
    }

    // Check balance
    if (balancePence < it.pricePence) {
        std::cout << "  Need " << penceStr(it.pricePence - balancePence) << " more.\n";
        return;
    }

    // Dispense item
    balancePence -= it.pricePence;
    it.stock--;

    std::cout << "\n*** DISPENSING: " << it.name << " (" << it.code << ") ***\n";

    if (balancePence > 0) {
        std::cout << "Return change? (y/n): ";
        std::string ans;
        if (std::getline(std::cin, ans) && toUpper(trim(ans)) == "Y")
            returnChange(balancePence);
    }
}


int main() {
    // Initial items in the machine
    std::vector<Item> items = {
        {"A1", "Coffee", 150, 5},
        {"A2", "Tea", 120, 5},
        {"B1", "Cola", 130, 6},
        {"B2", "Orange Soda", 120, 6},
        {"C1", "Chocolate Bar", 100, 4},
        {"D1", "Crisps (Salt)", 90, 5},
        {"E1", "Biscuits", 110, 4}
    };

    int balancePence = 0;
    bool running = true;

    // --- MAIN USER LOOP ---
    while (running) {
        showMenu(items, balancePence);
        std::cout << "Choose [I/S/R/Q]: ";

        std::string choice;
        if (!std::getline(std::cin, choice)) break;

        if (choice.empty()) continue;

        char c = static_cast<char>(std::toupper(choice[0]));

        if (c == 'I') insertMoney(balancePence);
        else if (c == 'S') selectItem(items, balancePence);
        else if (c == 'R') returnChange(balancePence);
        else if (c == 'Q') {
            if (balancePence > 0) returnChange(balancePence);
            std::cout << "Goodbye!\n";
            running = false;
        }
        else {
            std::cout << "Unknown option.\n";
        }
    }

    return 0;
}
