#include <stdio.h>
#include <stdlib.h>
long int v[10000][3];
long int o[1000][1000];

void ordenar (int nivel, int valor)
{
	int i;

	for (i=0; i<1000; i++)
	{
		if (o[nivel][i]==-1)
		{
			o[nivel][i] = valor;
			break;	
		}
	}
	return;
}

 int altura (long int  n, long int nivel )
{
	if (v[n][0]==-1) return 0;
	ordenar (nivel, v[n][0]);


	 altura (v[n][1], nivel+1); // PELA ESQUERDA
	 altura (v[n][2], nivel +1); // PELA DIREITA
	 return 0;
}

int main ()
{
	long int valor, filhoE, filhoD, i, j, N, TEMP;
	scanf ("%ld", &N);
	//DEIXANDO O VETOR =-1
	for (i=0; i<1000; i++)
	{
		for (j=0; j<1000; j++)
		{
			o[i][j]=-1;
		}
	}
	for (i=0; i<N; i++)
	{
		scanf ("%ld%ld%ld", &valor, &filhoE, &filhoD);
		v[i][0]=valor;
		v[i][1]=filhoE;
		v[i][2]=filhoD;
	}


	//altura (0, 0);


for (i=0; i<N; i++)
	{
		for (j=0; j<3; j++)
		{
			printf ("%ld ",v[i][j]);
		}
		printf ("\n ");
	}
	


	//SAÍDA
	/*
	for (i=0; i<N; i++)
	{
		for (j=0; j<1000; j++)
		{
			if (o[i][j]!=-1) printf("%d ",o [i][j] );
			else
			{
				break;
			}
		}
		printf("\n");
	}	
*/

	return 0;
}







/*
#define li long int


li **arvore;

li maximo(li a, li b)
{
	return a > b ? a : b;
}

li altura(li n)
{
	if (n == -1)
	{
		return 0;
	}
	return 1 + maximo( altura(arvore[n][0]), altura(arvore[n][1]) );
}

int main()
{
	li i, n, e, raiz, pai, filho_esq, filho_dir;
	
	scanf("%ld%ld%ld", &n, &e, &raiz);
	
	arvore = (li **) malloc( n * sizeof(li *) );

	for (i = 0; i < n; ++i)
	{
		// cada linha
		arvore[i] = (li *) malloc(sizeof(li) * 2);	
		arvore[i][0] = -1;
		arvore[i][1] = -1;
	}

	for (i = 0; i < e; ++i)
	{
		scanf("%ld%ld%ld", &pai, &filho_esq, &filho_dir);
		arvore[pai][0] = filho_esq;
		arvore[pai][1] = filho_dir;
	}
	
	printf("%ld\n", altura(raiz));

	return 0;
}
*/
