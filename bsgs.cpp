#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <map>

char bits[65536];
 
typedef unsigned long ulong;
ulong primes[7000], n_primes;
 

typedef struct { ulong p, e; } prime_factor;

void sieve()
{
	int i, j;
	memset(bits, 1, 65536);
	bits[0] = bits[1] = 0;
	for (i = 0; i < 256; i++)
		if (bits[i])
			for (j = i * i; j < 65536; j += i)
				bits[j] = 0;
 
	/* collect primes into a list. slightly faster this way if dealing with large numbers */
	for (i = j = 0; i < 65536; i++)
		if (bits[i]) primes[j++] = i;
 
	n_primes = j;
}

int ulong_cmp(const void *a, const void *b)
{
	return *(const ulong*)a < *(const ulong*)b ? -1 : *(const ulong*)a > *(const ulong*)b;
}

int get_prime_factors(ulong n, prime_factor *lst)
{
	ulong i, e, p;
	int len = 0;
 
	for (i = 0; i < n_primes; i++) {
		p = primes[i];
		if (p * p > n) break;
		for (e = 0; !(n % p); n /= p, e++);
		if (e) {
			lst[len].p = p;
			lst[len++].e = e;
		}
	}
 
	return n == 1 ? len : (lst[len].p = n, lst[len].e = 1, ++len);
}

ulong mpow(ulong a, ulong p, ulong m)
{
	ulong r = 1;
	while (p) {
		if ((1 & p)) r = r * a % m;
		a = a * a % m;
		p >>= 1;
	}
	return r;
}
 
ulong ipow(ulong a, ulong p) {
	ulong r = 1;
	while (p) {
		if ((1 & p)) r = r * a;
		a *= a;
		p >>= 1;
	}
	return r;
}

ulong gcd(ulong m, ulong n)
{
	ulong t;
	while (m) { t = m; m = n % m; n = t; }
	return n;
}
 
ulong lcm(ulong m, ulong n)
{
	ulong g = gcd(m, n);
	return m / g * n;
}
int get_factors(ulong n, ulong *lst)
{
	int n_f, len, len2, i, j, k, p;
	prime_factor f[100];
 
	n_f = get_prime_factors(n, f);
 
	len2 = len = lst[0] = 1;
	/* L = (1); L = (L, L * p**(1 .. e)) forall((p, e)) */
	for (i = 0; i < n_f; i++, len2 = len)
		for (j = 0, p = f[i].p; j < f[i].e; j++, p *= f[i].p)
			for (k = 0; k < len2; k++)
				lst[len++] = lst[k] * p;
 
	qsort(lst, len, sizeof(ulong), ulong_cmp);
	return len;
}
ulong multi_order_p(ulong a, ulong p, ulong e)
{
	ulong fac[10000];
	ulong m = ipow(p, e);
	ulong t = m / p * (p - 1);
	int i, len = get_factors(t, fac);
	for (i = 0; i < len; i++)
		if (mpow(a, fac[i], m) == 1)
			return fac[i];
	return 0;
}
ulong multi_order(ulong a, ulong m)
{
	prime_factor pf[100];
	int i, len = get_prime_factors(m, pf);
	ulong res = 1;
	for (i = 0; i < len; i++)
		res = lcm(res, multi_order_p(a, pf[i].p, pf[i].e));
	return res;
}

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

int main()
{
	sieve();
	ulong i,j,k = 0;
	ulong ul_g = 9704;
	ulong ul_h = 13896;
	ulong ul_p = 17389;
	ulong ul_N = multi_order(ul_g,ul_p);
	ulong ul_x = 0;
	mpz_t g;  mpz_init(g);  mpz_set_ui(g,ul_g);
	mpz_t h;  mpz_init(h);  mpz_set_ui(h,ul_h);
	mpz_t p;  mpz_init(p);  mpz_set_ui(p,ul_p);
	mpz_t N;  mpz_init(N);  mpz_set_ui(N,ul_N);
	mpz_t x;  mpz_init(x);  mpz_set_ui(x,ul_x);
	mpz_t n;  mpz_init(n);

	//Calculate n = root(N)+1
	mpz_sqrt(n, N);
	mpz_add_ui(n,n,1);

	mpz_t gprog[mpz_get_ui(n)];
	mpz_t hprog;
	printf(" Generator(g) = %lu\n h = %lu\n modulo(p) = %lu\n", ul_g,ul_h,ul_p);
	printf(" Order(N) = %lu\n", mpz_get_ui(N));
	printf(" Now n = %lu\n", mpz_get_ui(n));

	mpz_init_set(gprog[1],g);
	std::map<mpz_t,ulong,mpzCompare> gk;
	//algo starts here
	for(k=1;k<=mpz_get_ui(n);k++){
		gk[gprog[k]]=k;
		mpz_init(gprog[k+1]);
		mpz_mul(gprog[k+1],gprog[k],g);
		mpz_mod(gprog[k+1],gprog[k+1],p);
	}
	for(auto const &ent : gk) {
		printf(" g^%lu=%lu\n",ent.second,mpz_get_ui(ent.first));
	}
	
	mpz_t ginv;  mpz_init(ginv);  mpz_set(ginv,g);
	mpz_powm(ginv, ginv, n, p);
	mpz_invert(ginv, ginv, p);
	mpz_init_set(hprog, h);

	for(j=1;j<=mpz_get_ui(n);j++){
		
		mpz_mul(hprog,hprog,ginv);
		mpz_mod(hprog,hprog,p);						
		i = gk.find(hprog)->second,j;
		if(i!=0){
			printf(" Collision detected: i=%lu and j=%lu for %lu\n",gk.find(hprog)->second,j,mpz_get_ui(hprog));	
			break;
		}		
	}
	printf("\n Solution:%lu\n",(i+(j*mpz_get_ui(n))));
	return 0;

} 	
