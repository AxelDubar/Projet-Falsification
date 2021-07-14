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

using namespace std;

bool isGrey=false;
OCTET *ImgIn,*ImgGrey;
std::vector<std::vector<std::vector<double>>> dcts;
std::vector<std::vector<std::vector<double>>> dctsRed;
std::vector<std::vector<std::vector<double>>> dctsGreen;
std::vector<std::vector<std::vector<double>>> dctsBlue;
int nW,nH;
int N;

std::vector<std::vector<double>> dctMatrix;
std::vector<std::vector<double>> dctMatrixTranspos;


void greyScaleImage(){
	printf("debut greyscale\n");
	int nR,nG,nB;
	for(int i=0;i<nH;i++){
		for(int j=0;j<nW;j++){
			nR=ImgIn[(i*nW+j)*3];
			nG=ImgIn[(i*nW+j)*3+1];
			nB=ImgIn[(i*nW+j)*3+2];
			ImgGrey[i*nW+j]=0.3*nR+0.587*nG+0.114*nB;
		}
	}
}

double C(int x){
	if(x==0)
		return sqrt(1.0/N);
	return sqrt(2.0/N);
}

double dct(Bloc img,int i,int j){
	double somme=0;
	for(int x=0;x<N;x++){
		for(int y=0;y<N;y++){
			somme+=ImgGrey[(img.x+x)*nW+img.y+y]*cos( ((2*x+1)*i*M_PI)/(2*N) )*cos(cos( ((2*y+1)*j*M_PI)/(2*N) ));
		}
	}
	double result=(1.0/sqrt(2))*C(i)*C(j)*somme;
	return result;
}

double dctR(Bloc img,int i,int j){
	double somme=0;
	for(int x=0;x<N;x++){
		for(int y=0;y<N;y++){
			somme+=ImgIn[((img.x+x)*nW+img.y+y)*3]*cos( ((2*x+1)*i*M_PI)/(2*N) )*cos(cos( ((2*y+1)*j*M_PI)/(2*N) ));
		}
	}
	double result=(1.0/sqrt(2))*C(i)*C(j)*somme;
	return result;
}
double dctG(Bloc img,int i,int j){
	double somme=0;
	for(int x=0;x<N;x++){
		for(int y=0;y<N;y++){
			somme+=ImgIn[((img.x+x)*nW+img.y+y)*3+1]*cos( ((2*x+1)*i*M_PI)/(2*N) )*cos(cos( ((2*y+1)*j*M_PI)/(2*N) ));
		}
	}
	double result=(1.0/sqrt(2))*C(i)*C(j)*somme;
	return result;
}
double dctB(Bloc img,int i,int j){
	double somme=0;
	for(int x=0;x<N;x++){
		for(int y=0;y<N;y++){
			somme+=ImgIn[((img.x+x)*nW+img.y+y)*3+2]*cos( ((2*x+1)*i*M_PI)/(2*N) )*cos(cos( ((2*y+1)*j*M_PI)/(2*N) ));
		}
	}
	double result=(1.0/sqrt(2))*C(i)*C(j)*somme;
	return result;
}

void initMatrix(){
	for(int i=0;i<N;i++){
		std::vector<double> row;
		for(int j=0;j<N;j++){
			if(i==0){
				row.push_back(1.0/sqrt(N));
			}
			else{
				row.push_back(sqrt(2.0/N) *cos((2*j+1)*i*M_PI/(2*N)));
			}
		}
		dctMatrix.push_back(row);
	}


	for(int i=0;i<N;i++){
		std::vector<double>row;
		for(int j=0;j<N;j++){
			row.push_back(dctMatrix[j][i]);
		}
	dctMatrixTranspos.push_back(row);
	}
}

