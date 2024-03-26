#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <chrono>
using namespace std;

int maxDots;

class Triangle{
   
private:
    double x[3];
    double y[3];
    double perim;
    double lengths[3]; //0- Between x/y[0] and x/y[1],1- Between x/y[1] and x/y[2], 2- Between x/y[2] and x/y[0] 
    double area;
    double lineRepresenation[3][2]; //[0][0] - length 0's slope, [0][1] length 0's y int
    int numDots;

    int gcd(int a, int b){
        a = abs(a);
        b = abs(b);
        if(a==0){
            return abs(b);
        }
        return gcd(b%a,a);
    }

public:
    void calcLengths(){
        lengths[0] = sqrt(pow(x[0]-x[1],2) + pow(y[0]-y[1],2));
        lengths[1] = sqrt(pow(x[1]-x[2],2) + pow(y[1]-y[2],2));
        lengths[2] = sqrt(pow(x[2]-x[0],2) + pow(y[2]-y[0],2));
        for(int i =0; i<(sizeof(lengths)/sizeof(*lengths));i++){
            cout<<"Length @"<<i<<" is:"<<lengths[i]<<endl;
        }
        perim = lengths[0] + lengths[1] + lengths[2];
    
    }
    double getArea(){
       //Herons Formula 
       double s = perim/2;
       return sqrt(s*(s-lengths[0])*(s-lengths[1])*(s-lengths[2]));
    }
    void calcLineRepresntation(){
        for(int i = 0; i<sizeof(lineRepresenation)/sizeof(*lineRepresenation); i++){
            lineRepresenation[i][0] = (y[i]-y[(i+1)%2])/(x[i]-x[(i+1)%2]);
            lineRepresenation[i][1] = y[i] - lineRepresenation[i][0]*x[i];
        }
    }
    int getNumDots(){
        return (x[1]*y[2] - x[2]*y[1] + 2 - gcd(x[1]-x[2],y[2]-y[1]) - gcd(x[2],y[2]) - gcd(x[1],y[1]))/2;
    }
//Triangle to string - Converts all relevant Information to a String
    string ttos(){ 
        string gbes;
        gbes += "Coordinates: ";
        for(int i = 0; i< sizeof(x)/sizeof(*x);i++){
            gbes += (" ("+to_string(x[i]) +" , " + to_string(y[i]) + ") ");
        }
        gbes += " Lengths: ";
        for(int i = 0; i< sizeof(x)/sizeof(*x);i++){
            gbes += (" ("+to_string(lengths[i]) +" , " + to_string(lengths[i]) + ") ");
        }
        gbes += " Permieter: "+to_string(perim);
        gbes += " Area: " + to_string(area);
        gbes += " numDots: " + to_string(numDots);

        return gbes;
    }
    double getPerimeter(){
        return perim;
    }
    Triangle(int tempx[3],int tempy[3]){
        copy(tempx,tempx+3,x);
        copy(tempy,tempy+3,y);
        if(y[1]>y[2]){
            throw runtime_error("ERROR Y1 CANNOT BE GREATER THAN Y2");
        }
        for(int i = 0; i< sizeof(x)/sizeof(*x);i++){
            cout<<"{"<<x[i]<<","<<y[i]<<"}"<<endl;
        }
        calcLineRepresntation();
        calcLengths();
        area = getArea();
        numDots = getNumDots();
        cout<<area<<"<--Area"<<endl;
    }
};

int runTest(int maxDots, string file){ // all possible Triangles within maxDotsxMaxDots should be covered
    vector<Triangle> tris;
    ofstream dir;
    dir.open(file);
    for(int i = 1; i<maxDots;i++){ //loop for y2
        for(int j = 1; j<maxDots;j++){//loop for x2
            for(int k = 0; k<i;k++){//Loop for y1
                for(int l = (k*j)/i + 1;l<maxDots;l++){//Loop for x1 - sets l(counter) to k*j/l +1 as it takes the line representation of length[2] & and checks for the lowest integer(rounding up, hence the +1) which will result in a triangle where numDots is not negative
                    int x[] = {0,l,j};
                    int y[] = {0,k,i};
                    Triangle z(x,y);
                    //Iterate thru all already in vector and put it in order based on nuiDots, or replace based on perimeter
                    tris.push_back(z);
                }
            }
        }
    }
    int size_was = tris.size();

 

    for(int i = 0; i<tris.size();i++){
        for(int j = 0; j< tris.size()-1;j++){
            if(tris[j].getNumDots()>tris[j+1].getNumDots()){
                Triangle temp = tris[j];
                tris[j] = tris[j+1];
                tris[j+1] = temp;
            }
            else if(tris[j].getNumDots()==tris[j+1].getNumDots()){
                tris.erase(tris.begin()+  ((tris[j].getPerimeter()<tris[j+1].getPerimeter()) ? j+1:j));
                j--;
            }
        }
        
    }

    for(Triangle t : tris){
        dir << t.ttos() + "\n";
        cout<<t.ttos()<<endl;
    }

    return size_was;

}

int main(){
    int range;
    string outputFile;
    cout<<"Enter Range(Check all Triangles where x<X and y<X): "<<endl;
    cin >> range;
    cout<<"Enter Name of Output File(including file extension e.g. 'results.txt'): "<<endl;
    cin >> outputFile;
    cout<<"BEGINNING PROGRAM"<<endl;
    auto start = chrono::high_resolution_clock::now();
    int size_was = runTest(range,outputFile);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::minutes>(stop - start);
    ofstream dir;
    dir.open(outputFile);
    cout<<"PROGRAM FINISHED IN "<<to_string(duration.count())<<" MINUTES"<<endl;
    cout<<size_was<<" TRIANGLES INDEXED"<<endl;
    dir<<"PROGRAM FINISHED IN "<<to_string(duration.count())<<" MINUTES \n";
    dir<<size_was<<" TRIANGLES INDEXED";
    return 0;
}

