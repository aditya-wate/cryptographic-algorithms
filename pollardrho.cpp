#include<stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <map>

typedef unsigned long ulong;

//Sample values

//const int64_t p = 48611,n= p-1;  /* p  -- prime     */
//const int64_t g = 19;            /* generator       */
//const int64_t a = 24717;    /* problem of form g^x = a%p, finding x */         

mpz_t p_mp;  //prime
mpz_t n_mp;  //p-1
mpz_t g_mp;  //generator
mpz_t a_mp;  //a
 
void f(mpz_t& x, mpz_t& alpha, mpz_t& beta){
	int cmp_0_gt,cmp_p, cmp_2p;
	mpz_t pby3; mpz_init(pby3);
	mpz_t twopby3; mpz_init(twopby3);
	mpz_cdiv_q_ui(pby3,p_mp,3);
	mpz_mul_ui(twopby3,pby3,2);	

	cmp_0_gt = mpz_cmp_ui(x,0);
	cmp_p = mpz_cmp(x,pby3);	
	cmp_2p = mpz_cmp(x,twopby3);	

	//0<= x < p/3	
	if(cmp_0_gt>=0 && cmp_p<0)
	{
		mpz_mul(x,x,g_mp);
		mpz_mod(x,x,p_mp);
		mpz_add_ui(alpha,alpha,1);
		mpz_mod(alpha,alpha,n_mp);		
	}

	//p/3 <= x < 2p/3
	else if(cmp_p>=0 && cmp_2p<0)
	{
		mpz_mul(x,x,x);
                mpz_mod(x,x,p_mp);
		mpz_mul_ui(alpha,alpha,2);
                mpz_mod(alpha,alpha,n_mp);
		mpz_mul_ui(beta,beta,2);
                mpz_mod(beta,beta,n_mp);

	}
	//2p/3 < x < p
	else
	{
		mpz_mul(x,x,a_mp);
                mpz_mod(x,x,p_mp);
                mpz_add_ui(beta,beta,1);
                mpz_mod(beta,beta,n_mp);

	}
	
	//freeing memory
	mpz_clear(pby3);
	mpz_clear(twopby3);
}

int main(void) {
	
	//mpz_init_set_ui(p_mp, p);
	mpz_init(p_mp);
	mpz_init(n_mp);
	mpz_init(g_mp);
	mpz_init(a_mp);
	printf("g^x = a(mod p)\n");
	printf("We will find x using Pollard Rho method\n...\n");
	printf("Enter p = ");
	gmp_scanf("%Zd",p_mp);

	mpz_sub_ui(n_mp,p_mp,1);
	printf("Enter g = ");
	gmp_scanf("%Zd",g_mp);

	printf("Enter a = ");
	gmp_scanf("%Zd",a_mp);
	
	mpz_t x, alpha, beta, k, y, gamma, delta;

	mpz_init_set_ui(x,1);
	mpz_init_set_ui(alpha,0);
	mpz_init_set_ui(beta,0);
	mpz_init_set_ui(k,0);

	mpz_init_set(y,x);
	mpz_init_set(gamma,alpha);
	mpz_init_set(delta,beta);

	mpz_t i; mpz_init_set_ui(i,1);
	int cmp_i_n = -1;
	int cmp_x_y = -1;

	//calling function once for x and twice for y	
	while(cmp_i_n<1)
	{
		f(x,alpha,beta);
		f(y,gamma,delta);
		f(y,gamma,delta);
		gmp_printf("%Zd %Zd %Zd %Zd %Zd %Zd %Zd \n", i,x,y,alpha,beta,gamma,delta);
	
		mpz_add_ui(i,i,1);
		cmp_i_n = mpz_cmp(i,n_mp);

		cmp_x_y = mpz_cmp(x,y);
		if(cmp_x_y==0) break;	
	}

	mpz_t u, v, w, d, s;
	mpz_init(u);
	mpz_init(v);
	mpz_init(w);
	mpz_init(d);
	mpz_init(s);
	
	//u = alpha-gamma%n
	mpz_sub(u,alpha,gamma);
	mpz_mod(u,u,n_mp);

	//v = beta-delta%n
	mpz_sub(v,delta,beta);
	mpz_mod(v,v,n_mp);

	//d = gcd(v,n)
	mpz_gcd(d,v,n_mp);
	gmp_printf("u=%Zd v=%Zd d=%Zd\n", u,v,d);	

	//extended eucledean for gcd d, we divide by d
	mpz_t vbyd, nbyd;
	mpz_init(vbyd);
	mpz_init(nbyd);
	
	//calculate v/d
	mpz_cdiv_q(vbyd,v,d);

	//calculate n/d
	mpz_cdiv_q(nbyd,n_mp,d);
	
	//calculate inverse of v/d mod n/d 
	mpz_invert(s,vbyd,nbyd);

	//w=s*u%n
	mpz_mul(w,s,u);
	mpz_mod(w,w,n_mp);
	
	gmp_printf("w=%Zd \n",w);

	int cmp_k_d = -1;
	int cmp_sol_a = -1;
        while(cmp_k_d<1)
	{
		mpz_t psol;	mpz_init(psol);
		mpz_t sol;	mpz_init(sol);
		mpz_t wbyd;	mpz_init(wbyd);		mpz_cdiv_q(wbyd,w,d);
		mpz_t knbyd;	mpz_init(knbyd);	mpz_mul(knbyd,k,nbyd);
		mpz_add(psol, wbyd, knbyd);
		mpz_powm(sol,g_mp,psol,p_mp);

		cmp_sol_a = mpz_cmp(sol,a_mp);
		if(cmp_sol_a == 0)
		{
			printf(" ... ... ...\n");
                        gmp_printf(" Answer to discrete log is %Zd\n",psol);
			mpz_clear(psol);
			mpz_clear(sol);
			mpz_clear(wbyd);
			mpz_clear(knbyd);
                        break;

		}
		mpz_add_ui(k,k,1);		
		cmp_k_d = mpz_cmp(k,d);

		//freeing memory
		mpz_clear(psol);
                mpz_clear(sol);
                mpz_clear(wbyd);
                mpz_clear(knbyd);

	}
	
	if(cmp_k_d==0)
		printf(" No solution to the discrete log.\n");
	
	//freeing memory
	mpz_clear(x);
	mpz_clear(y);
	mpz_clear(u);
	mpz_clear(v);
	mpz_clear(w);
	mpz_clear(d);
	mpz_clear(s);
	mpz_clear(vbyd);
	mpz_clear(nbyd);
	mpz_clear(alpha);
	mpz_clear(beta);
	mpz_clear(gamma);
	mpz_clear(delta);
	mpz_clear(i);
	mpz_clear(p_mp);
	mpz_clear(a_mp);
	mpz_clear(n_mp);
	mpz_clear(g_mp);
	
   	return 0;
}
