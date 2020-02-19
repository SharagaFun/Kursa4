//
//  Caesar.c
//  Kursa4
//
//  Created by Коля on 31/05/2019.
//  Copyright © 2019 Коля. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>

bool encryptCaesar (const char * infilename, const char * outfilename, const int offset)
{
    FILE *inf = NULL, *outf = NULL;
    if((inf = fopen(infilename, "r"))==NULL)
        error (infilename);
    if((outf = fopen(outfilename, "w+"))==NULL)
        error (outfilename);
    wchar_t c;
    do
    {
        c = getwc(inf);
        if (!c)
            break;
        c += offset;
        fprintf (outf, "%lc", c);
    }
    while (!feof(inf));
    fclose(inf); fclose(outf);
    return 1;
}

bool decryptCaesar (const char * infilename, const char * outfilename, const int offset)
{
    FILE *inf = NULL, *outf = NULL;
    if((inf = fopen(infilename, "r"))==NULL)
        error (infilename);
    if((outf = fopen(outfilename, "w+"))==NULL)
        error (outfilename);
    wchar_t c;
    do
    {
        c = getwc(inf);
        if (!c)
            break;
        c -= offset;
        fprintf (outf, "%lc", c);
    }
    while (!feof(inf));
    fclose(inf); fclose(outf);
    return 1;
}
