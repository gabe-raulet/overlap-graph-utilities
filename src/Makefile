all: filter_overlaps paf2gml

filter_overlaps: filter_overlaps.c
	clang -O2 -o $@ $<

paf2gml: paf2gml.cpp
	clang++ -O2 -std=c++11 -o $@ $<

clean:
	rm -f filter_overlaps paf2gml
