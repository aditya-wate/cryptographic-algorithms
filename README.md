# cryptographic-algorithms

pollardrho.cpp is the implementation for Pollard p Algorithm to find the solution to discrete log.

bsgs.cpp implements Shanks baby-step giant-step algorithm for the same problem.

bsgs.cpp reuses functions to find Multiplicative order from [4].

The implementations are based on the algorithms explained in: `An Introduction to Mathematical Cryptography by Jeffrey Hoffstein, Jill Pipher, Joseph H. Silverman`

*References:*

[1] Modular exponentiation: https://gist.github.com/orlp/3551590

[2] GCD algorithm: http://www.math.wustl.edu/~victor/mfmm/compaa/gcd.c

[3] Multiplicative inverse modulo: http://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/

[4] Multilpicative Order: https://rosettacode.org/wiki/Multiplicative_order#C

[5] Factors of an integer: https://rosettacode.org/wiki/Factors_of_an_integer#Prime_factoring