void computeDCTTransform(Bloc img,std::vector<std::vector<double>> &result,std::vector<std::vector<double>> &resultR,std::vector<std::vector<double>> &resultG,std::vector<std::vector<double>> &resultB){
	std::vector<std::vector<double>> temp,tempR,tempG,tempB;
	double val=0, valR=0,valG=0,valB=0;
	for(int i=0;i<N;i++){
		std::vector<double> row,rowR,rowG,rowB;
		for(int j=0;j<N;j++){
			val=0.0;valR=0.0;valG=0.0;valB=0.0;
			for(int k=0;k<N;k++){
				val+=dctMatrix[i][j]*(ImgGrey[(img.x+i)*nW+img.y+j]-128);
				valR+=dctMatrix[i][j]*(ImgIn[((img.x+i)*nW+img.y+j)*3]-128);
				valG+=dctMatrix[i][j]*(ImgIn[((img.x+i)*nW+img.y+j)*3+1]-128);
				valB+=dctMatrix[i][j]*(ImgIn[((img.x+i)*nW+img.y+j)*3+2]-128);
			}
			row.push_back(val);rowR.push_back(valR);rowG.push_back(valG);rowB.push_back(valB);
		}
		temp.push_back(row);tempR.push_back(rowR);tempG.push_back(rowG);tempB.push_back(rowB);
	}
	for(int i=0;i<N;i++){
		std::vector<double> row,rowR,rowG,rowB;
		for(int j=0;j<N;j++){
			val=0.0;valR=0.0;valG=0.0;valB=0.0;
			for(int k=0;k<N;k++){
				val+=temp[i][j]*dctMatrixTranspos[i][j];
				valR+=tempR[i][j]*dctMatrixTranspos[i][j];
				valG+=tempG[i][j]*dctMatrixTranspos[i][j];
				valB+=tempB[i][j]*dctMatrixTranspos[i][j];
			}
			row.push_back(val);rowR.push_back(valR);rowG.push_back(valG);rowB.push_back(valB);
		}
		result.push_back(row);resultR.push_back(row);resultG.push_back(row);resultB.push_back(row);
	}
	
}

void computeFeatures(Bloc &bloc){
	/*std::vector<std::vector<double>> result,resultR,resultG,resultB,temp,tempR,tempG,tempB;
	double val=0, valR=0,valG=0,valB=0;
	for(int i=0;i<2;i++){
		std::vector<double> row;
		for(int j=0;j<N;j++){
			val=0.0;
			for(int k=0;k<N;k++){
				val+=dctMatrix[i][j]*(ImgGrey[(bloc.x+i)*nW+bloc.y+j]-128);
			}
			row.push_back(val);
		}
		temp.push_back(row);
	}
	for(int i=0;i<2;i++){
		std::vector<double> rowR,rowG,rowB;
		for(int j=0;j<N;j++){
			valR=0.0;valG=0.0;valB=0.0;
			for(int k=0;k<N;k++){
				valR+=dctMatrix[i][j]*(ImgIn[((bloc.x+i)*nW+bloc.y+j)*3]-128);
				valG+=dctMatrix[i][j]*(ImgIn[((bloc.x+i)*nW+bloc.y+j)*3+1]-128);
				valB+=dctMatrix[i][j]*(ImgIn[((bloc.x+i)*nW+bloc.y+j)*3+2]-128);
			}
			rowR.push_back(valR);rowG.push_back(valG);rowB.push_back(valB);
		}
		tempR.push_back(rowR);tempG.push_back(rowG);tempB.push_back(rowB);
	}


	for(int i=0;i<2;i++){
		std::vector<double> row;
		for(int j=0;j<2;j++){
			val=0.0;
			for(int k=0;k<N;k++){
				val+=temp[i][j]*dctMatrixTranspos[i][j];
			}
			row.push_back(val);
		}
		result.push_back(row);
	}

	for(int i=0;i<1;i++){
		std::vector<double> rowR,rowG,rowB;
		for(int j=0;j<1;j++){
			valR=0.0;valG=0.0;valB=0.0;
			for(int k=0;k<N;k++){
				valR+=tempR[i][j]*dctMatrixTranspos[i][j];
				valG+=tempG[i][j]*dctMatrixTranspos[i][j];
				valB+=tempB[i][j]*dctMatrixTranspos[i][j];
			}
			rowR.push_back(valR);rowG.push_back(valG);rowB.push_back(valB);
		}
		resultR.push_back(rowR);resultG.push_back(rowG);resultB.push_back(rowB);
	}
*/
	int nR=0,nG=0,nB=0;
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			nR+=ImgIn[((bloc.x+i)*nW+bloc.y+j)*3];
			nG+=ImgIn[((bloc.x+i)*nW+bloc.y+j)*3+1];
			nB+=ImgIn[((bloc.x+i)*nW+bloc.y+j)*3+2];
		}
	}
	int nTaille=(N*N);
	nR=nR/nTaille;nG=nG/nTaille;nB=nB/nTaille;
	
	
	bloc.features.push_back(dct(bloc,0,0));
	bloc.features.push_back(dct(bloc,0,1));
	bloc.features.push_back(dct(bloc,1,0));
	bloc.features.push_back(dctR(bloc,0,0));
	bloc.features.push_back(dctG(bloc,0,0));
	bloc.features.push_back(dctB(bloc,0,0));
	
	bloc.features.push_back(nR);
	bloc.features.push_back(nG);
	bloc.features.push_back(nB);

	

}

