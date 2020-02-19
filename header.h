//
//  header.h
//  Kursa4
//
//  Created by Коля on 31/05/2019.
//  Copyright © 2019 Коля. All rights reserved.
//

#ifndef header_h
#define header_h
#define MAX_CAESAR_OFFSET 10000
#define MIN_CAESAR_OFFSET 0
#define WRONG_USAGE_MESSAGE "Usage:\nGenerate RSA keys: Kursa4 -g private_key_file_name pubic_key_file_name\nEncrypt using RSA: Kursa4 -e public_key_file_name input_file_name output_file_name\nDecrypt using RSA: Kursa4 -d private_key_file_name input_file_name output_file_name\nEncrypt using caesar: Kursa4 -c input_file_name output_file_name offset\nDecrypt using caesar: Kursa4 -u input_file_name output_file_name offset\n"

void error(const char * info);
void wrongUsage(void);

#endif /* header_h */
