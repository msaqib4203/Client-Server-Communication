#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>

using namespace std;

int R1,C1,R2,C2;
int **mat1,**mat2;

void findinput(){
	
	//return_package = new int*[3];
	//int *dimensions;
	//dimensions = new int[4];
	//int **mat1;
	//int **mat2;
 ifstream file;
 file.open("input.txt");
 string buffer;
 int r1=0,r2=0;
 bool matrix1 = true;

/**Counting rows*/
 while(getline(file,buffer)){
  if(buffer.length()==0){
   matrix1 = false;
   continue;
   }
   switch(matrix1){
    case true:
     r1++; 
     break;
    case false:
     r2++;
     break;
   }
 }
//clear buffer
 file.clear();
 file.seekg(0);

 getline(file,buffer);

 /** Counting columns*/
 int c1=1,c2=1;
 for(int i=0;i<buffer.length();i++){
  if(buffer[i]==',')
   c1++;
 }
 file.clear();
 file.seekg(0);

 for(int i=0;i<r1+2;i++){
  getline(file,buffer);
 }
 for(int i=0;i<buffer.length();i++){
  if(buffer[i]==',')
	c2++;
 }
 /**Setting values to matrix A and matrix B*/
 
 file.clear();
 file.seekg(0);
 mat1 = new int*[r1];
 mat2 = new int*[r2];
 for(int i=0;i<r1;i++){
  mat1[i] = new int[c1];
  getline(file,buffer);
  int x=0,count=0,j=0;
  for(j=0;j<buffer.length();j++){
   if(buffer[j]==','){
    mat1[i][count++] = stoi(buffer.substr(x,j-x));
    x = j+1;
   }
  }
  mat1[i][count] = stoi(buffer.substr(x,j-x));
 }
 cout<<endl;
 getline(file,buffer);

 for(int i=0;i<r2;i++){
  mat2[i] = new int[c2];
  getline(file,buffer);
  int x=0,count=0,j=0;
  for(j=0;j<buffer.length();j++){
   if(buffer[j]==','){
    mat2[i][count++] = stoi(buffer.substr(x,j-x));
    x = j+1;
   }
  }
  mat2[i][count] = stoi(buffer.substr(x,j-x));
 }
 file.close();
R1 = r1;
C1 = c1;
R2 = r2;
C2 = c2;

}
