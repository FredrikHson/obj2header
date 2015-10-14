#include <stdio.h>
#include <getopt.h>
#include <ctype.h>
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
 * unsigned int ${filename}_triangles[]
 * unsigned int ${filename}_numVerts
 * unsigned int ${filename}_numUvs
 * unsigned int ${filename}_numTriangles
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
                printf("make sure to triangulate the model on export\n");
                return 0;
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

    char variablepart[FILENAME_MAX] = {0};
    snprintf(variablepart, FILENAME_MAX, "%s", inputfilename);
    int i = 0;

    while(i < FILENAME_MAX)
    {
        if(variablepart[i] == 0)
        {
            break;
        }
        else if(variablepart[i] == '/')
        {
            variablepart[i] = '_';
        }
        else if(variablepart[i] == '.')
        {
            variablepart[i] = '_';
        }

        variablepart[i] = tolower(variablepart[i]);
        i++;
    }

    fprintf(output, "unsigned int %s_numverts = %i;\n\n", variablepart, model->numverts);
    fprintf(output, "float %s_verts[%i] =\n{\n", variablepart, model->numverts * 3);

    for(i = 0; i < model->numverts - 1; i++)
    {
        fprintf(output, "    %f, %f, %f,\n", model->verts[i].x, model->verts[i].y, model->verts[i].z);
    }

    fprintf(output, "    %f, %f, %f\n", model->verts[i].x, model->verts[i].y, model->verts[i].z);

    fprintf(output, "};\n");

    if(outputuvs)
    {
        fprintf(output, "float %s_verts[%i] =\n{\n", variablepart, model->numuvs * 2);

        for(i = 0; i < model->numuvs - 1; i++)
        {
            fprintf(output, "    %f, %f,\n", model->uvs[i].x, model->uvs[i].y);
        }

        fprintf(output, "    %f, %f\n", model->uvs[i].x, model->uvs[i].y);
        fprintf(output, "};\n");

    }

    if(output != stderr)
    {
        fclose(output);
    }

    return 0;
}
