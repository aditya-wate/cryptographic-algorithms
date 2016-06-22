# cryptographic-algorithms

pollardrho.cpp is the implementation for Pollard p Algorithm to find the solution to discrete log.

bsgs.cpp implements Shanks baby-step giant-step algorithm for the same problem.

bsgs.cpp reuses functions to find Multiplicative order from [4].

The implementations are based on the algorithms explained in: `An Introduction to Mathematical Cryptography by Jeffrey Hoffstein, Jill Pipher and Joseph H. Silverman`


Instructions to run code:
1. Install libmpg for multiprecision:
    sudo apt-get install libgmp3-dev
2. Install libmpfr for mutliprecision
     sudo apt-get install libmpfr-dev libmpfr-doc libmpfr4 libmpfr4-dbg
3. Install Flint (http://www.flintlib.org/flint-2.5.pdf):
    a. wget http://www.flintlib.org/flint-2.5.2.tar.gz
    b. tar -xvf flint-2.5.2.tar.gz
    c. cd flint-2.5.2/
    d. ./configure
    e. make
    f. make install
4. Once all libraries are install. Run make:
    $ make
The able should create 3 files bsgs (Baby-step-giant-step), pollardrho (Pollard Rho) and ic (Index Calculus). These can be directly run. However, the files are not formatted to accept input. The values need to be changed in the code.


*References:*

[1] Modular exponentiation: https://gist.github.com/orlp/3551590

[2] GCD algorithm: http://www.math.wustl.edu/~victor/mfmm/compaa/gcd.c

[3] Multiplicative inverse modulo: http://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/

[4] Multilpicative Order: https://rosettacode.org/wiki/Multiplicative_order#C

[5] Factors of an integer: https://rosettacode.org/wiki/Factors_of_an_integer#Prime_factoring
