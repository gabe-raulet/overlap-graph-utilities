INSTALL_DIR=/global/homes/g/gabeh98/software/bin

all: filter_overlaps paf2gml subsample

install: filter_overlaps paf2gml subsample
	cp $^ $(INSTALL_DIR)

filter_overlaps: filter_overlaps.c
	gcc -O2 -o $@ $<

paf2gml: paf2gml.cpp
	g++ -O2 -std=c++11 -o $@ $<

subsample: subsample.cpp
	g++ -O2  -std=c++11 -o $@ $<

clean:
	rm -f filter_overlaps paf2gml subsample