std::vector<Bloc> splitting(){
	int nbLigne=nH-N;
	int nbCol=nW-N;
	int n=0;
	std::vector<Bloc> blocs;
	for(int i=0;i<=nbLigne;i++){
		for(int j=0;j<=nbCol;j++){
			blocs.push_back(Bloc(i,j));
			n++;
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
			value1=ImgGrey[((img1.x+i)*nW+img1.y+j)];
			value2=ImgGrey[((img2.x+i)*nW+img2.y+j)];
			if(value1>value2){
				return false;
			}
			else if(value1<value2){
				return true;
			}
		}
	}
	return false;
}

bool greaterThan(Bloc img1,Bloc img2){
	for(int i=0;i<img1.features.size();i++){
		if(img1.features[i]>img2.features[i])
			return true;
		if(img1.features[i]<img2.features[i])
			return false;
	}
	return false;
}
void permuter(std::vector<Bloc> &liste,int i,int j){
	Bloc tmp=liste[i];
	liste[i]=liste[j];
	liste[j]=tmp;
	
}


int partitionner(std::vector<Bloc> &liste,int premier,int dernier,int pivot){
	//printf("%d %d %d\n",premier,dernier,pivot);
	permuter(liste,pivot,dernier);
	int j=premier;
	for(int i=premier;i<dernier;i++){
		if(greaterThan(liste[dernier],liste[i])){
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


double euclidianDistBloc(Bloc img1,Bloc img2){
	double result=0;
	int nR1,nG1,nB1,nR2,nG2,nB2;
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			nR1=ImgIn[((img1.x+i)*nW+j+img1.y)*3];
			nG1=ImgIn[((img1.x+i)*nW+j+img1.y)*3+1];
			nB1=ImgIn[((img1.x+i)*nW+j+img1.y)*3+2];

			nR2=ImgIn[((img2.x+i)*nW+j+img2.y)*3];
			nG2=ImgIn[((img2.x+i)*nW+j+img2.y)*3+1];
			nB2=ImgIn[((img2.x+i)*nW+j+img2.y)*3+2];
			result+=pow(nR1-nR2,2);result+=pow(nG1-nG2,2);result+=pow(nB1-nB2,2);
		}
		
	}
	result=sqrt(result);
	return result;
}

double euclidianDistFeatures(Bloc img1,Bloc img2){
	double result=0;
	for(int i=0;i<img1.features.size();i++){
		result+=pow(img1.features[i]-img2.features[i],2);
	}
	result=sqrt(result);
	return result;
}

bool vectorClose(Bloc img1,Bloc img2,double threshold){
	if(euclidianDistFeatures(img1,img2)<=threshold)
		return true;
	return false;
}

void marquer(OCTET* ImgOut,Bloc bloc){
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			ImgOut[(bloc.x+i)*nW+bloc.y+j]=255;
		}
	}
}

int nbShiftSimilar(std::vector<int> shift,std::vector<std::vector<int>> shifts){
	int res=0;
	for(int i=0;i<shifts.size();i++){
		std::vector<int> currentShift=shifts[i];
		if(currentShift[0]==shift[0] && currentShift[1]==shift[1])
			res++;
	}
	return res;
}

int shiftIsPresent(std::vector<int> shift,std::vector<std::vector<int>> shifts){
	for(int i=0;i<shifts.size();i++){
		std::vector<int> currentShift=shifts[i];
		if(currentShift[0]==shift[0] && currentShift[1]==shift[1])
			return i;
	}
	return -1;
}

