//
//  RSA.c
//  Kursa4
//
//  Created by Коля on 31/05/2019.
//  Copyright © 2019 Коля. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

bool genKeys(const char * prkeyfname, const char * pubkeyfname, const char * pem_password)
{
    /* указатель на структуру для хранения ключей */
    RSA * rsa = NULL;
    int key_length = 2048; /* длина ключа в битах */
    FILE *priv_key_file = NULL, *pub_key_file = NULL;
    /* контекст алгоритма шифрования */
    const EVP_CIPHER *cipher = NULL;
    
    if((priv_key_file = fopen(prkeyfname, "wb"))==NULL)
        error (prkeyfname);
    if((pub_key_file = fopen(pubkeyfname, "wb"))==NULL)
        error (pubkeyfname);
    /* Генерируем ключи */
    rsa = RSA_generate_key(key_length, RSA_F4, NULL, NULL);
    
    /* Формируем контекст алгоритма шифрования */
    OpenSSL_add_all_ciphers();
    cipher = EVP_get_cipherbyname("bf-ofb");
    
    /* Получаем из структуры rsa открытый и секретный ключи и сохраняем в файлах.
     * Секретный ключ шифруем с помощью парольной фразы "hello"
     */
    PEM_write_RSAPrivateKey(priv_key_file, rsa, cipher,
                            NULL, 0, NULL, pem_password);
    PEM_write_RSAPublicKey(pub_key_file, rsa);
    
    /* Освобождаем память, выделенную под структуру rsa */
    RSA_free(rsa);
    
    fclose (priv_key_file);
    fclose (pub_key_file);
    return true;
}

int encryptFile(const char * pubkeyfname, const char * infilename, const char * outfilename)
{
    /* стктура для хранения открытого ключа */
    RSA * pubKey = NULL;
    unsigned char *ptext, *ctext;
    FILE * pub_key_file = NULL;
    
    /* Открываем входной и создаем выходной файлы */
    int inf = open(infilename, O_RDWR);
    if (inf==-1)
        error (infilename);
    int outf = open(outfilename,
                    O_CREAT|O_TRUNC|O_RDWR, 0600);
    if (outf==-1)
        error (outfilename);
    /* Считываем открытый ключ */
    if((pub_key_file = fopen(pubkeyfname, "rb"))==NULL)
        error (pubkeyfname);
    pubKey = PEM_read_RSAPublicKey(pub_key_file, NULL, NULL, NULL);
    if (!pubKey)
    {
        error ("Error reading public key\n");
    }
    /* Определяем длину ключа */
    int key_size = RSA_size(pubKey);
    ptext = malloc(key_size);
    ctext = malloc(key_size);
    
    /* Шифруем содержимое входного файла */
    while(1) {
        int inlen = (int)read(inf, ptext, key_size - 11);
        if(inlen <= 0) break;
        
        int outlen = RSA_public_encrypt(inlen, ptext, ctext, pubKey, RSA_PKCS1_PADDING);
        if(outlen != RSA_size(pubKey)) error("Error encrypting file");
        
        write(outf, ctext, outlen);
    }
    close(inf);
    close(outf);
    fclose(pub_key_file);
    return 1;
}

int decryptFile(const char * prkeyfname, const char * infilename, const char * outfilename)
{
    RSA *privKey = NULL;
    FILE *priv_key_file;
    unsigned char *ptext, *ctext;
    
    /* Открываем входной и создаем выходной файл */
    int inf = open(infilename, O_RDWR);
    if (inf==-1)
        error (infilename);
    int outf = open(outfilename, O_CREAT|O_TRUNC|O_RDWR, 0600);
    if (outf==-1)
        error (outfilename);
    /* Открываем ключевой файл и считываем секретный ключ */
    if((priv_key_file = fopen(prkeyfname, "rb"))==NULL)
        error (prkeyfname);
    
    privKey = PEM_read_RSAPrivateKey(priv_key_file, NULL, NULL, NULL);
    if (!privKey)
    {
        error ("Error reading private key\n");
    }
    /* Определяем размер ключа */
    int key_size = RSA_size(privKey);
    ctext = malloc(key_size);
    ptext = malloc(key_size);
    
    /* Дешифруем файл */
    while(1) {
        int inlen = (int)read(inf, ctext, key_size);
        if(inlen <= 0) break;
        
        int outlen = RSA_private_decrypt(inlen, ctext, ptext, privKey, RSA_PKCS1_PADDING);
        if(outlen < 0) error("Error decrypting file");
        
        write(outf, ptext, outlen);
    }
    close (inf);
    close (outf);
    fclose (priv_key_file);
    return 1;
}
