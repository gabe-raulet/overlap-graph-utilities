#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define fmin(a, b) (((a) < (b)) ? (a) : (b))
#define fmax(a, b) (((a) < (b)) ? (b) : (a))

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "usage: ./filter_overlaps <input.paf> <max overhang length> <overhang to mapping length ratio]\n");
        return 1;
    }

    const char *input_paf = argv[1];
    const int overhang_length = atoi(argv[2]);
    const double ratio = atof(argv[3]);

    if (ratio <= 0 || ratio > 1) {
        fprintf(stderr, "error: ratio must be in the interval (0, 1]\n");
        return 1;
    }

    FILE *ifp;

    if (!(ifp = fopen(input_paf, "r"))) {
        fprintf(stderr, "error: unable to open '%s'\n", input_paf);
        return 1;
    }

    ssize_t nread;
    size_t size;
    char *lineptr = NULL;
    char processed[2048];

    char qreadname[128], treadname[128], strand, *end;
    int l1, b1, e1, l2, b2, e2, d1, d2, d3;
    while ((nread = getline(&lineptr, &size, ifp) != -1)) {
        strcpy(processed, lineptr);
        end = processed;
        for (int i = 0; i < 9; ++i)
            end = strchr(end, '\t') + 1;
        *end = '\0';
        sscanf(processed, "%s\t%d\t%d\t%d\t%c\t%s\t%d\t%d\t%d",
               qreadname, &l1, &b1, &e1, &strand, treadname, &l2, &b2, &e2);

        int overhang = fmin(b1, b2) + fmin(l1 - e1, l2 - e2);
        int maplen = fmax(e1 - b1, e2 - b2);
        if (overhang <= fmin(overhang_length, (maplen+0.0)*ratio))
            printf("%s", lineptr);
    }

    free(lineptr);
    fclose(ifp);

    return 0;
}
