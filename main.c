//
//  main.c
//  Kursa4
//
//  Created by Коля on 27/04/2019.
//  Copyright © 2019 Коля. All rights reserved.
//

#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <wchar.h>
#include <locale.h>
#include <unistd.h>
#include "header.h"
//#include "Caesar.c"
//#include "RSA.c"


void error(const char * info)
{
    if (errno)
        printf("%s: %s\n",info,
           strerror(errno));
    else
        printf("%s", info);
    exit (EXIT_FAILURE);
}

void wrongUsage()
{
    printf(WRONG_USAGE_MESSAGE);
    exit(EXIT_FAILURE);
}

int main(int argc, const char * argv[])
{
    
    OpenSSL_add_all_algorithms();
    OpenSSL_add_all_ciphers();
    int offset;
    setlocale(LC_CTYPE, "");
    
    if ((argc > 1) && (argv[1][0] == '-'))
    {
        switch (argv[1][1])
        {
            case 'e':
                if (!argv[2]||!argv[3]||!argv[4])
                    wrongUsage(); 
                encryptFile(argv[2], argv[3], argv[4]);
                break;
            
            case 'd':
                if (!argv[2]||!argv[3]||!argv[4])
                    wrongUsage();
               decryptFile(argv[2], argv[3], argv[4]);
                break;
            case 'g':
                if (!argv[2]||!argv[3])
                    wrongUsage();
                genKeys(argv[2], argv[3], getpass("Enter PEM pass phrase:"));
                break;
            
            case 'c':
                if (!argv[2]||!argv[3]||!argv[4])
                    wrongUsage();
                offset = atoi(argv[4]);
                if (!offset||offset>MAX_CAESAR_OFFSET||offset<MIN_CAESAR_OFFSET)
                {
                    printf("Wrong offset variable\n");
                    wrongUsage();
                }
                encryptCaesar(argv[2], argv[3], offset);
                break;
                
            case 'u':
                if (!argv[2]||!argv[3]||!argv[4])
                    wrongUsage();
                offset = atoi(argv[4]);
                if (!offset||offset>MAX_CAESAR_OFFSET||offset<MIN_CAESAR_OFFSET)
                {
                    printf("WrЯong offset variable\n");
                    wrongUsage();
                }
                decryptCaesar(argv[2], argv[3], offset);
                break;
                
            default:
                printf ("illegal option -- %c\n", argv[1][1]);
                wrongUsage();
        }
    }
    else
    {
        wrongUsage();
    }
    
    return 0;
}
