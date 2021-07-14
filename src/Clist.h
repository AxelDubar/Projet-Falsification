#ifndef CLIST_H
#define CLIST_H

#include "Bloc.h"
class Maillon{
	public:
	Bloc img;
	Maillon* next;
	Maillon(Bloc img,Maillon *next){
		this->img=img;
		this->next=next;
	}

};

void addHead(Maillon *&list,Maillon *element){
    element->next=list->next;
    list->next=element;
}

#endif
