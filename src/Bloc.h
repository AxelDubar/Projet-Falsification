#ifndef BLOC_H
#define BLOC_H

class Bloc{
	public:
	int x,y;
	std::vector<double> features;
	Bloc(int x,int y){
		this->x=x;
		this->y=y;
	}
    Bloc(){
        this->x=0;
        this->y=0;
    }
};

#endif