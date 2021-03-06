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
  if(a==0)
  return a;
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
	mpz_t *gi, *A, *B, *X;
	
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
	X = (mpz_t*)calloc(PRIME_SIZE,sizeof(mpz_t));	
	
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
			
			/*	
			printf("Factors: ");
			for(ulong i=0;i<FACTORS_SIZE;i++)
				printf("%lu,",factors[i]);
			*/
			printf("\n");
		

	        	for(ulong i=0;i<FACTORS_SIZE;i++)
			{
				for(ulong j=0;j<PRIME_SIZE;j++)
				{
					if(factors[i]==primes_till_b[j])
					++ptoe[k][primes_till_b[j]];
				}	
			}
			
			printf("Factors");
			for(auto const &ent : ptoe[k]) {
                		printf(" %lu^%lu \t",ent.first,ent.second);
	        	}
			printf("\n");
			
			//Set the A matrix to calculated values
			for(ulong i=0;i<PRIME_SIZE;i++)
			{
				mpz_init_set_ui(A[m],ptoe[k][primes_till_b[i]]);
				++m;
			}

			k++;
			FACTORS_SIZE=0;	
			free(factors);	
			
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
	
        printf("\n");
	*/
	printf("Creating Matrices for solution of Ax=B\n");
	nmod_mat_t Amod;
	nmod_mat_t Bmod;
	nmod_mat_t Xmod;

	//TODO: Delete
	/*
	mpz_set_ui(A[0],3);
	mpz_set_ui(A[1],4);
	mpz_set_ui(A[2],1);
	mpz_set_ui(A[3],3);
	mpz_set_ui(A[4],0);
	mpz_set_ui(A[5],2);
	mpz_set_ui(A[6],3);
	mpz_set_ui(A[7],3);
	mpz_set_ui(A[8],1);
	mpz_set_ui(A[9],3);
	mpz_set_ui(A[10],1);
	mpz_set_ui(A[11],4);
	
	mpz_set_ui(B[0],12708);
	mpz_set_ui(B[1],11311);
	mpz_set_ui(B[2],15400);
	mpz_set_ui(B[3],2731);
	*/
	//Initialize & Populate Matrix Amod	
	slong arows = PRIME_SIZE; //+1
	slong acols = PRIME_SIZE;
	mp_limb_t modn = UWORD(ul_p -1);
	nmod_mat_init(Amod , arows , acols ,modn);
	for(ulong j=0;j<PRIME_SIZE;j++)
	{
		for(ulong i=0;i<PRIME_SIZE;i++) //+1
		{
			ulong curr = mpz_get_ui(A[(i*PRIME_SIZE)+j]);
			mp_limb_t *aij = nmod_mat_entry_ptr(Amod, i, j);
			*aij = UWORD(curr+0);
		}
	}
	
	nmod_mat_print_pretty(Amod);

	//Initialize & Polpulate Matrix Bmod
	slong brows = PRIME_SIZE;//+1
	slong bcols = 1;

	nmod_mat_init(Bmod , brows , bcols ,modn);
	for(ulong i = 0; i<PRIME_SIZE;i++)//+1
	{
		ulong curr = mpz_get_ui(B[i]);
		mp_limb_t *bij = nmod_mat_entry_ptr(Bmod, i, bcols-1);
		*bij = UWORD(curr+0);
	}
	nmod_mat_print_pretty(Bmod);

	//Initialize  Matrix Xmod
	slong crows = PRIME_SIZE;
	slong ccols = 1;
	
	nmod_mat_init(Xmod, crows, ccols, modn);

	//if(nmod_mat_solve(Xmod, Amod, Bmod)==1)
	nmod_mat_solve(Xmod,Amod,Bmod);
	nmod_mat_print_pretty(Xmod);
	
	for(ulong i = 0; i<PRIME_SIZE;i++)
        {
		mpz_init(X[i]);
		mpz_set_ui(X[i],nmod_mat_get_entry(Xmod,i,bcols-1));

        }
	
	
	//Verify values
	mpz_t *solutions = (mpz_t*)calloc(PRIME_SIZE,sizeof(mpz_t));
	for(ulong i = 0; i<PRIME_SIZE;i++)
	{	
		mpz_init(solutions[i]);
		mpz_powm_ui(solutions[i],g,mpz_get_ui(X[i]),p);
		printf("%lu\n",mpz_get_ui(solutions[i]));
	}
	//TODO: delete below hard-code

	/*
	mpz_set_ui(solutions[0],5733);
	mpz_set_ui(solutions[1],15750);
	mpz_set_ui(solutions[2],6277);
	*/
	std::map<ulong,ulong> log;

	 for(ulong j=0;j<PRIME_SIZE;j++)
                {
                        log[primes_till_b[j]]=mpz_get_ui(solutions[j]);
                }


	free(A);
	free(B);
	free(gi);


	nmod_mat_clear(Amod);
        nmod_mat_clear(Bmod);
        nmod_mat_clear(Xmod);
	
	//check if hg^-k is B smooth
	mpz_t hginvk;
	mpz_t ginv; mpz_init(ginv);
	mpz_t ginvk;
	mpz_t zero;
	mpz_init_set_ui(hginvk, 0);
	mpz_init_set_ui(ginvk,0 );
	mpz_invert(ginv,g,p);
	k =0;
	while(isSmooth(mpz_get_ui(hginvk),primes_till_b)!=1)
	{
		k = rand() % ul_p + min;
		mpz_powm_ui(ginvk,ginv,k,p);
		mpz_mul(hginvk,h,ginvk);
		mpz_mod(hginvk,hginvk,p);	
		//k = rand() % ul_p + min;
		//printf("K=%lu\n",k);	
	}
	printf("K=%lu\n",k);
	//k = 9549;
	ulong * fac = (ulong *)calloc(1000,sizeof(ulong));
                        //factorize hg^-k mod p

        factorize(mpz_get_ui(hginvk), primes_till_b,fac);

	std::map<ulong,ulong> stoe;

	for(ulong i=0;i<FACTORS_SIZE;i++)
        {
        	for(ulong j=0;j<PRIME_SIZE;j++)
        	{
                	if(fac[i]==primes_till_b[j])
                        ++stoe[primes_till_b[j]];
                }
         }

         printf("Factors");
         for(auto const &ent : stoe) {
         	printf(" %lu^%lu \t",ent.first,ent.second);
         }
                        printf("\n");

	mpz_t final_soln;
	mpz_t prod;
	mpz_init(prod);
	mpz_init_set_ui(final_soln,k);
	for(ulong i=0;i<PRIME_SIZE;i++)
	{
		printf("%lu\n",stoe[primes_till_b[i]]);
		mpz_set_ui(prod,log[primes_till_b[i]]);
		mpz_mul_ui(prod,prod,stoe[primes_till_b[i]]);
		mpz_add(final_soln,final_soln,prod);	
	}
	mpz_mod_ui(final_soln,final_soln,(ul_p-1));
	printf("Discrete Log Solution is %lu\n",mpz_get_ui(final_soln));


	//free memory
	free(primes_till_b);
	return 0;
}
