#include <stdio.h>
#include <getopt.h>
#include "fast_obj_loader.h"
#include "fastdynamic2.h"

char inputfilename[FILENAME_MAX] = {0};
char outputfilename[FILENAME_MAX] = {0};

int main(int argc, char* argv[])
{

    int c;

    while((c = getopt(argc, argv, "i:o:h")) != -1)
    {
        switch(c)
        {
            case 'i':
            {
                snprintf(inputfilename, FILENAME_MAX, "%s", optarg);
                break;
            }

            case 'o':
            {
                snprintf(outputfilename, FILENAME_MAX, "%s", optarg);
                break;
            }

            case 'h':
            {
                printf("-i   input file\n");
                printf("-o   output file\n");
                break;
            }

            default:
                break;
        }
    }

    obj* model;

    if(inputfilename[0] == 0)
    {
        fprintf(stderr, "no input file\n");
        return 1;
    }
    else
    {
        model = loadObj(inputfilename);
    }

    FILE* output = 0;

    if(outputfilename[0] == 0)
    {
        fprintf(stderr, "printing result to stdout\n");
        output = stdout;
    }
    else
    {
        output = fopen(outputfilename, "wb");
    }

    fprintf(output, "numverts:%i\n", model->numverts);

    if(output != stderr)
    {
        fclose(output);
    }

    return 0;
}
