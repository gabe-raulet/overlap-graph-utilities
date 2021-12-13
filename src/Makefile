all: filter_overlaps

filter_overlaps: filter_overlaps.c
	clang -O2 -o $@ $<

clean:
	rm -f filter_overlaps
