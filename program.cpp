/***
 * Simple Encryption Program:
 * Encrypts and decrypts a file using a key.
 * This uses the easily reversible XOR operation
 * Encryption: plain ^ key = enc 
 * Decryption: enc ^ key = plain 
 */

#include <iostream>
#include <fstream>
#include <getopt.h>
#include <cstring>

using namespace std;

/**
 * Encrypts the byte contents of an input buffer and writes the result
 * to an output buffer.
 * @param in the input buffer
 * @param out the output buffer
 * @param n length of the contents in the input buffer to be processed
 * @param key encryption key 
 */
void encrypt(char* in, char* out, int n, char* key);

/**
 * Prepares the input and output buffers from the file streams and
 * calls the encrypt function
 * @param in file stream to be read and encrypted
 * @param out file stream to be written to with encrypted contents
 * @param inbuff input file buffer
 * @param outbuff output file buffer
 * @param key encryption key
 */ 
void encryption(ifstream &in, ofstream &out, char* inbuff, char* outbuff, char* key);

/**
 * Decrypts the byte contents of an input buffer and writes the result
 * to an output buffer.
 * @param in the input buffer
 * @param out the output buffer
 * @param n length of the contents in the input buffer to be processed
 * @param key decryption key 
 */
void decrypt(char* in, char* out, int n, char* key);

/**
 * Prepares the input and output buffers from the file streams and
 * calls the dncrypt function
 * @param in file stream to be read and decrypted
 * @param out file stream to be written to with decrypted contents
 * @param inbuff input file buffer
 * @param outbuff output file buffer
 * @param key decryption key
 */ 
void decryption(ifstream &in, ofstream &out, char* inbuff, char* outbuff, char* key);

/**
 * Prints error message
 * @param message error message
 */ 
void printErr(const char* message);

/**
 * Displays the help message
 */ 
void printusage(char* progname);

const int BUFFER_SIZE = 100000; // This buffer seems kinda small, can be improved
const int MAX_KEY_SIZE = 64;
const int MIN_KEY_SIZE = 8;
const int ENCRYPT_FLAG = 1;
const int DECRYPT_FLAG = 2; 

int main(int argc, char** argv) {
    int opt; // for getopt
    int actionFlag = 0;

    char inbuffer[BUFFER_SIZE];
    char outbuffer[BUFFER_SIZE];
    char* key;
    ifstream infile;
    ofstream outfile;
    
    
    // process command line args
    while ((opt = getopt(argc, argv, "hedk:i:o:")) != -1) {
        switch (opt) {
        case 'e':
            if (actionFlag) {
                printErr("Cannot set encrypt flag when decrypt is set.");
            }
            actionFlag = ENCRYPT_FLAG;
            break;
        case 'd':
            if (actionFlag) {
                printErr("Cannot set decrypt flag when encrypt is set.");
            }
            actionFlag = DECRYPT_FLAG;
            break;
        case 'i':
            infile = ifstream(optarg, std::ios::binary);
            if (!infile.good()) {
                printErr("Cannot access input file.");
            }
            break;
        case 'o':
            outfile = ofstream(optarg, std::ios::binary);
            if (!outfile.good()) {
                printErr("Cannot write to output file.");
            }
            break;
        case 'k': 
            key = optarg;
            if ( strlen(key) < MIN_KEY_SIZE || strlen(key) > MAX_KEY_SIZE) {
                printErr("Key string must be minimum of 8 chars and maximum of 64 chars");
            }
            break;
        case 'h':
            printusage(argv[0]);
            exit(EXIT_SUCCESS);
        default: // '?' 
            printusage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Handle errors not caught in getopt loop
    // if e | d not selected
    if (actionFlag == 0) {
        printErr("No action selected.");
    }

    //enforce key length constraint
    if (strlen(key) < MIN_KEY_SIZE || strlen(key) > MAX_KEY_SIZE) {
        printErr("Key string must be minimum of 8 chars and maximum of 64 chars");
    }

    
    if (actionFlag == ENCRYPT_FLAG)
    {
        encryption(infile, outfile, inbuffer, outbuffer, key);
    } else {
        decryption(infile, outfile, inbuffer, outbuffer, key);
    }
    
    infile.close();
    outfile.close();
    return 0;
}

void encryption(ifstream &in, ofstream &out, char* inbuff, char* outbuff, char* key) {
    while (in.good()) {
        in.read(inbuff, BUFFER_SIZE);
        int read = in.gcount();
        encrypt(inbuff, outbuff, read, key);
        out.write(outbuff, read);
    }
} 

void decryption(ifstream &in, ofstream &out, char* inbuff, char* outbuff, char* key) {
    while (in.good()) {
        in.read(inbuff, BUFFER_SIZE);
        int read = in.gcount();
        decrypt(inbuff, outbuff, read, key);
        out.write(outbuff, read);
    }
} 

void encrypt(char* in, char* out, int n, char* key) {
    int keyindex = 0;
    for(int i = 0; i < n; i++, keyindex++) {
        char k = key[keyindex];
        if (k) {
            out[i] = (in[i] ^ k) + keyindex;
        } else {
            keyindex = 0;
            out[i] = (in[i] ^ key[keyindex]) + keyindex;
        }
    }
}

void decrypt(char* in, char* out, int n, char* key) {
    int keyindex = 0;
    for(int i = 0; i < n; i++, keyindex++) {
        char k = key[keyindex];
        if (k) {
            out[i] = (in[i] - keyindex) ^ k;
        } else {
            keyindex = 0;
            out[i] = (in[i] - keyindex) ^ key[keyindex];
        }
    }
}


void printErr(const char* message) {
    cerr << "Err: " << message << endl;
    exit(EXIT_FAILURE);
}

void printusage(char* progname) {
    cout << "Usage: "<< progname << " -e|d [encrypt | decrypt] -i [input file] -o [output file] -k [key]\n"
    << "\te: encrypt; Cannot be selected if <d> is selected\n"
    << "\td: decrypt; Cannot be selected if <e> is selected\n"
    << "\ti: path to the source / input file\n"
    << "\to: path to the destination / output file\n"
    << "\tk: key string. Minimum of 8 chars. Maximum of 64 chars. No spaces"
    << endl;
}