#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <numeric>
#include <string>
#include <vector>

template<class ContainerT>
void tokenize(const std::string_view& str, ContainerT& tokens,
              const std::string_view& delimiters=" \n\t", bool trimEmpty=true) {
    std::string_view::size_type lastPos = 0, length = str.length();
    
    using value_type = typename ContainerT::value_type;
    using size_type = typename ContainerT::size_type;

    while(lastPos < length+1) {
        std::string_view::size_type pos = str.find_first_of(delimiters, lastPos);
        if(pos == std::string_view::npos) pos = length;

        if(pos != lastPos || !trimEmpty) {
            tokens.push_back(value_type(str.data()+lastPos,
                             static_cast<size_type>(pos)-lastPos));
        }
        lastPos = pos+1;
    }
}

struct Game {
    Game(const std::string &game) {
        std::vector<std::string> tokens;
        auto idx = game.find(':', 0);
        auto space = game.find(' ', 0);
        id = std::stoull(game.substr(space, idx-space));
        auto moves = game.substr(idx+1);
        tokenize(moves, tokens, ";");
        for(const auto &move : tokens) {
            ParseMove(move);
        }
    }

    void ParseMove(const std::string &move) {
        std::vector<std::string> tokens;
        tokenize(move, tokens, ",");
        for(const auto &cubes : tokens) {
            auto space = cubes.find(' ', 1);
            std::string color = cubes.substr(space+1);
            size_t count = std::stoull(cubes.substr(1, space));
            if(color == "red") {
                red = std::max(count, red);
            } else if(color == "green") {
                green = std::max(count, green);
            } else if(color == "blue") {
                blue = std::max(count, blue);
            }
        }
    }

    bool IsValid(size_t rmax, size_t gmax, size_t bmax) {
        return rmax >= red && gmax >= green && bmax >= blue;
    }

    size_t id;
    size_t red{0}, green{0}, blue{0};
};

int main(int argc, char *argv[]) {
    if(argc != 2) {
        std::cout << "Requires a file input!\n";
        return -1;
    }

    std::ifstream input(argv[1]);
    std::vector<Game> games;
    std::string line;
    while(std::getline(input, line)) {
        games.emplace_back(line);
    }

    auto sum_valid = [](size_t a, Game b) {
        return a + (b.IsValid(12, 13, 14) ? b.id : 0);
    };

    auto power_sum = [](size_t a, Game b) {
        return a + b.red*b.green*b.blue;
    };

    auto sum = std::accumulate(games.begin(), games.end(), 0, sum_valid);
    auto sum2 = std::accumulate(games.begin(), games.end(), 0, power_sum);
    std::cout << "PartA: " << sum << std::endl;
    std::cout << "PartB: " << sum2 << std::endl;

    return 0;
}
