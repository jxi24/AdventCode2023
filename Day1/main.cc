#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>


std::vector<std::string> ParseFile(const std::string &filename) {
    std::string line;
    std::vector<std::string> lines;
    std::ifstream file(filename);
    while(std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

int ParseLineA(const std::string &line) {
    int first = -1;
    int last = -1;
    for(const auto &c : line) {
        int val = c - '0';
        if(val >= 0 && val <= 9) {
            if(first < 0) first = val;
            else last = val;
        }
    }

    if(last < 0) last = first;

    return first*10 + last;
}

int ParseLineB(std::string line) {
    std::map<std::string_view, std::string_view> digits = {
        {"one", "on1ne"},
        {"two", "tw2wo"},
        {"three", "thre3hree"},
        {"four", "fou4our"},
        {"five", "fiv5ive"},
        {"six", "si6ix"},
        {"seven", "seve7even"},
        {"eight", "eigh8ight"},
        {"nine", "nin9ine"}};

    // Replace spelled out first
    for(const auto &[digit, value] : digits) {
        std::size_t idx = 0;
        while((idx = line.find(digit, idx)) != std::string::npos) {
            line.replace(idx, digit.size(), value);
            idx += value.size();
        }
    }
    int first = -1;
    int last = -1;
    for(const auto &c : line) {
        int val = c - '0';
        if(val >= 0 && val <= 9) {
            if(first < 0) first = val;
            else last = val;
        }
    }

    if(last < 0) last = first;

    return first*10 + last;
}


int main(int argc, char *argv[]) {
    if(argc != 2) {
        std::cout << "Requires filename!" << std::endl;
        return -1;
    }
    std::string filename = argv[1];
    auto lines = ParseFile(filename);
    int sum = 0;
    int sumB = 0;
    for(const auto &line : lines) {
        sum += ParseLineA(line);
        sumB += ParseLineB(line);
    }
    std::cout << "PartA: " << sum << std::endl;
    std::cout << "PartB: " << sumB << std::endl;

    return 0;
}
