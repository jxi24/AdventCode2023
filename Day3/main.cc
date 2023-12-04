#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Schematic {
    std::vector<std::pair<size_t, size_t>> numbers_loc;
    std::vector<int> numbers;
    std::vector<size_t> symbols, gears;
    size_t width, height;

    Schematic(const std::string &filename) {
        std::ifstream input(filename);
        width = 0;
        height = 0;
        std::string line;
        while(std::getline(input, line)) {
            if(width == 0) width = line.size();

            bool read_number = false;
            std::vector<int> num;
            size_t pos = 0;
            for(size_t i = 0; i < line.size(); ++i) {
                const auto c = line[i];
                int as_num = c - '0';
                if(as_num >= 0 && as_num <= 9) {
                    if(!read_number) {
                        read_number = true; 
                        pos = i;
                    }
                    num.push_back(as_num);
                } else {
                    if(read_number && num.size() > 0) {
                        numbers_loc.push_back({height*width+pos, height*width+i-1});
                        int number = 0;
                        for(const auto n : num) number = number*10 + n;
                        numbers.push_back(number);
                        num.clear();
                        read_number = false;
                    }
                    if(c != '.') symbols.push_back(height*width+i);
                    if(c == '*') gears.push_back(height*width+i);
                }
            }
            if(read_number) {
                numbers_loc.push_back({height*width+pos, height*width+width-1});
                int number = 0;
                for(const auto n : num) number = number*10 + n;
                numbers.push_back(number);
            }
            height++;
        }
    }

    size_t SumParts() const {
        size_t sum = 0;
        for(size_t i = 0; i < numbers_loc.size(); ++i) {
            int x0 = static_cast<int>(numbers_loc[i].first%width);
            int x1 = static_cast<int>(numbers_loc[i].second%width);
            int y0 = static_cast<int>(numbers_loc[i].first/width);

            for(const auto &symbol : symbols) {
                int x = static_cast<int>(symbol%width);
                int y = static_cast<int>(symbol/width);

                // Check up and down
                if(x <= x1 && x >= x0 && ((y-1) == y0 || (y+1) == y0)) {
                    sum += static_cast<size_t>(numbers[i]);
                    break;
                }
                // Check left and right
                else if(y == y0 && ((x-1) == x1 || (x+1) == x0)) {
                    sum += static_cast<size_t>(numbers[i]);
                    break;
                }
                // Check diagonal
                else if(((y-1) == y0 || (y+1) == y0) && ((x-1) == x1 || (x+1) == x0)) {
                    sum += static_cast<size_t>(numbers[i]);
                    break;
                }
            }
        }
        return sum;
    }

    size_t GearRatio() const {
        size_t ratio = 0;
        for(const auto &gear : gears) {
            int x = static_cast<int>(gear%width);
            int y = static_cast<int>(gear/width);

            int gear1 = -1;
            for(size_t i = 0; i < numbers_loc.size(); ++i) {
                int x0 = static_cast<int>(numbers_loc[i].first%width);
                int x1 = static_cast<int>(numbers_loc[i].second%width);
                int y0 = static_cast<int>(numbers_loc[i].first/width);

                // Check up and down
                if(x <= x1 && x >= x0 && ((y-1) == y0 || (y+1) == y0)) {
                    if(gear1 < 0) gear1 = numbers[i];
                    else {
                        ratio += static_cast<size_t>(numbers[i]*gear1);
                        break;
                    }
                }
                // Check left and right
                else if(y == y0 && ((x-1) == x1 || (x+1) == x0)) {
                    if(gear1 < 0) gear1 = numbers[i];
                    else {
                        ratio += static_cast<size_t>(numbers[i]*gear1);
                        break;
                    }
                }
                // Check diagonal
                else if(((y-1) == y0 || (y+1) == y0) && ((x-1) == x1 || (x+1) == x0)) {
                    if(gear1 < 0) gear1 = numbers[i];
                    else {
                        ratio += static_cast<size_t>(numbers[i]*gear1);
                        break;
                    }
                }
                
            }
        }
        return ratio;
    }
};

int main(int argc, char *argv[]) {
    if(argc != 2) {
        std::cout << "Requires a file input!\n";
        return -1;
    }

    Schematic schematic(argv[1]);
    std::cout << "Part A: " << schematic.SumParts() << std::endl;
    std::cout << "Part B: " << schematic.GearRatio() << std::endl;
}
