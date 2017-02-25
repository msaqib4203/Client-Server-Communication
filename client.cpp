#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <pthread.h>

#include "handlefile.cpp"

using namespace std;

int sockfd;

int **resmat;
pthread_mutex_t lock;

/**type of data to be sent to server**/
struct dataForServer{
 int *mat1R;/**one row of mat A**/
 int **matB;/** entire matB**/
 int rowno;/**row index**/
};
/** method to be executed by pthreads**/
void *sendMsg(void *threadarg)
{
	
 pthread_mutex_lock(&lock);	
 int *temp ;
 temp = new int[R2*C2+R1+1];/**set index*/
	
 struct dataForServer *data;
 data = (struct dataForServer*) threadarg;
 int wbytes=0;
 temp[R2*C2+C1] = data->rowno;
 cout<<"\nThread "<<data->rowno+1<<" started.."<<endl;

 cout<<"sending Matrix A row "<<data->rowno+1<<endl;
 for(int i=0;i<C1;i++){
  temp[i] = data->mat1R[i];
 }
 cout<<"sending Matrix B"<<endl;
   
 for(int i=C1;i<R2*C2+C1;i++){
  temp[i] = data->matB[(i-C1)/C2][(i-C1)%C2];
 }
   
 wbytes = send(sockfd,temp,(R2*C2+C1+1)*sizeof(int),0);/** send to server*/
 if(wbytes < 0)
  printf("Cannot write to socket error");
 else
  cout<<"Data Sent !"<<endl;
  int wrec = 0;
  int *tempresult;

  tempresult = new int[C2];
 for(int i=0;i<C2;i++){
	 int colno=-1;
	 int xx=-1;
   wrec+=recv(sockfd,&colno,sizeof(int),0);
    wrec+=recv(sockfd,&xx,sizeof(int),0);
    tempresult[colno] = xx;
 }
  resmat[data->rowno] = tempresult; 
  cout<<"Received row "<<data->rowno+1<<" of multiplied matrix"<<endl;
  cout<<"Thread "<<data->rowno+1<<" finished.."<<endl<<endl;
pthread_mutex_unlock(&lock);

}


int main(){

//int sockfd; // socket file descriptor
 int portno = 1234; // port number
 struct sockaddr_in serv_addr;
 struct hostent *server;


 sockfd = socket(AF_INET, SOCK_STREAM, 0); // generate file descriptor
 if (sockfd < 0)
  printf("ERROR opening socket");


 server = gethostbyname("127.0.0.1"); //the ip address (or server name) of the listening server.
 if (server == NULL) {
  fprintf(stderr,"ERROR, no such host\n");
  exit(0);
}
bzero((char *) &serv_addr, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
serv_addr.sin_port = htons(portno);


 if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
  printf("ERROR connecting");
  exit(0);
}
 else
  cout<<"Connection established with Server..."<<endl;

 if (pthread_mutex_init(&lock, NULL) != 0)
 {
  printf("\n mutex init failed\n");
  return 1;
 }
 /** fetch matrices from input.txt**/
 findinput();


 cout<<"Matrix A as found in input.txt\n"<<endl;
 for(int i=0;i<R1;i++){
  for(int j=0;j<C1;j++){
   cout<<" "<<mat1[i][j]<<" ";
  }
 cout<<endl;
 }
 cout<<endl;
 cout<<"Matrix B as found in input.txt\n"<<endl;
 for(int i=0;i<R2;i++){
  for(int j=0;j<C2;j++){
   cout<<" "<<mat2[i][j]<<" ";	
 }
 cout<<endl;
 }
 cout<<endl;
 if(C1!=R2){
  cout<<"Multiplication cannot be done with provided matrices!";
 exit(0);
 }

 resmat = new int*[R1];
 /** Sending dimesnions to server*/
 send(sockfd,&R1,sizeof(int),0);
 send(sockfd,&C1,sizeof(int),0);
 send(sockfd,&R2,sizeof(int),0);
 send(sockfd,&C2,sizeof(int),0);
 
 pthread_t threads[R1];
 
 struct dataForServer dfs[R1];
 cout<<"\nPreparing to send data to server...\n";
 int rc=0;
 for( int i=0; i <R1; i++ ){
 cout <<"Creating Thread "<< i+1 << endl;
 dataForServer *tt;
 tt = new dataForServer;
 tt->mat1R = mat1[i];
 tt->matB = mat2;
 tt->rowno = i;
 //tt->result = resmat[i];
 rc = pthread_create(&threads[i], NULL,sendMsg, (void *)tt);
 if(rc){
  cout << "Error:unable to create thread," << rc << endl;
  exit(-1);
  }
 }
 
   /**Blocking further access*/
 for(int i=0;i<R1;i++){
  pthread_join(threads[i], NULL);
 }
pthread_mutex_destroy(&lock);

 
 /**Displaying final matrix*/
 
 cout<<"\nResultant Matrix:\n"<<endl;
 for(int i=0;i<R1;i++){
  for(int j=0;j<C2;j++){
   cout<<" "<<resmat[i][j]<<" ";
  }
  cout<<endl;
 }
 cout<<endl<<"Program Terminated....";
 return 0;
 }







