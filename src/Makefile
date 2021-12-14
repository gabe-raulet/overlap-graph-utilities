all: filter_overlaps paf2gml subsample

filter_overlaps: filter_overlaps.c
	gcc -O2 -o $@ $<

paf2gml: paf2gml.cpp
	g++ -O2 -std=c++11 -o $@ $<

subsample: subsample.cpp
	g++ -O2  -std=c++11 -o $@ $<

clean:
	rm -f filter_overlaps paf2gml subsample
