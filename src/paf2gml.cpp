#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cmath>

struct ReadNode {
    int id;
    std::string name, species;

    ReadNode(int id, std::string name, std::string species) : id(id), name(name), species(species) {}

    friend std::ostream& operator<<(std::ostream&os, ReadNode n)
    {
        os << "\tnode\n\t[\n\t\tid " << n.id << "\n\t\tname \"" << n.name << "\"\n\t\tspecies \"" << n.species << "\"\n\t]";
        return os;
    }
};

struct ReadEdge {
    int qid, tid, qlen, tlen, nalign, nblock, cross;
    char strand;
    float dv;

    ReadEdge(int qid, int tid, int qlen, int tlen, int nalign, int nblock, char strand, float dv, int cross) :
        qid(qid), tid(tid), qlen(qlen), tlen(tlen), nalign(nalign), nblock(nblock), strand(strand), dv(dv), cross(cross) {}

    friend std::ostream& operator<<(std::ostream& os, ReadEdge e)
    {
        os << "\tedge\n\t[\n\t\tsource " << e.qid << "\n\t\ttarget " << e.tid << "\n\t\tslen " << e.qlen
           << "\n\t\ttlen " << e.tlen << "\n\t\tnalign " << e.nalign << "\n\t\tnblock " << e.nblock
           << "\n\t\tstrand \"" << e.strand << "\"\n\t\tcross " << e.cross << "\n\t\tdv " << e.dv << "\n\t]";
        return os;
    }
};

int main(int argc, char *argv[])
{
    if (argc != 5) {
        std::cerr << "usage: ./parse_overlaps <minimap2.paf> <read.vs.species.tsv> [max overhang length] [overhang to mapping length ratio]" << std::endl;
        return 1;
    }

    std::ifstream overlap_stream(argv[1]);
    std::ifstream readmap_stream(argv[2]);
    const int overhang_length = atoi(argv[3]);
    const double ratio = atof(argv[4]);

    bool write_through = (fabs(ratio) < 1e-5 || overhang_length == 0);

    ///if (ratio <= 0 || ratio > 1) {
    ///    std::cerr << "error: overhang to mapping ratio must be in the interval (0, 1]" << std::endl;
    ///    return 1;
    ///}

    std::unordered_map<std::string, int> idmap;
    std::unordered_map<int, std::string> readmap;

    std::vector<ReadNode> nodes;
    std::vector<ReadEdge> edges;

    std::string line, readname, speciesname;

    int id = 0;
    while (getline(readmap_stream, line)) {
        std::istringstream record(line);
        record >> readname >> speciesname;
        idmap[readname] = id;
        readmap[id] = speciesname;
        nodes.push_back(ReadNode(id, readname, speciesname));
        ++id;
    }

    readmap_stream.close();

    std::string qreadname, treadname, divergence;
    int l1, l2, b1, b2, e1, e2, nalign, nblock;
    char strand;
    float dv;

    std::string dv_delim = "dv:f:";

    while (getline(overlap_stream, line)) {
        std::istringstream record(line);
        record >> qreadname >> l1 >> b1 >> e1 >> strand >> treadname >> l2 >> b2 >> e2 >> nalign >> nblock;
        std::istringstream(line.substr(line.find(dv_delim) + dv_delim.size(), line.size())) >> dv;

        int overhang = std::min(b1, b2) + std::min(l1 - e1, l2 - e2);
        int maplen = std::max(e1 - b1, e2 - b2);

        if (!write_through && overhang > std::min((double)overhang_length, (maplen+0.0)*ratio)) continue;

        int qid = idmap[qreadname];
        int tid = idmap[treadname];
        int cross = (readmap[qid] == readmap[tid]) ? 0 : 1;
        edges.push_back(ReadEdge(qid, tid, l1, l2, nalign, nblock, strand, dv, cross));
    }

    overlap_stream.close();

    std::cout << "graph\n[\n\tdirected 0\n";

    for (auto itr = nodes.begin(); itr != nodes.end(); itr++)
        std::cout << *itr << std::endl;

    for (auto itr = edges.begin(); itr != edges.end(); itr++)
        std::cout << *itr << std::endl;

    std::cout << "]" << std::endl;


    return 0;
}
