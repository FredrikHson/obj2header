#include <stdio.h>
#include <getopt.h>
#include "fast_obj_loader.h"
#include "fastdynamic2.h"

char inputfilename[FILENAME_MAX] = {0};
char outputfilename[FILENAME_MAX] = {0};

/*
struct Frame
{
    float* vertPos; // *3
};
struct Model
{
    struct Frame* frames;
    float* vertUVs; // *2
    unsigned int numVerts;
    unsigned int numFrames;
    unsigned int* index; // *3 in size
};
*/
/*
 * format spec
 * float ${filename}_verts[]
 * float ${filename}_vertuvs[]
 * unsigned int ${filename}_numVerts
 */

int main(int argc, char* argv[])
{

    int c;
    bool outputuvs = true;

    while((c = getopt(argc, argv, "i:o:hu")) != -1)
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

            case 'u':
            {
                outputuvs = false;
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

    fprintf(output, "unsigned int %s_numverts = %i;\n\n", inputfilename, model->numverts);
    fprintf(output, "float %s_verts[%i] =\n{\n", inputfilename, model->numverts * 3);
    int i = 0;

    for(i = 0; i < model->numverts - 1; i++)
    {
        fprintf(output, "    %f, %f, %f,\n", model->verts[i].x, model->verts[i].y, model->verts[i].z);
    }

    fprintf(output, "    %f, %f, %f\n", model->verts[i].x, model->verts[i].y, model->verts[i].z);

    fprintf(output, "};\n");

    if(output != stderr)
    {
        fclose(output);
    }

    return 0;
}
