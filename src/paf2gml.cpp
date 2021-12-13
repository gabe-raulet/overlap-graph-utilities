#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

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
    std::vector<ReadEdge> edges;
    std::unordered_map<std::string, int> read_idxs;
    std::string line, qreadname, treadname, divergence, qid, tid, qspecies, tspecies, dummy;
    int idx, nalign, nblock, cross, qlen, tlen;
    char strand;
    float dv;

    std::cout << "graph\n[\n\tdirected 0\n";

    idx = 0;

    while (getline(std::cin, line)) {
        std::istringstream record(line);

        record >> qreadname
               >> qlen
               >> dummy >> dummy
               >> strand
               >> treadname
               >> tlen
               >> dummy >> dummy
               >> nalign
               >> nblock
               >> dummy >> dummy >> dummy >> dummy
               >> divergence;

        dv = std::atof(divergence.substr(5, divergence.size()).c_str()); /* magic number 5 is length of "dv:f:" */

        int qsplit = qreadname.find("|");
        int tsplit = treadname.find("|");

        qid = qreadname.substr(0, qsplit);
        tid = treadname.substr(0, tsplit);

        qspecies = qreadname.substr(qsplit+1, qreadname.size());
        tspecies = treadname.substr(tsplit+1, treadname.size());

        cross = (qspecies == tspecies)? 0 : 1;

        if (read_idxs.find(qid) == read_idxs.end()) {
            std::cout << "\tnode\n\t[\n\t\tid " << idx << "\n\t\tname \"" << qid << "\"\n\t\tspecies \"" << qspecies << "\"\n\t]\n";
            read_idxs.insert(std::make_pair(qid, idx++));
        }

        if (read_idxs.find(tid) == read_idxs.end()) {
            std::cout << "\tnode\n\t[\n\t\tid " << idx << "\n\t\tname \"" << tid << "\"\n\t\tspecies \"" << qspecies << "\"\n\t]\n";
            read_idxs.insert(std::make_pair(tid, idx++));
        }

        edges.push_back(ReadEdge(read_idxs.find(qid)->second, read_idxs.find(tid)->second, qlen, tlen, nalign, nblock, strand, dv, cross));

    }

    for (auto itr = edges.begin(); itr != edges.end(); itr++)
        std::cout << *itr << std::endl;

    std::cout << "]" << std::endl;

    return 0;
}

