
#include <string>
#include <iostream>
#include <filesystem>

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

namespace fs = std::filesystem;

using namespace std;

bool isGrey=false;
OCTET* ImgIn;
int nW,nH;
int N;

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

std::vector<Bloc> splitting(){
	int nbLigne=nH-N;
	int nbCol=nW-N;
	std::vector<Bloc> blocs;
	for(int i=0;i<=nbLigne;i++){
		for(int j=0;j<=nbCol;j++){
			blocs.push_back(Bloc(i,j));
		}
	}
	printf("%ld\n",blocs.size());
	return blocs;
}



bool greaterThanColor(Bloc img1,Bloc img2){
	int value1,value2;
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			value1=256*256*ImgIn[((img1.x+i)*nW+img1.y+j)*3]+256*ImgIn[((img1.x+i)*nW+img1.y+j)*3+1]+ImgIn[((img1.x+i)*nW+img1.y+j)*3+2];
			value2=256*256*ImgIn[((img2.x+i)*nW+img2.y+j)*3]+256*ImgIn[((img2.x+i)*nW+img2.y+j)*3+1]+ImgIn[((img2.x+i)*nW+img2.y+j)*3+2];
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

void permuter(std::vector<Bloc> &liste,int i,int j){
	Bloc tmp=liste[i];
	liste[i]=liste[j];
	liste[j]=tmp;
}

void envoyerAuDebut(std::vector<Bloc> &liste,int debutA,int debutB){
	for(int i=debutB;i>debutA;i--){
	
		permuter(liste,i,i-1);
	}
}

void fusion(std::vector<Bloc> &liste, int debutA, int finA, int debutB, int finB){
	int nA=finA-debutA;
	printf("Fusion %d %d %d %d\n",debutA,finA,debutB,finB);
	if(nA==-1)
		return;
	int nB=finB-debutB;
	if(nB==-1)
		return;
	if(greaterThanColor(liste[debutA],liste[debutB])){
		envoyerAuDebut(liste,debutA,debutB);
		fusion(liste,debutA+1,finA+1,debutB+1,finB);
	}
	else{
		fusion(liste,debutA+1,finA,debutB,finB);
	}
}

void triFusion(std::vector<Bloc> &liste,int indexDebut,int indexFin){
	int n=indexFin-indexDebut;
	printf("Tri %d %d",indexDebut,indexFin);
	if(n<0)
		exit(-1);
	if(n<=0)
		return;
	int q=(indexDebut+indexFin)/2;
	triFusion(liste,indexDebut,q);
	triFusion(liste,q+1,indexFin);
	fusion(liste,indexDebut,q,q+1,indexFin);
}

void tri_bulle(std::vector<Bloc> &liste){
	for(int i=liste.size()-1;i>0;i--){
		printf("%lf\n",100-100*(double)i/liste.size());
		for(int j=0;j<i-1;j++){
			if(greaterThanGrey(liste[j],liste[j+1]))
				permuter(liste,j+1,j);
		}
	}
}

int partitionner(std::vector<Bloc> &liste,int premier,int dernier,int pivot){
	permuter(liste,pivot,dernier);
	int j=premier;
	for(int i=premier;i<dernier;i++){
		if(greaterThanColor(liste[dernier],liste[i])){
			permuter(liste,i,j);
			j++;
		}
	}
	permuter(liste,dernier,j);
	return j;
}

int choix_pivot(std::vector<Bloc> &liste,int premier,int dernier){
	return premier;
}

void tri_rapide(std::vector<Bloc> &liste,int premier, int dernier){
	if(premier<dernier){
		int pivot=choix_pivot(liste,premier,dernier);
		pivot=partitionner(liste,premier,dernier,pivot);
		tri_rapide(liste,premier,pivot-1);
		tri_rapide(liste,pivot+1,dernier);
	}
}

bool isEqual(Bloc img1,Bloc img2){
	int value1,value2;
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			value1=256*256*ImgIn[((img1.x+i)*nW+img1.y+j)*3]+256*ImgIn[((img1.x+i)*nW+img1.y+j)*3+1]+ImgIn[((img1.x+i)*nW+img1.y+j)*3+2];
			value2=256*256*ImgIn[((img2.x+i)*nW+img2.y+j)*3]+256*ImgIn[((img2.x+i)*nW+img2.y+j)*3+1]+ImgIn[((img2.x+i)*nW+img2.y+j)*3+2];
			if(value1!=value2){
				return false;
			}
		}
	}
	return true;
}

