// test_couleur.cpp : Seuille une image en niveau de gris

#include <iostream>
#include <fstream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <string.h>
#include <math.h>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <map> 

#include "image_ppm.h"
#include "Bloc.h"
#include "Clist.h"
#define N 765

using namespace std;

bool isGrey=false;
OCTET* ImgIn;
int nW,nH;

double euclidianDistance(OCTET *Img1,OCTET *Img2,int taille){
	double result=0.0;
	for (int i=0; i < taille; i+=3){
		int R1=Img1[i];int G1=Img1[i+1];int B1=Img1[i+2];
		int R2=Img2[i];int G2=Img2[i+1];int B2=Img2[i+2];

		result+=pow(R1-R2,2);
		result+=pow(G1-G2,2);
		result+=pow(B1-B2,2);
	}
	result=sqrt(result);
	return result;
}



void imageToBloc(OCTET* imageIn ,OCTET* bloc ,int ligneDep,int colDep,int nH_bloc, int nW_bloc){
	int indice,indiceBloc;
	for (int k = 0; k < nH_bloc; k++)
	{
		for (int g = 0; g < nW_bloc; g++)
		{
			indice=((ligneDep+k)*nW   +   colDep+g)*3;
			indiceBloc=(k*nW_bloc+g)*3;

			bloc[indiceBloc]=imageIn[indice];
			bloc[indiceBloc+1]=imageIn[indice+1];
			bloc[indiceBloc+2]=imageIn[indice+2];
		}
	}
}

void blocToImage(OCTET* imageOut,OCTET* bloc,int ligneDep,int colDep,int nH_bloc,int nW_bloc){
	int indice,indiceBloc;
	for (int k = 0; k < nH_bloc; k++)
	{
		for (int g = 0; g < nW_bloc; g++)
		{
			indice=((ligneDep+k)*nW   +   colDep+g)*3;
			indiceBloc=(k*nW_bloc+g)*3;
			imageOut[indice] = bloc[indiceBloc];
			imageOut[indice+1] = bloc[indiceBloc+1];
			imageOut[indice+2] = bloc[indiceBloc+2];
		}
	}
}


int nbElement(Maillon *blocs){
	int result=0;
	while(blocs!=NULL){
		result++;
		blocs=blocs->next;
	}
	return result;
}

Maillon* splitting(OCTET *ImgIn){
	int nbLigne=nH-N;
	int nbCol=nH-N;
	Maillon *result=new Maillon(Bloc(),NULL);
	for(int i=0;i<=nbLigne;i++){
		//printf("%d/%d\n",i,nH-N);
		for(int j=0;j<=nW-N;j++){
			Maillon *maillon=new Maillon(Bloc(i,j),NULL);
			addHead(result,maillon);
			//printf("%d %d\n",blocs->img.x,blocs->img.y);
		}
	}
	printf("%d\n",nbElement(result));
	return result;
}



bool greaterThanColor(Bloc img1,Bloc img2){
	int value1,value2;
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			value1=256*256*ImgIn[(img1.x+i)*nW+img1.y+j]+256*ImgIn[(img1.x+i)*nW+img1.y+j+1]+ImgIn[(img1.x+i)*nW+img1.y+j+2];
			value2=256*256*ImgIn[(img2.x+i)*nW+img2.y+j]+256*ImgIn[(img2.x+i)*nW+img2.y+j+1]+ImgIn[(img2.x+i)*nW+img2.y+j+2];
			if(value1>value2){
				return true;
			}
			else if(value1<value2){
				return false;
			}
		}
	}
	return true;
}
bool greaterThanGrey(Bloc img1,Bloc img2){
	int value1,value2;
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			value1=ImgIn[(img1.x+i)*nW+img1.y+j];
			value2=ImgIn[(img1.x+i)*nW+img1.y+j];
			if(value1>value2){
				return true;
			}
			else if(value1<value2){
				return false;
			}
		}
	}
	return true;
}

std::vector<OCTET*> subVect(std::vector<OCTET*> list,int debut,int fin){
	std::vector<OCTET*> result;
	for(int i=debut;i<fin;i++){
		result.push_back(list[i]);
	}
	return result;
}

void splitList(Maillon *list,Maillon* &l1, Maillon* &l2,int indexSplit){
	if(indexSplit==0 || indexSplit>nbElement(list)){
		l1=list;
		l2=NULL;
		return;
	}

	l1=list;
	l2=list;
	for(int i=0;i<indexSplit-1;i++){
		l2=l2->next;
	}
	Maillon *tail=l2;
	l2=l2->next;
	tail->next=NULL;
}

std::vector<OCTET*> addBeginning(std::vector<OCTET*> list,OCTET* head){
	std::vector<OCTET*> result;
	result.reserve(list.size()+1);
	result[0]=head;
	for(int i=1;i<list.size()+1;i++){
		result[i]=list[i-1];
	}
	return result;
}

