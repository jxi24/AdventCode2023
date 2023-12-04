#include <cmath>
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
    Game(const std::string &card) {
        std::vector<std::string> tokens1, tokens2;
        auto idx = card.find(':', 0);
        auto game = card.substr(idx+1); 
        auto split = game.find('|', 0);
        auto winning = game.substr(0, split);
        auto mine = game.substr(split+1);
        tokenize(winning, tokens1, " ");
        tokenize(mine, tokens2, " ");

        count = 0;
        for(const auto &win : tokens1) {
            for(const auto &my : tokens2) {
                if(win == my) count++;
            }
        }

        score = count ? static_cast<size_t>(std::pow(2, count-1)) : 0;
    }

    size_t count;
    size_t score;
};

size_t TotalCards(const std::vector<Game> &games) {
    std::vector<size_t> cards(games.size(), 1);

    for(size_t i = 0; i < games.size(); ++i) {
        size_t ncards = cards[i];
        for(size_t j = 0; j < games[i].count; ++j) {
            cards[i+j+1] += ncards; 
        }
    }

    return std::accumulate(cards.begin(), cards.end(), 0ul);
}

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

    auto sum_func = [](size_t a, Game b) {
        return a + (b.score);
    };

    auto sum = std::accumulate(games.begin(), games.end(), 0, sum_func);
    std::cout << "PartA: " << sum << std::endl;
    std::cout << "PartB: " << TotalCards(games) << std::endl;

    return 0;
}
