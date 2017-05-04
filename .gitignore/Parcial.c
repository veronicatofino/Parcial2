
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Dict {
	char palIngles [50];
	char palEspanol [50];
};

typedef struct Dict TipoDict;


typedef struct nodoP{
	char palabra[50];
	struct nodoP* siguiente;
	struct nodoP* traduccion;
}TipoNodoP;

TipoNodoP * crearNodoP(char* pal){
	TipoNodoP *nuevo=((TipoNodoP*) malloc(sizeof(TipoNodoP)));
	strcpy(nuevo->palabra,pal);
	nuevo->siguiente=NULL;
	nuevo->traduccion=NULL;

}

TipoNodoP* lstIngles=NULL;
TipoNodoP* lstEspanol=NULL;


int buscarPosI (char *palabra){
	int cont=0;
	char palIngles [50];
	TipoNodoP *p=lstIngles;
	if (lstIngles== NULL){
		return 1;
	}
	else {
		do{
			if (p == NULL){
				return cont+1;
			}
			strcpy (palIngles,p->palabra);
			cont ++;
			p=p->siguiente;
		} while (strcmp(palabra,palIngles)>0);
	}
	return cont;
}

int buscarPosE(char *palabra){
	int cont=0;
	char palEspanol [50];
	TipoNodoP *p=lstEspanol;
	if (lstEspanol== NULL){
		return 1;
	}
	else {
		do{
			if (p == NULL){
				return cont+1;
			}
			strcpy (palEspanol,p->palabra);
			cont ++;
			p=p->siguiente;
		} while (strcmp(palabra,palEspanol)>0);
	}
	return cont;
} 

TipoNodoP * addLista2P (char * palIngles,char *palEspanol){
	TipoNodoP *p=lstIngles;
	TipoNodoP *q=lstEspanol;
	TipoNodoP *nodoIngles=crearNodoP(palIngles);
	TipoNodoP *nodoEspanol=crearNodoP(palEspanol);
	nodoIngles->traduccion=nodoEspanol;
	nodoEspanol->traduccion=nodoIngles;
	int posi=buscarPosI(palIngles);
	int pose=buscarPosE(palEspanol);
	int conti=1;
	int conte=1;

	if (posi == 1 && pose==1){
		lstIngles=nodoIngles;
		lstEspanol=nodoEspanol;
	}
	else {
		while (conti<posi-1){
			p=p->siguiente;
			conti++;
		}
		nodoIngles->siguiente=p->siguiente;
		p->siguiente=nodoIngles;
		while (conte<pose-1){
			q=q->siguiente;
			conte++;
		}
		nodoEspanol->siguiente=q->siguiente;
		q->siguiente=nodoEspanol;

	}
}
void imprimirOrdenadoEsp (){
	TipoNodoP* lst=lstEspanol;
	while (lst !=NULL){
		printf("%s : %s\n",lst->palabra,lst->traduccion->palabra);
		lst=lst->siguiente;
	}
}
void cargarArchaLista(char * nombreArch){
	FILE *mnjbin;
	mnjbin=fopen(nombreArch,"r");
	TipoDict dicttmp;
	while (fread(&dicttmp,sizeof(TipoDict),1,mnjbin)==1){
		addLista2P(dicttmp.palIngles,dicttmp.palEspanol);
	}
	fclose(mnjbin);
	imprimirOrdenadoEsp();

	
}

void guardarSubArchivoTexto(char *nombreArch,TipoNodoP* lst ,char *palInicial,char *palFinal){
	FILE* backup;
	backup=fopen(nombreArch,"w");
	int posi=buscarPosE(palInicial);
	int posno=buscarPosE(palFinal);
	TipoNodoP *p=lst;
	char cadena[50];
	while(p != NULL){
		int i=buscarPosE(p->palabra);
		if (i>posi && i<posno){
			sprintf(cadena,"< %s >\n",p->palabra);
			fputs(cadena,backup);
			printf("%s",cadena);
		}
		p=p->siguiente;
	}

}


int main (int argc, char * argv[]){
	cargarArchaLista("minidict.bin");
	guardarSubArchivoTexto("jai.txt",lstEspanol,"carbohidrato","elegante");
	return 0;
}
