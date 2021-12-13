#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "usage: filter_overlaps <max overhang length (int)> <overhang to mapping length ratio (double)>" << std::endl;
        return -1;
    }

    int overhang_length = std::atoi(argv[1]);
    double mapping_ratio = std::atof(argv[2]);

    if (mapping_ratio <= 0 || mapping_ratio > 1) {
        std::cerr << "error: mapping ratio must be in the interval (0, 1]" << std::endl;
        return -1;
    }

    char strand;
    std::string line, qreadname, treadname;
    int l[2], b[2], e[2];

    while (std::getline(std::cin, line)) {
        std::stringstream iss(line);
        iss >> qreadname >> l[0] >> b[0] >> e[0] >> strand >> treadname >> l[1] >> b[1] >> e[1];
        int overhang = std::min(b[0], b[1]) + std::min(l[0] - e[0], l[1] - e[1]);
        int maplen = std::max(e[0] - b[0], e[1] - b[1]);
        if (overhang <= std::min(static_cast<double>(overhang_length), static_cast<double>(maplen)*mapping_ratio))
            std::cout << line << std::endl;
    }

    return 0;
}