void marquer(OCTET* ImgOut,Bloc bloc){
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			ImgOut[(bloc.x+i)*nW+bloc.y+j]=255;
		}
	}
}

void detection(OCTET* ImgOut,std::vector<Bloc> liste){
	for(int i=0;i<liste.size()-1;i++){
		if(isEqual(liste[i],liste[i+1])){
			marquer(ImgOut,liste[i]);
			marquer(ImgOut,liste[i+1]);
		}
	}
}

void describeBloc(Bloc b){
	int value1;
	printf("%d %d\n",b.x,b.y);
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			value1=256*256*ImgIn[((b.x+i)*nW+b.y+j)*3]+256*ImgIn[((b.x+i)*nW+b.y+j)*3+1]+ImgIn[((b.x+i)*nW+b.y+j)*3+2];
			printf("%d %d %d ",ImgIn[((b.x+i)*nW+b.y+j)*3],ImgIn[((b.x+i)*nW+b.y+j)*3+1],ImgIn[((b.x+i)*nW+b.y+j)*3+2]);
		}
		printf("\n\n");
	}
}

int main(int argc, char* argv[]){
	int nTaille,nTaille3;
    char inputName[50];
    char outputName[50];


	int i,j;
	if (argc != 2)
	{
		printf("Usage: tailleBloc\n");
		exit(1);
	}

	sscanf(argv[1], "%d", &N);

	OCTET *ImgOut; 
    std::string dir_path = "imageBank";
    std::string imageBank="results/";
    std::string currentDatabase,database_image_path,database_mask_path,file_path;
    std::string outputFile;

			
	
	for (const auto & entry : fs::directory_iterator(dir_path)){
        database_image_path=entry.path().string(); 
        currentDatabase=entry.path().string().substr(dir_path.size()+1);
        for (const auto & entry : fs::directory_iterator(database_image_path)){
			file_path=entry.path().string();

        	sprintf(inputName,"%s",file_path.c_str());

			outputFile=imageBank+currentDatabase+file_path.substr(dir_path.size()+1+currentDatabase.size());
            //if(outputFile[outputFile.size()-3]=='j')
              //  continue;
            outputFile[outputFile.size()-1]='m';
            outputFile[outputFile.size()-2]='g';
            outputFile[outputFile.size()-3]='p';
            std::cout<< outputFile <<std::endl; 
			
        	sprintf(outputName,"%s",outputFile.c_str());


        lire_nb_lignes_colonnes_image_ppm(inputName, &nH, &nW);
        std::cout << nH << " " << nW << endl;
        nTaille = nH * nW;
        nTaille3=nTaille*3;


        allocation_tableau(ImgIn, OCTET, nTaille3);
        lire_image_ppm(inputName, ImgIn, nTaille);
        //allocation_tableau(ImgTemp, OCTET, nTaille/(Q * Q));
        allocation_tableau(ImgOut, OCTET, nTaille);
        

        printf("Splitting\n");
        std::vector<Bloc> blocs=splitting();
        

        printf("Debut du tri...\n");
        try {
            tri_rapide(blocs,0,blocs.size()-1);
        }
        catch(const std::exception& e) {
                cout << e.what() << endl;
            }
        printf("Tri fini\n");


        for(int i=0;i<nTaille;i++){
            ImgOut[i]=0;
        }
        printf("Detection...\n");
        detection(ImgOut,blocs);
        printf("Copie finie\n");
        ecrire_image_pgm(outputName, ImgOut, nH, nW);

        free(ImgIn);
    	free(ImgOut);
		}

    }
}