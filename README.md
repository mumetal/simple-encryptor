# simple-encryptor
A simple file encryption and decryption program

Many a young man has had to store his 3Gb "homework" on his laptop and with that decision comes the risk of his "homework" being disovered. Now, guys, don't get so attached to "homework" that you save it to hard storage, that's just weird, don't be that guy. Lift weights, read books, go fishing...just go outside. Enough rambling, let's get back to encryption i.e making sure our skeletons are right where they should be... in the closet.

This program operates on the reversible XOR (^) operation.
If
```
a ^ b => c
```
Then
```
c ^ b => a
```
Since
```
c ^ b => a ^ b ^ b => a    (b ^ b cancels out leaving a)
```

In like manner, we can XOR the plaintext data with a key to produce what appears to be gibberish, and if we XOR that gibberish with that same key, we get our data back.
So that's it.

## Compiling and Running the program
If you're on a Linux distro, you've most likely got g++ already. On Windows, you'll need to install [MinGW](https://sourceforge.net/projects/mingw/files/latest/download).
To compile, run
```
g++ program.cpp -o program.out
```

To run encryption
```
./program.out -e -i inputfile -o encrrptedoutputfile -k key
```
On Windows, type `program.out` instead of `./program.out`.

Decryption
```
./program.out -d -i encryptedfile -o plaintextoutputfile -k key
```

Running `./program.out -h` would print the help message.

Have fun.
