#include<stdio.h>
#include <inttypes.h>
const int64_t p = 48611,n= p-1;  /* p  -- prime     */
const int64_t g = 19;            /* generator       */
const int64_t a = 24717;    /* problem of form g^x = a%p, finding x */         
 
int64_t power(int64_t base, int64_t exponent, int64_t modular)
{
    // Source: https://gist.github.com/orlp/3551590
    int64_t result = 1;      // Initialize result

    base = base % modular;  // Update x if it is more than or
                // equal to p

    while (exponent > 0)
    {
        // If y is odd, multiply x with result
        if (exponent & 1)
            result = (result*base) % modular;

        // y must be even now
        exponent = exponent>>1; // y = y/2
       base = (base*base) % modular;
    }
    return result;
}


int64_t gcd ( int64_t a, int64_t b )
{
	//Source: http://www.math.wustl.edu/~victor/mfmm/compaa/gcd.c
	int64_t c;
	while ( a != 0 ) 
	{
		c = a; a = b%a;  b = c;
	}
  	return b;
}

int64_t modInverse(int64_t a, int64_t m)
{
	//Source: http://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
    	int64_t m0 = m, t, q;
    	int64_t x0 = 0, x1 = 1;
    	if (m == 1)
      	return 0;
    	while (a > 1)
    	{
        	// q is quotient
        	q = a / m;
        	t = m;
        	// m is remainder now, process same as
        	// Euclid's algo
        	m = a % m, a = t;
		t = x0;
        	x0 = x1 - q * x0;
        	x1 = t;
    	}	
    	// Make x1 positive
    	if (x1 < 0)
       		x1 += m0;
    	return x1;
}

void fx(int64_t& x, int64_t& alpha, int64_t& beta){
	if(x>=0 && x<(p/3))
	{
		x = x*g % p;  
		alpha = (alpha+1) % n;
	}
	else if(x>=(p/3) && x < ((2*p)/3))
	{
		x = x*x % p;  
		alpha =  alpha*2  % n;  
		beta =  beta*2  % n;
	}
	else
	{
		x = x*a  % p;                  
		beta = (beta+1) % n;
	}
}

int main(void) {
   	int64_t x=1, alpha=0, beta=0, k;
   	int64_t y=x, gamma=alpha, delta=beta;
   	for(int i = 1; i < n; ++i ) {
     		fx( x, alpha, beta );
     		fx( y, gamma, delta );
     		fx( y, gamma, delta );
     		printf( " %3d"  " %"PRIu64 " %"PRIu64 " %"PRIu64 " %"PRIu64 " %"PRIu64 " %"PRIu64"\n", i, x, y, alpha, beta, gamma, delta );
     
		if( x == y ) break;
   	}

   	int64_t u = (alpha - gamma)%n;
   	int64_t v = (delta - beta) %n;
   	//check if d is >=2 
   	int64_t d = gcd(v,n);
  	printf("\n u=%"PRIu64"\n v=%"PRIu64"\n d=%"PRIu64"\n", u,v,d);
   	
	//extended eucledean for gcd d, we divide by d
   	int64_t s = modInverse(v/d,n/d);
   	
	printf(" s=%"PRIu64"\n",s);
   	
	int64_t w = (s*u)%n;
 	
	printf(" w=%"PRIu64"\n",w);
	
	for(k = 0;k <= (d-1); k++){
		int64_t psol = ((w/d)+(k*(n/d)));
		int64_t sol = power(g,psol,p);
		
		if(sol==a)
		{
			printf(" ... ... ...\n");
			printf(" Answer to dicrete log is %"PRIu64"\n",sol);
			break;
		}
	}
        if(k==d)
		printf(" No solution to the discrete log.\n"); 
   	return 0;
}