/*
void fusion(Maillon* &A,Maillon* &B){
	if(nbElement(A)==0){
		A=B;
	}
	if(nbElement(B)==0){
		return;
	}
	printf("Fusion de %d %d\n",nbElement(A),nbElement(B));
	if(isGrey){
		if(greaterThanGrey(A->img,B->img)){
			Maillon *A1=A;
			Maillon *A2=A->next;
			A1->next=NULL;
			fusion(A2,B)
			addHead(A2,A1);
		}
		else{
			Maillon *B1=B;
			Maillon *B2=B->next;
			B1->next=NULL;fusion(B2,A)
			addHead(,B1);
		}
	}
	else{
		if(greaterThanColor(A->img,B->img)){
			Maillon *A1=A;
			Maillon *A2=A->next;
			A1->next=NULL;
			addHead(fusion(A2,B),A1);
		}
		else{
			Maillon *B1=B;
			Maillon *B2=B->next;
			B1->next=NULL;
			addHead(fusion(B2,A),B1);
		}
	}
}

void tri(Maillon* &list){
	if(nbElement(list)<=1){
		return;
	}
	else{
		Maillon *list2;
		splitList(list,list2,nbElement(list)/2);
		fusion(tri(list),tri(list2));
	}
}
*/


//Deplacer q apres p;
void deplacerMaillon(Maillon* &p,Maillon* &q){
	Maillon* tmp=q;
	q=tmp->next;
	tmp->next=p->next;
	p->next=tmp;
}

Maillon* fusionner(Maillon* &p,int P,Maillon* &q,int Q){
	for(int i=0;i<nbElement(p)-1;i++){
		if(greaterThanColor(p->next->img,q->next->img)){
			deplacerMaillon(p,q->next);
			if(Q==1)
				break;
			Q--;
		}
		else{
			if(P==1){
				while (Q>=1)
				{
					q=q->next;
					Q--;
				}
				break;
			}
			P--;
		}
		p=p->next;
	}
	return q;
}

Maillon* trier(Maillon *&list, int taille){
	Maillon *q=NULL;
	//printf("%d\n",taille);
	int Q=taille/2;
	int P=taille-Q;
	if(P>=2){
		q=trier(list,P);
		if(Q>=2)
			trier(q,Q);
	}
	else{
		q=list->next;
	}
	q=fusionner(list,P,q,Q);
	return q;
}



/*
void detection(OCTET *ImgOut,std::vector<OCTET*> blocs,std::vector<int> x ,std::vector<int> y,int deltaNW,int deltaNH){
	for(int i=0;i<blocs.size();i++){
		double min=MAXFLOAT;
		for(int j=i+1;j<blocs.size();j++){
			double dist=euclidianDistance(blocs[i],blocs[j],deltaNH*deltaNW*3);
			if(dist<min)
				min=dist;
			//printf("%lf\n",dist);
			if(dist==0.0){
				blocToImage(ImgOut,blocs[i],x[i],y[i],nW,deltaNH,deltaNW);
				blocToImage(ImgOut,blocs[i],x[j],y[j],nW,deltaNH,deltaNW);
				std::cout<<"Falsification"<<std::endl;
			}
		}
		printf("%d/%ld  %lf\n",i,blocs.size(),min);
	}
}
*/

int main(int argc, char* argv[])
{
	char cNomImgLue[250], cNomImgEcrite[250];
	int nTaille,nTaille3;

	int i,j;

	if (argc != 3)
	{
		printf("Usage: ImageIn.ppm ImageOut.ppm\n");
		exit(1);
	}

	sscanf(argv[1], "%s", cNomImgLue);
	sscanf(argv[2], "%s", cNomImgEcrite);

	OCTET *ImgOut; 

	lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
	std::cout << nH << " " << nW << endl;
	nTaille = nH * nW;
	nTaille3=nTaille*3;


	allocation_tableau(ImgIn, OCTET, nTaille3);
	lire_image_ppm(cNomImgLue, ImgIn, nTaille);
	//allocation_tableau(ImgTemp, OCTET, nTaille/(Q * Q));
	allocation_tableau(ImgOut, OCTET, nTaille3);
	
	char name_img[250];

	printf("Splitting\n");
	Maillon *blocs=	splitting(ImgIn);
	
	printf("%d elements\n",nbElement(blocs));
	printf("le premier est %d %d de pixel %d %d %d\n",blocs->img.x,blocs->img.y,ImgIn[blocs->img.x*nW+blocs->img.y],ImgIn[blocs->img.x*nW+blocs->img.y+1],ImgIn[blocs->img.x*nW+blocs->img.y+2]);
	//blocs=tri(blocs,N,false);

	//detection(ImgOut,blocs,xs,ys,nW,N,N);
	//test(ImgOut,blocs,xs,ys,nW,N,N);
	//imageToBloc(ImgIn,Bloc,0,0,nW,nH/2,nW/2);
	

	printf("Debut du tri...\n");
	trier(blocs,nbElement(blocs));
	printf("Tri fini\n");
	printf("le premier est %d %d de pixel %d %d %d\n",blocs->img.x,blocs->img.y,ImgIn[blocs->img.x*nW+blocs->img.y],ImgIn[blocs->img.x*nW+blocs->img.y+1],ImgIn[blocs->img.x*nW+blocs->img.y+2]);
blocs=blocs->next;
	printf("le premier est %d %d de pixel %d %d %d\n",blocs->img.x,blocs->img.y,ImgIn[blocs->img.x*nW+blocs->img.y],ImgIn[blocs->img.x*nW+blocs->img.y+1],ImgIn[blocs->img.x*nW+blocs->img.y+2]);

	printf("Copie finie\n");
	ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
	std::vector<Maillon*> maillons;

	while(blocs!=NULL){
		maillons.push_back(blocs);
		blocs=blocs->next;
	}
	printf("%ld\n",maillons.size());
	for(int i=0;i<maillons.size();i++){
		//printf("%d %d\n",maillons[i]->img.x,maillons[i]->img.y);
		delete maillons[i];
	}
	//delete blocs;

	free(ImgIn);
	free(ImgOut);
	return 1;
}

