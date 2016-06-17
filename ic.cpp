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
#include "fmpz.h"
#include "fmpz_mat.h"
 
typedef unsigned long ulong;
ulong PRIME_SIZE = 0;
ulong FACTORS_SIZE = 0;

static int mpcompare (void const *av, void const *bv)
{
        mpz_t *const *a = (mpz_t* const*)av;
        mpz_t *const *b = (mpz_t* const*)bv;
        return mpz_cmp (**a, **b);
}

struct mpzCompare
{
    bool operator() (const mpz_t val1, const mpz_t val2) const
    {
        return mpz_cmp(val1, val2) > 0;
    }
};


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
}   
  return (no == 1)? 1 : 0;
} 

void factorize(ulong no, ulong *primes, ulong *factors){

	for(ulong i=0;i<PRIME_SIZE;i++)
	{
		while(no%primes[i]==0)
		{
			no = no/primes[i];
			factors[FACTORS_SIZE++]=primes[i];
		}	
	}
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
		primes[PRIME_SIZE++]=p;
		//PRIME_SIZE++;
	}
	
}

void prime_exponents(ulong *factors, ulong *factor_exponent)
{
//array factor_exponent will have expo[prime] form
	ulong i;
	
	for(i=0;i<FACTORS_SIZE;i++)
	{
		++factor_exponent[factors[i]];
		printf("Factor[%lu]:Exponent[%lu]\n",factors[i],factor_exponent[factors[i]]);
	}	
}

