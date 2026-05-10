// =============================================
//   WORDLE - C++ Console Edition
//   A fun project for learning GitHub!
// =============================================

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cctype>

// ---- ANSI Color Codes for terminal output ----
const std::string RESET   = "\033[0m";
const std::string BOLD    = "\033[1m";
const std::string GREEN   = "\033[42m\033[30m";   // Correct letter, correct position
const std::string YELLOW  = "\033[43m\033[30m";   // Correct letter, wrong position
const std::string GRAY    = "\033[100m\033[37m";  // Letter not in word

const int WORD_LENGTH = 5;
const int MAX_GUESSES = 6;

// ---- Word list (add more words to make the game richer!) ----
const std::vector<std::string> WORD_LIST = {
    "apple", "brave", "chair", "dance", "eagle",
    "flame", "grace", "heart", "ivory", "joker",
    "kneel", "laser", "mango", "night", "ocean",
    "piano", "queen", "river", "stone", "tiger",
    "ultra", "viola", "water", "xenon", "yacht",
    "zebra", "bloom", "crisp", "drive", "earth",
    "frost", "gloom", "honey", "input", "juice",
    "karma", "lemon", "moose", "nurse", "olive",
    "plumb", "quill", "rouge", "solar", "touch",
    "under", "value", "witch", "xylem", "yearn"
};

// ---- Pick a random word from the list ----
std::string pickRandomWord() {
    srand(static_cast<unsigned int>(time(nullptr)));
    int index = rand() % WORD_LIST.size();
    return WORD_LIST[index];
}

// ---- Convert string to lowercase ----
std::string toLower(std::string s) {
    for (char& c : s) c = tolower(c);
    return s;
}

// ---- Check if a word is valid (in our word list) ----
bool isValidWord(const std::string& word) {
    return std::find(WORD_LIST.begin(), WORD_LIST.end(), word) != WORD_LIST.end();
}

// ---- Evaluate a guess against the target ----
// Returns a vector: 2 = green, 1 = yellow, 0 = gray
std::vector<int> evaluateGuess(const std::string& guess, const std::string& target) {
    std::vector<int> result(WORD_LENGTH, 0);
    std::vector<bool> targetUsed(WORD_LENGTH, false);
    std::vector<bool> guessUsed(WORD_LENGTH, false);

    // First pass: find exact matches (green)
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == target[i]) {
            result[i] = 2;
            targetUsed[i] = true;
            guessUsed[i] = true;
        }
    }

    // Second pass: find wrong-position matches (yellow)
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guessUsed[i]) continue;
        for (int j = 0; j < WORD_LENGTH; j++) {
            if (!targetUsed[j] && guess[i] == target[j]) {
                result[i] = 1;
                targetUsed[j] = true;
                break;
            }
        }
    }

    return result;
}

// ---- Display a colored guess row ----
void displayGuess(const std::string& guess, const std::vector<int>& result) {
    std::cout << "  ";
    for (int i = 0; i < WORD_LENGTH; i++) {
        char c = toupper(guess[i]);
        if (result[i] == 2)      std::cout << GREEN  << " " << c << " " << RESET;
        else if (result[i] == 1) std::cout << YELLOW << " " << c << " " << RESET;
        else                     std::cout << GRAY   << " " << c << " " << RESET;
    }
    std::cout << std::endl;
}

// ---- Display the title banner ----
void displayBanner() {
    std::cout << BOLD << "\n";
    std::cout << "  ╔══════════════════════════╗\n";
    std::cout << "  ║   W O R D L E  (C++)     ║\n";
    std::cout << "  ╚══════════════════════════╝\n" << RESET;
    std::cout << "  Guess the 5-letter word in " << MAX_GUESSES << " tries.\n";
    std::cout << "  " << GREEN << " G " << RESET << " = right letter, right spot\n";
    std::cout << "  " << YELLOW << " Y " << RESET << " = right letter, wrong spot\n";
    std::cout << "  " << GRAY << " X " << RESET << " = letter not in word\n\n";
}

// ---- Display the empty board ----
void displayEmptyRows(int guessesUsed) {
    for (int i = guessesUsed; i < MAX_GUESSES; i++) {
        std::cout << "  ";
        for (int j = 0; j < WORD_LENGTH; j++) {
            std::cout << "[ _ ]";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

// ---- Main game loop ----
void playGame() {
    std::string target = pickRandomWord();
    std::vector<std::string> guesses;
    std::vector<std::vector<int>> results;

    displayBanner();

    for (int attempt = 0; attempt < MAX_GUESSES; attempt++) {
        // Reprint all previous guesses
        std::cout << "\033[2J\033[H"; // Clear screen
        displayBanner();

        for (int i = 0; i < (int)guesses.size(); i++) {
            displayGuess(guesses[i], results[i]);
        }
        displayEmptyRows((int)guesses.size());

        // Get input
        std::string guess;
        std::cout << "  Attempt " << (attempt + 1) << "/" << MAX_GUESSES << " > ";
        std::cin >> guess;
        guess = toLower(guess);

        // Validate input
        if ((int)guess.length() != WORD_LENGTH) {
            std::cout << "  ⚠  Please enter a " << WORD_LENGTH << "-letter word.\n";
            attempt--;
            continue;
        }
        if (!isValidWord(guess)) {
            std::cout << "  ⚠  Not in word list. Try again.\n";
            attempt--;
            continue;
        }

        // Evaluate
        std::vector<int> result = evaluateGuess(guess, target);
        guesses.push_back(guess);
        results.push_back(result);

        // Check for win
        bool won = std::all_of(result.begin(), result.end(), [](int r){ return r == 2; });
        if (won) {
            std::cout << "\033[2J\033[H";
            displayBanner();
            for (int i = 0; i < (int)guesses.size(); i++) {
                displayGuess(guesses[i], results[i]);
            }
            std::cout << "\n  🎉 " << BOLD << "Brilliant! You got it in " << (attempt + 1) << " guess";
            if (attempt + 1 > 1) std::cout << "es";
            std::cout << "!" << RESET << "\n\n";
            return;
        }
    }

    // Game over - reveal the word
    std::cout << "\033[2J\033[H";
    displayBanner();
    for (int i = 0; i < (int)guesses.size(); i++) {
        displayGuess(guesses[i], results[i]);
    }
    std::cout << "\n  😢 Out of guesses! The word was: " << BOLD;
    for (char c : target) std::cout << toupper(c);
    std::cout << RESET << "\n\n";
}

// ---- Entry point ----
int main() {
    char playAgain = 'y';
    while (playAgain == 'y' || playAgain == 'Y') {
        playGame();
        std::cout << "  Play again? (y/n): ";
        std::cin >> playAgain;
    }
    std::cout << "\n  Thanks for playing! 👋\n\n";
    return 0;
}
