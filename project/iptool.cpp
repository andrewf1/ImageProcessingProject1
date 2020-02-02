#include "../iptools/core.h"
#include <strings.h>
#include "roi.hpp"
#include <string.h>
#include <iostream>
#include <vector>
using namespace std;

#define MAXLEN 256

int main (int argc, char** argv)
{
    image src, tgt;
    FILE *fp;
    char str[MAXLEN];
    char outfile[MAXLEN];
    char *pch;
    if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);        
    }

    // extracting first line
    int num_regions;
    if (fgets(str, MAXLEN, fp) != NULL) {
        pch = strtok(str, " ");
        num_regions = atoi(pch);
        if (num_regions > 3) {
            cout << "Only 3 ROI allowed" << endl;
            exit(1);
        }
    }

    // getting the regions
    vector<roi> regions;
    for (int i = 0; i < num_regions; i++) {
        if (fgets(str, MAXLEN, fp) != NULL) {
            pch = strtok(str, " ");
            int x = atoi(pch);
            pch = strtok(NULL, " ");
            int y = atoi(pch);
            pch = strtok(NULL, " ");
            int sx = atoi(pch);
            pch = strtok(NULL, " ");
            int sy = atoi(pch);
            pch = strtok(NULL, " ");
            int t1 = atoi(pch);
            pch = strtok(NULL, " ");
            int t2 = atoi(pch);
            pch = strtok(NULL, " ");
            int cr = atoi(pch);
            pch = strtok(NULL, " ");
            int cg = atoi(pch);
            pch = strtok(NULL, " ");
            int cb = atoi(pch);
            pch = strtok(NULL, " ");
            int tc = atoi(pch);
            pch = strtok(NULL, " ");
            int dc = atoi(pch);
            pch = strtok(NULL, " ");
            int ws = atoi(pch);
            roi new_region = roi(x, y, sx, sy, t1, t2, cr, cg, cb, 
                tc, dc, ws);
            regions.push_back(new_region);
        }
    }

    while (fgets(str, MAXLEN, fp) != NULL) {
        pch = strtok(str, " ");
        src.read(pch); // creating the source image

        pch = strtok(NULL, " ");
        strcpy(outfile, pch);

        pch = strtok(NULL, " ");
        pch[strlen(pch) - 1] = '\0';
        if (strncasecmp(pch, "double_thresh", MAXLEN) == 0) {
            utility::doubleThreshold(src, tgt, regions);
        }   
        else if (strncasecmp(pch, "color_binar", MAXLEN) == 0) {
            utility::colorBinarization(src, tgt, regions);
        } 
        else if (strncasecmp(pch, "2d_smooth", MAXLEN) == 0) {
            utility::smoothing2d(src, tgt, regions);
        }
        else if(strncasecmp(pch, "1d_smooth", MAXLEN) == 0) {
            utility::smoothing1d(src, tgt, regions);
        }
        else if(strncasecmp(pch, "inc_1d_smooth", MAXLEN) == 0) {
            utility::smoothing1dInc(src, tgt, regions);
        }
        else {
            cout << "No function: " << pch << endl;
            continue;
        }

        tgt.save(outfile);
    }

    fclose(fp);
    return 0;
}