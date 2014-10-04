/*
*Noah Herron
*CPSC 360 - Program 1 - Feb 7th, 2013
*This program decrypts input by XOR byte by byte with the 
*provided key, there is a encryption program to reverse the text
*/
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[]){
  //display message to stdout
  char key_note[]={"Enter decryption key: "};
  write(STDOUT_FILENO,key_note,sizeof(key_note));

  int legalKey = FALSE;
  char key[512]; //buffer for key storage
  //wait until the user enters a legal key
  while(!legalKey){
    //store input into key
    int nKeyBytes =  read(STDIN_FILENO,key,sizeof(key));
    //key + \n = 2
    if(nKeyBytes>2){
      //if more than one character is entered for key display message
      char error[]={"Please enter only 1 character for the key!\n"};
      write(STDERR_FILENO,error,sizeof(error));
    }else{
      //truncate key to only hold one value, overwrite the new line character
      key[1] = '\0';
      legalKey = TRUE;
    }
  }

  //display message to stdout
  char file_note[]={"Enter file name for decrytion: "};
  write(STDOUT_FILENO,file_note,sizeof(file_note));
  //buf is the buffer for filename
  char buf[512];
  char *filename = buf;
  //store input into filename
  int nbytes = read(STDIN_FILENO,filename,(sizeof(char)*100)-1);
  //overwrite new line character
  filename[nbytes-1]='\0';
  //open file for decryption
  int file_descripter =open(filename,O_RDONLY,S_IREAD);
  //if error occurs while opening
  if(file_descripter < 0){
    char error[]={"Error opening file...\n"};
    write(STDERR_FILENO,error,sizeof(error));
    exit(-1);
  }

  char buffer;
  int rbytes=0, wbytes=0;
  //read in one byte into buffer
  while( (rbytes = read(file_descripter,&buffer,1)) >0){
    if(buffer!=EOF){
        //decrypt byte
	char decrypt = buffer^key[0];
        //write decrypted byte to stdout
        write(STDOUT_FILENO,&decrypt,rbytes);
    }else if(rbytes<0){
      char Rerror[]={"Error reading form file...\n"};
      write(STDERR_FILENO,Rerror,sizeof(Rerror));
      exit(-1);
    }else if(wbytes < 0){
      char Werror[]={"Error writing to file...\n"};
      write(STDERR_FILENO,Werror,sizeof(Werror));
      exit(-2);
    }
  }
  close(file_descripter);
  return 0;
}