int main(int argc, char *argv[])
{
	ulong i,j,k = 0;
        ulong ul_g = 37;
        ulong ul_h = 211;
        ulong ul_p = 18443;
        ulong ul_x = 0;
	ulong min = 100;
	ulong default_fact = 100; // TODO
        mpz_t g;  mpz_init(g);  mpz_set_ui(g,ul_g);
        mpz_t h;  mpz_init(h);  mpz_set_ui(h,ul_h);
        mpz_t p;  mpz_init(p);  mpz_set_ui(p,ul_p);
        mpz_t x;  mpz_init(x);  mpz_set_ui(x,ul_x);
        mpz_t n;  mpz_init(n);
	mpz_t *gi;
	mpz_t *A;
	mpz_t *B;
	//std::vector<std::map<mpz_t,ulong,mpzCompare>> ptoe;
	//std::vector<int>::iterator it;
	//it = ptoe.being();
	//std::map<mpz_t,ulong,mpzCompare> ptoe;	
	
	ulong b;
	printf("Enter B: ");
	scanf("%lu",&b);

	ulong * primes_till_b = (ulong *)calloc(b,sizeof(primes_till_b));
	
		
    	printf("Primes till %lu\n",b);
	//calculate primes to B 
	primes_to_n(b, primes_till_b);
	for(ulong i=0;i<PRIME_SIZE;i++)
		printf("%lu\t",primes_till_b[i]);
	printf("\n");
	std::map<ulong,ulong> *ptoe = new std::map<ulong,ulong>[10];
	//initialize gi array with prime size
	gi = (mpz_t*)malloc(sizeof(mpz_t)*(PRIME_SIZE+1));
	A = (mpz_t*)calloc(PRIME_SIZE*(PRIME_SIZE+1),sizeof(mpz_t));
	B = (mpz_t*)calloc(PRIME_SIZE+1,sizeof(mpz_t));
		
	//size of row of A matrix
	ulong m =0;	
	while(k<PRIME_SIZE+1)	
	{
		ulong num = rand() % ul_p + min;
		mpz_t temp_g ; mpz_init(temp_g);
		mpz_powm_ui(temp_g,g,num,p);	
		if(isSmooth(mpz_get_ui(temp_g),primes_till_b))
		{
			mpz_init_set_ui(B[k],num);	
			mpz_init_set(gi[k],temp_g);
			printf("%lu^%lu mod %lu is %lu smooth\n",ul_g,num,ul_p,b);
			ulong * factors = (ulong *)calloc(default_fact,sizeof(factors));
			//factorize g^i mod p
			factorize(mpz_get_ui(temp_g), primes_till_b,factors);
			printf("Factors: ");
			for(ulong i=0;i<FACTORS_SIZE;i++)
			printf("%lu,",factors[i]);
			printf("\n");
		
			//ulong * factor_exponent = (ulong *)calloc(PRIME_SIZE,sizeof(factor_exponent));
			//prime_exponents(factors,factor_exponent);

	        	for(ulong i=0;i<FACTORS_SIZE;i++)
			{
				for(ulong j=0;j<PRIME_SIZE;j++)
				{
					if(factors[i]==primes_till_b[j])
					++ptoe[k][primes_till_b[j]];
				}	
			}
			/*	
			for(ulong i=0;i<PRIME_SIZE;i++)
			{
				printf("Prime[%lu] exp[%lu]\n",primes_till_b[i],factor_exponent[primes_till_b[i]]);
			}
			*/
			printf("Factors");
			for(auto const &ent : ptoe[k]) {
                		printf(" %lu^%lu \t",ent.first,ent.second);
	        	}
			printf("\n");

			for(ulong i=0;i<PRIME_SIZE;i++)
			{
				mpz_init_set_ui(A[m],ptoe[k][primes_till_b[i]]);
				++m;
			}

			k++;
			FACTORS_SIZE=0;	
			free(factors);	
			//free(factor_exponent);
			
		}
	}
	/*
	printf("Matrix A[%lu]\n",m);
	for(ulong i=0; i<m;i++)
	{
		printf(" %lu",mpz_get_ui(A[i]));
		if((i+1)%3==0)
		printf("\n");
	}
	printf("\n");
	printf("Matrix B[%lu]\n",k);
	for(ulong i=0; i<k;i++)
        {
                printf(" %lu",mpz_get_ui(B[i]));
        }
	*/
        printf("\n");
	
	printf("Creating Matrices for Solving\n");
	nmod_mat_t Amod;
	nmod_mat_t Bmod;
	nmod_mat_t Xmod;

	//fmpz_t rows;	fmpz_init_set_ui(rows, PRIME_SIZE+1);
	//fmpz_t cols;	fmpz_init_set_ui(cols, PRIME_SIZE);
	slong arows = PRIME_SIZE+1;
	slong acols = PRIME_SIZE;
	mp_limb_t modn = UWORD(ul_p - 1);
	nmod_mat_init(Amod , arows , acols ,modn);
	for(ulong j=0;j<PRIME_SIZE;j++)
	{
		for(ulong i=0;i<PRIME_SIZE+1;i++)
		{
			ulong curr = mpz_get_ui(A[(i*PRIME_SIZE)+j]);
			mp_limb_t *aij = nmod_mat_entry_ptr(Amod, i, j);
			*aij = UWORD(curr+0);
		}
	}
	//mp_limb_t *ij = nmod_mat_entry_ptr(Amod, 1, 2);
	//*ij = UWORD(2); 	
	nmod_mat_print_pretty(Amod);

	slong brows = PRIME_SIZE+1;
	slong bcols = 1;

	nmod_mat_init(Bmod , brows , bcols ,modn);
	for(ulong i = 0; i<k;i++)
	{
		ulong curr = mpz_get_ui(B[i]);
		mp_limb_t *bij = nmod_mat_entry_ptr(Bmod, i, bcols-1);
		*bij = UWORD(curr+0);
	}
	nmod_mat_print_pretty(Bmod);

	slong crows = PRIME_SIZE;
	slong ccols = 1;
	
	nmod_mat_init(Xmod, crows, ccols, modn);

	//if(nmod_mat_solve(Xmod, Amod, Bmod)==1)
	nmod_mat_print_pretty(Xmod);

	free(primes_till_b);
	free(A);
	free(B);
	free(gi);
	return 0;
}
