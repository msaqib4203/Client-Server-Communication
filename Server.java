/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

//package clientserver;
import java.io.*;
import java.util.*;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.*;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author msaqib
 */

class ServerThread extends Thread {
 int resrow[];
 int colno;
 int rowno;
 Server s;
 Socket clientSocket;
 public ServerThread(Server s, int rowno,int colno,Socket clientSocket) {
  this.s = s;
  this.colno = colno;
  this.rowno = rowno;
  this.clientSocket = clientSocket;
 }
 public void run() {
  try {
   this.s.sendtoClient(this.rowno,this.colno, clientSocket);
  } catch (IOException ex) {
   // Logger.getLogger(Server.class.getName()).log(Level.SEVERE, null, ex);
  }
 }

}




public class Server {

 public static int mat1[][];
 public static int mat2[][];
 public static int r1, c1, r2, c2;
 public static int little2big(int i) {
  return (i & 0xff) << 24 | (i & 0xff00) << 8 | (i & 0xff0000) >> 8 | (i >> 24) & 0xff;
 }
 public static void main(String[] args) throws IOException {

  ServerSocket serverSocket = null;
  Socket clientSocket = null;

  serverSocket = new ServerSocket(1234); // 1234 is port number
  System.out.println("Waiting for client at " + serverSocket);
  clientSocket = serverSocket.accept(); // blocks and listen until a connection is made. 
  System.out.println("Client connected at" + clientSocket);

System.out.println("\nWaiting to receive data from client...\n");
  DataInputStream ds = new DataInputStream(clientSocket.getInputStream());
  /** receive dimesion*/
  r1 = little2big(ds.readInt());
  c1 = little2big(ds.readInt());
  r2 = little2big(ds.readInt());
  c2 = little2big(ds.readInt());

  mat1 = new int[r1][c1];
  mat2 = new int[r2][c2];


  /** Receive data*/
  int temprow[];
 ServerThread t[][] = new ServerThread[r1][c2];
  Server s = new Server();
 
  for (int k = 0; k < r1; k++) {
   temprow = new int[c1];
   
   for (int i = 0; i < c1; i++) {
    temprow[i] = little2big(ds.readInt());
   }
   
   for (int i = c1; i < c1 + r2 * c2; i++) {
    mat2[(i - c1) / c2][(i - c1) % c2] = little2big(ds.readInt());
   }
   
   int r = little2big(ds.readInt());
   mat1[r] = temprow;
   System.out.println("\nReceived row " + (r + 1) + " of Matrix A\nReceived matrix B\n");
    for (int i = 0; i < c2; i++) {

   System.out.println("Creating thread " + (r+1)+","+(i + 1));
   t[k][i] = new ServerThread(s,r,i,clientSocket);
   t[k][i].start();


  }
   
  }

 /**Blocking further access*/
  try {
   for(int i = 0; i < r1; i++)
	for(int j=0;j<c2;j++)
    t[i][j].join();
  } catch (Exception e) {

  } finally {
   serverSocket.close();
   clientSocket.close();
   System.out.println("\nConnections Closed....!");
  }
 
 }
 synchronized void sendtoClient(int rowno,int colno, Socket clientSocket) throws IOException {

  DataOutputStream ds = new DataOutputStream(clientSocket.getOutputStream());
  int res = 0;
  System.out.println("\nStarting thread " +(rowno + 1)+","+(colno + 1));
  System.out.println("Multiplying... Row "+(rowno + 1)+" of matrix A with ,"+"Column " + (colno + 1) + " of matrix B");
   for (int j = 0; j < r2; j++) {
    res += mat1[rowno][j] * mat2[j][colno];
   }
   
  ds.write(ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt(colno).array());
  ds.write(ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt(res).array());

  System.out.println("\nCalculated element[" +(rowno+1)+","+(colno + 1) + "] of final matrix sent to client...");
  System.out.println("Thread " +(rowno + 1)+","+ (colno + 1) + " finished..");
 }


}
