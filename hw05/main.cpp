#include <iostream>
#include <string>

using std::string;

int remainingTurns {6};
string secret {};

string checkWord(string inp);

int main() {

    std::string readin;

    std::cout << "Give me a secret word: " << '\n';
    std::getline(std::cin, secret);

    while (remainingTurns > 0) {
        std::cout << "Give me a guess: " << '\n';

        if (!std::getline(std::cin, readin)) {
            break;
        }

        std::cout << checkWord(readin) << "\n";

        if (readin == secret) {
            std::cout << "You Win!" << std::endl;
            return 0;
        }


        remainingTurns--;
    }

    std::cout << "You Lose." << std::endl;

    return 0;
}

string checkWord(string inp) {
    
    string output {};

    for (size_t i = 0; i < inp.length(); ++i) {
        if (i < secret.length() && inp.at(i) == secret.at(i)) {
            output.push_back(inp.at(i));
        } else if (secret.find(inp.at(i)) != string::npos) {
            output.push_back('?');
        }
        else {
            output.push_back('.');
        }
    }
    
    return output;
}
