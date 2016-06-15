#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <map>
#include <inttypes.h> // uintmax_t
#include <limits>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
 
typedef unsigned long ulong;
ulong PRIME_SIZE = 0;

ulong maxDivide(ulong a, ulong b)
{
  while (a%b == 0)
   a = a/b; 
  return a;
}

int isSmooth(ulong no, ulong *primes)
{

for(ulong i=0;i<PRIME_SIZE;i++)
{
  no = maxDivide(no, primes[i]);
  printf("Progress:%lu\n",no);
 // no = maxDivide(no, 3);
 // no = maxDivide(no, 5);
}   
  return (no == 1)? 1 : 0;
} 

void primes_to_n(ulong n , ulong *primes)
{
    // AW:Source: http://www.geeksforgeeks.org/sieve-of-eratosthenes/
    // AW:the code has been modified to support unsigned long
    // Create a boolean array "prime[0..n]" and initialize
    // all entries it as true. A value in prime[i] will
    // finally be false if i is Not a prime, else true.
    bool prime[n+1];
    memset(prime, true, sizeof(prime));
     
    for (ulong p=2; p*p<=n; p++)
    {
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == true)
        {
            // Update all multiples of p
            for (ulong i=p*2; i<=n; i += p)
                prime[i] = false;
        }
    }

    // Print all prime numbers
    for (ulong p=2; p<=n; p++)
       if (prime[p])
	{
		primes[PRIME_SIZE]=p;
		PRIME_SIZE++;
	}
	
}

int main(int argc, char *argv[])
{
	ulong i,j,k = 0;
        ulong ul_g = 37;
        ulong ul_h = 211;
        ulong ul_p = 18443;
        ulong ul_x = 0;
        mpz_t g;  mpz_init(g);  mpz_set_ui(g,ul_g);
        mpz_t h;  mpz_init(h);  mpz_set_ui(h,ul_h);
        mpz_t p;  mpz_init(p);  mpz_set_ui(p,ul_p);
        mpz_t x;  mpz_init(x);  mpz_set_ui(x,ul_x);
        mpz_t n;  mpz_init(n);
	
	ulong b;
	printf("Enter B: ");
	scanf("%lu",&b);

	ulong * primes_till_b = (ulong *)calloc(b,sizeof(primes_till_b));

		
    	printf("Primes till %lu\n",b);
 
	primes_to_n(b, primes_till_b);
	for(ulong i=0;i<PRIME_SIZE;i++)
		printf("Prime[%lu]:%lu\n",i,primes_till_b[i]);
	printf("\n");
	ulong num = 61;
	if(isSmooth(num,primes_till_b))
		printf("is smooth\n");
	
	return 0;
}