void detection(OCTET* ImgOut,std::vector<Bloc> liste,double T1,int Tn,int Ts,int T2){
	std::vector<std::vector<int>> positions,shifts;
	std::vector<int> shiftOccurence;
	std::vector<int> shiftIndex;
	
	for(int i=0;i<liste.size()-2;i++){
		for(int j=i+1;(j<i+Tn && j<liste.size()-1);j++){
			if(vectorClose(liste[i],liste[j],T1)){
				printf("Trouvé en i=%d %d %d et %d %d\n",i,liste[i].x,liste[i].y,liste[j].x,liste[j].y);
				
				std::vector<int> position,shift;
				position.push_back(i);
				position.push_back(j);
				positions.push_back(position);
				shift.push_back(abs(liste[i].x-liste[j].x));
				shift.push_back(abs(liste[i].y-liste[j].y));
				
				int index=shiftIsPresent(shift,shifts);
				if(index==-1){
					shiftIndex.push_back(shifts.size());
					shifts.push_back(shift);
					shiftOccurence.push_back(1);
				}
				else{
					shiftIndex.push_back(index);
					shiftOccurence[index]++;
				}
			}
		}
	}
	printf("%ld candidtats detectees\n",positions.size());
	std::vector<std::vector<int>> suspicious;

	for(int i=0;i<positions.size();i++){
	//	printf("%d/%ld\n",i,positions.size());
		int n=shiftOccurence[shiftIndex[i]];
		printf("%d\n",n);
		//if(n<Ts){
			suspicious.push_back(positions[i]);
		//}
	}
	
	printf("%ld positions suspectes\n",suspicious.size());
	for(int i=0;i<suspicious.size();i++){
		//printf("%d/%ld\n",i,suspicious.size());
		std::vector<int> currentPosition=suspicious[i];
		double dist=euclidianDistBloc(liste[currentPosition[0]],liste[currentPosition[1]]);
		if(dist<T2){
			marquer(ImgOut,liste[currentPosition[0]]);
			marquer(ImgOut,liste[currentPosition[1]]);
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

int main(int argc, char* argv[])
{
	char cNomImgLue[250], cNomImgEcrite[250];
	int nTaille,nTaille3;

	int i,j;

	if (argc != 4)
	{
		printf("Usage: ImageIn.ppm ImageOut.pgm tailleBloc\n");
		exit(1);
	}

	sscanf(argv[1], "%s", cNomImgLue);
	sscanf(argv[2], "%s", cNomImgEcrite);
	sscanf(argv[3], "%d", &N);

	double T1=1.00;
	int T2=110000,Ts=10,Tn=16;

	OCTET *ImgOut; 

	lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
	std::cout << nH << " " << nW << endl;
	nTaille = nH * nW;
	nTaille3=nTaille*3;

	initMatrix();
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			printf("%lf	",dctMatrix[i][j]);
		}
		printf("\n");
	}
	
	printf("\n");

	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			printf("%lf	",dctMatrixTranspos[i][j]);
		}
		printf("\n");
	}


	allocation_tableau(ImgIn, OCTET, nTaille3);
	allocation_tableau(ImgGrey, OCTET, nTaille);
	lire_image_ppm(cNomImgLue, ImgIn, nTaille);
	//allocation_tableau(ImgTemp, OCTET, nTaille/(Q * Q));
	greyScaleImage();

	
	


	allocation_tableau(ImgOut, OCTET, nTaille);
	
	char name_img[250];

	printf("Splitting\n");
	std::vector<Bloc> blocs=splitting();
	
/*	for (int i = 0; i < blocs.size(); i++)
	{
		//if(ImgGrey[blocs[i].x*nW+blocs[i].y]==255)
			printf("%d	%d	%d	%d\n",ImgGrey[blocs[i].x*nW+blocs[i].y],ImgIn[(blocs[i].x*nW+blocs[i].y)*3],ImgIn[(blocs[i].x*nW+blocs[i].y)*3+1],ImgIn[(blocs[i].x*nW+blocs[i].y)*3+2]);
	}*/

	for(int i=0;i<blocs.size();i++){
		computeFeatures(blocs[i]);
	}

	printf("le premier est %d %d de pixel %d %d %d\n",blocs[blocs.size()].x,blocs[blocs.size()].y,ImgIn[blocs[0].x*nW+blocs[0].y],ImgIn[blocs[0].x*nW+blocs[0].y+1],ImgIn[blocs[0].x*nW+blocs[0].y+2]);
	
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
	detection(ImgOut,blocs,T1,Tn,Ts,T2);
	printf("Copie finie\n");
	
	ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);

	free(ImgIn);
	free(ImgGrey);
	free(ImgOut);
	
	return 1;
}

