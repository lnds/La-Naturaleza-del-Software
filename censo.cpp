#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/** 
** Programa que calcula datos simulados del censo de población en Chile, 
** usado para ilustrar un post en La naturaleza del software sobre los 
** problemas de manejar grandes datos
**
*/

typedef unsigned short int16;
int POBLACION = 20*1000000;

int16* datos = new int16 [POBLACION];

void fill_data(void) 
{
	// esta es la peor manera de simular datos del censo, un simulador mejor
	// usara alguna distribución estadística adecuada.
	srand(time(NULL));
	for (int i =0; i < POBLACION; i++) {
		datos[i] = (int16) ( (  ((rand()&0x80)|(rand()&0x7F)) << 8 ) | ((rand()&0xF0)|(rand()&0x0F)) ); 
		//printf("%d %04x\n", i, datos[i]);
	}
}

int sum(int cnt[2][16][128], int s, int r) 
{
	int tot = 0;
	for (int i = 0; i < 128; i++)
	   tot += cnt[s][r][i];
	return tot;
}

int main(int argc, char* argv[]) 
{
    fill_data();
	
	clock_t ini = clock();
	
	int edad, sexo, region;
	int cnt[2][16][128];

	
	memset(cnt, 0, sizeof(cnt));
	for (int i = 0; i < POBLACION; i++) {
	   sexo = (datos[i] & 0x8000) >> 15;
	   edad = (datos[i] & 0x7F00) >> 8;
	   region = (datos[i] &0x00F0) >> 4;
	   //printf("%d;%d;%d\n", sexo, region, edad);
	   cnt[sexo][region][edad] = 1;
	}

	int tot =0;
	int acc = 0;

	for (int s = 0; s < 2; s++) {
	   for (int r = 0; r < 16; r++) {
		  printf ("region: %d, sexo: %d", r, s);
		  tot = sum(cnt, s, r);
		  acc = 0;
		  for (int e = 0; e < 128; e++) {
		     acc += cnt[s][r][e];
			 if (acc >= tot/2) {
			    printf(", mediana de edad: %d\n", e);
				break;
			 }
		  }
	   }
	}
	
	clock_t fin = clock();

	printf("elapsed: %f\n", (fin - ini) / (1.0*CLOCKS_PER_SEC));
	return 0;
}