#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define BIT(n) (1<<(n))

char *byte2bin(uint8_t n, char *binstr) 
{
    // one element per bit (0/1)
    uint8_t binary[8];

    int i = 0;
    for(i = 0; i < 8; i++) {
        binary[i] = n % 2;
        n = n / 2;
    }

    // printing binary array in reverse order
    for (int j = 7, k= 0; j >= 0; j--, k++)
        sprintf(binstr + k, "%d", binary[j]);
        
	return binstr;
}

void print_usage(char *name) {
	printf("Usage: %s <action> <byte> <bit no>\n", name);
	printf("\tWhere: <action> one of 'h'|'l'|'r'|'s'|'t'\n"
		"\t\t <byte> 8-bit integer in hexadecimal\n"
		"\t\t <bit no> of bit to operate on\n");
}

int main(int argc, char *argv[])
{
    char a;	// action: 'h', 'l', 'r', 's', 't'
    unsigned long n; // value to convert must be smaller than 256
    int bit; // bit to operate on: must be between 0 an 7
    char binary[9]; // array for binary representation of n,
					//  remember that in C strings are terminated with a 0
 
	// Validate command line arguments
    if(argc != 4){ 
        printf("Please indicate the right ammount of arguments.");
        return -1;
    }

    if (argv[1][1] != 0){ 
        printf("Please use just a character for the indicated action");
        return -1;
    }
    a = argv[1][0];

    n = stoul(argv[2]);
    if (n > 126){
        printf("Please enter a number smaller than 126");
        return -1;
    }
    n = (uint8_t) n;

    bit = stoi(argv[3]);
    if (0 > bit || bit > 7){
        printf("Please enter a number between 0 and 7 for the bit value");
        return -1;
    }

	// Print to stdout the binary representation of n
    printf("The binary representation of 'n'(%lu) is %s",n,byte2bin(n,binary));
	
	// Do what the user asked and print the result

    uint8_t test;
    switch(a){
        case 'h':
            test = n & BIT(bit);
            if (test == 0) printf("The bit in position %d isn't 1",bit);
            else printf("The bit in position %d is 1",bit);
            break;
        case 'l':
            test = (n & BIT(bit));
            if(test == 0)printf("The bit in position %d is 0",bit);
            else printf("The bit in position %d isn't 0",bit);
            break;
        case 'r':
            test = n & (~BIT(bit));
            printf("After reseting the bit in position %d to 0, the byte is represented by %s",bit,byte2bin(test,binary));
            break;
        case 's':
            test = n | BIT(bit);
            printf("After setting the bit in position %d to 1, the byte is represented by %s",bit,byte2bin(test,binary));
            break;
        case 't':
            test = n ^ BIT(bit);            
            printf("After toggling the bit in position %d, the byte is represented by %s",bit,byte2bin(test,binary));
            break;
    }
	
    return 0;
}