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
    unsigned int* indexuv; // *3 in size
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
    bool outputnormals = true;

    while((c = getopt(argc, argv, "i:o:hun")) != -1)
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

            case 'n':
            {
                outputnormals = false;
                break;
            }

            case 'h':
            {
                printf("-i   input file\n");
                printf("-o   output file\n");
                printf("-u   do not output uvs\n");
                printf("-n   do not output normals\n");
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

    fprintf(output, "unsigned int %s_numverts     = %i;\n", variablepart, model->numverts);

    if(outputuvs && model->numuvs != 0)
    {
        fprintf(output, "unsigned int %s_numuvs       = %i;\n", variablepart, model->numuvs);
    }

    if(outputnormals && model->numnormals != 0)
    {
        fprintf(output, "unsigned int %s_numnormals   = %i;\n", variablepart, model->numnormals);
    }

    fprintf(output, "unsigned int %s_numtriangles = %i;\n\n", variablepart, model->numfaces);
    fprintf(output, "float %s_verts[%i] =\n{\n", variablepart, model->numverts * 3);

    for(i = 0; i < model->numverts - 1; i++)
    {
        fprintf(output, "    %f, %f, %f,\n", model->verts[i].x, model->verts[i].y, model->verts[i].z);
    }

    fprintf(output, "    %f, %f, %f\n", model->verts[i].x, model->verts[i].y, model->verts[i].z);

    fprintf(output, "};\n");

    if(outputuvs && model->numuvs != 0)
    {
        fprintf(output, "float %s_uvs[%i] =\n{\n", variablepart, model->numuvs * 2);

        for(i = 0; i < model->numuvs - 1; i++)
        {
            fprintf(output, "    %f, %f,\n", model->uvs[i].x, model->uvs[i].y);
        }

        fprintf(output, "    %f, %f\n", model->uvs[i].x, model->uvs[i].y);
        fprintf(output, "};\n");

    }

    if(outputnormals && model->numnormals != 0)
    {
        fprintf(output, "float %s_normals[%i] =\n{\n", variablepart, model->numnormals * 2);

        for(i = 0; i < model->numnormals - 1; i++)
        {
            fprintf(output, "    %f, %f, %f,\n", model->normals[i].x, model->normals[i].y, model->normals[i].z);
        }

        fprintf(output, "    %f, %f, %f\n", model->normals[i].x, model->normals[i].y, model->normals[i].z);
        fprintf(output, "};\n");

    }


    fprintf(output, "float %s_posindex[%i] =\n{\n", variablepart, model->numfaces * 3);

    for(i = 0; i < model->numfaces - 1; i++)
    {
        fprintf(output, "    %u, %u, %u,\n",
                model->faces[i].verts[0] - 1,
                model->faces[i].verts[1] - 1,
                model->faces[i].verts[2] - 1);
    }

    fprintf(output, "    %u, %u, %u\n",
            model->faces[i].verts[0] - 1,
            model->faces[i].verts[1] - 1,
            model->faces[i].verts[2] - 1);
    fprintf(output, "};\n");

    if(outputuvs && model->numuvs != 0)
    {
        fprintf(output, "float %s_uvindex[%i] =\n{\n", variablepart, model->numfaces * 3);

        for(i = 0; i < model->numfaces - 1; i++)
        {
            fprintf(output, "    %u, %u, %u,\n",
                    model->faces[i].uvs[0] - 1,
                    model->faces[i].uvs[1] - 1,
                    model->faces[i].uvs[2] - 1);
        }

        fprintf(output, "    %u, %u, %u\n",
                model->faces[i].uvs[0] - 1,
                model->faces[i].uvs[1] - 1,
                model->faces[i].uvs[2] - 1);
        fprintf(output, "};\n");
    }

    if(outputnormals && model->numnormals != 0)
    {
        fprintf(output, "float %s_normalindex[%i] =\n{\n", variablepart, model->numfaces * 3);

        for(i = 0; i < model->numfaces - 1; i++)
        {
            fprintf(output, "    %u, %u, %u,\n",
                    model->faces[i].normals[0] - 1,
                    model->faces[i].normals[1] - 1,
                    model->faces[i].normals[2] - 1);
        }

        fprintf(output, "    %u, %u, %u\n",
                model->faces[i].normals[0] - 1,
                model->faces[i].normals[1] - 1,
                model->faces[i].normals[2] - 1);
        fprintf(output, "};\n");
    }


    if(output != stderr)
    {
        fclose(output);
    }

    return 0;
}
