#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <cassert>

int main(int argc, char *argv[])
{
    /*  !!!! IMPORTANT !!!
     *  ------------------
     * For simplicity, assume each sequence in a fasta file is on one line.
     *
     *     "seqtk seq -l0 original.fa > in.fa"
     */
    if (argc != 3) {
        std::cerr << "usage: ./subsample <in.fa> <idmap.tsv>" << std::endl;
        return -1;
    }

    std::string sequences_filename(argv[1]);
    std::string idmap_filename(argv[2]);

    std::unordered_map<std::string, std::string> readmap;

    std::string line, readname, species;
    std::ifstream idmap_stream(idmap_filename, std::ifstream::in);

    while (getline(idmap_stream, line)) {
        std::istringstream iss(line);
        iss >> readname >> species;
        readmap.insert(std::make_pair(readname, species));
    }

    idmap_stream.close();

    std::ifstream sequence_stream(sequences_filename, std::ifstream::in);

    while (getline(sequence_stream, line)) {
        if (line[0] == '>') {
            readname = line.substr(1, line.size());
            auto entry = readmap.find(readname);
            if (entry != readmap.end()) {
                std::cout << ">" << readname << "|" << entry->second << "\n";
                assert(getline(sequence_stream, line));
                std::cout << line << std::endl;
            }
        }
    }

    sequence_stream.close();
    return 0;
}
