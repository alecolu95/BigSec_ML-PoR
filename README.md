# BigSec - Message-Locked Proofs of Retrievability with Secure Deduplication

## TEAM:
- Colucci Alessandro
- Convertini Pasquale
- Möllering Helen
- Sanislav Sebastian

## INSTALLATION
### Introduction
All the needed libraries are located into the **lib** folder:
- PBC: Pairing-Based Cryptography library, main lib for the implementation of the protocol;
- GMP: GNU  Multiple  Precision  Arithmetic library, on which PBC relies;
- OpenSSL: using the hashing functions (SHA256 in particular).

### Installation of the libraries
To install the libraries, go to the lib folder and extract the archives:
- gmp-4.3.1-tar-gz
- openssl-master.zip
- pbc-0.5.14.tar.gz

Then for each library go to the corresponding folder and run the following commands:
```
./configure (OR ./config , since it may change from lib to lib)
make
sudo make install
```

> Sources:
> - https://crypto.stanford.edu/pbc/manual/ch01.html
> - https://www.ubuntuupdates.org/package/core/precise/universe/base/libgmp3c2
> - https://github.com/openssl/openssl
> - http://voltar.org/pbcfiles/

Then update the installed libraries by running:
```
sudo ldconfig
```

Once you are done, everything should be ready.

#### Error solving
In case you get an error using the PBC library, try to copy the files from ***lib/all_pbc_files*** into ***/usr/local/include***.
If still not working, try to run again:
```
sudo ldconfig
```

> Source: https://github.com/blynn/pbc/tree/master/include

#### Library dependencies problem
We encountered a problem installing the latest version of gmp, conflicting with the one pbc requires.
We used the **gmp-4.3.1**, already in the ***lib*** folder.

> Link for all older gmp's versions: https://ftp.gnu.org/gnu/gmp




## Implementation
### Introduction
There are 2 versions of the implementation of the protocol, divided into 2 folders:
- c_impl: simulating a 1-user interaction with the system and checking that the signatures verify;
- c_impl2: simulating a 2-users interaction with the system and checking that they get the same key Kf.
### Running the program
Go into the folder of the program you want to run. 
To compile the code, execute:
```
make
```
To run the program, execute:
```
make run
```
