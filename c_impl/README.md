# INSTALLATION

## PBC Library
To install the pbc.h library, follow this instructions into the lib/gmp folder:

```
./configure
make
make install
```

WARNING: the last command could require to be run with admin privileges (sudo make install).
> Source: https://crypto.stanford.edu/pbc/manual/ch01.html


### ERROR: no pbc.h file

Copy all the files in "all_pbc_files" folder into /usr/local/include.

> Source: https://github.com/blynn/pbc/tree/master/include

### Needed libraries
Run the following commands, from c_impl/lib:
```
sudo apt update
sudo apt install ./libgmp3c2_4.3.2+dfsg-2ubuntu1_amd64.deb
sudo apt install ./libpbc0_0.5.12_amd64.deb
sudo apt install ./libpbc-dev_0.5.12_amd64.deb
```
If something is not working, try to run:
```
sudo ldconfig
```
> Sources:
> - https://www.ubuntuupdates.org/package/core/precise/universe/base/libgmp3c2
> - http://voltar.org/pbcfiles/

## SHA Library
To install the openssl/sha.h library, follow this instructions into the lib/openssl folder:

```
./config
make
make test
make install
```

WARNING: the last command could require to be run with admin privileges (sudo make install).
> Source: https://github.com/openssl/openssl

# BUILD and EXECUTION
To compile the program, just run the command:
```
make
```
within the c_impl folder.

To run it, just execute:
```
./ml_por
```
