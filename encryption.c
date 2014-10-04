/*
*Noah Herron
*CPSC 360 - Program 1 - Feb 7th, 2013
*This program encrypts input by XOR byte by byte with the 
*provided key, there is a decryption program to reverse the text
*/
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0


int main(int argc, char *argv[]){
  //display message to stdout
  char key_note[]={"Enter encryption key: "};
  write(STDOUT_FILENO,key_note,sizeof(key_note));
  
  int legalKey = FALSE;
  char key[512];//buffer for key storage
  //wait until the user enters a legal key
  while(!legalKey){
    //store input into key
    int nKeyBytes =  read(STDIN_FILENO,key,sizeof(key));
    //key + \n = 2
    if(nKeyBytes>2){
      //if more then one character is entered for key diplay message
      char error[]={"Please enter only 1 character for the key!\n"};
      write(STDERR_FILENO,error,sizeof(error));
    }else{
      //trucate the key to only hold one value,overwrite new line character
      key[1] = '\0';
      legalKey = TRUE;
    }
  }

  //display message to stdout
  char file_note[]={"Enter file name for encrytion,\nif the file exists new text will be appended: "};
  write(STDOUT_FILENO,file_note,sizeof(file_note));
  //buf is the buffer for filename
  char buf[512];
  char *filename = buf;
  //store input into filename
  int nbytes = read(STDIN_FILENO,filename,(sizeof(buf)));
  //overwrite newline character
  filename[nbytes-1]='\0';
  //create file from filename, append new data
  int fd = open(filename,O_WRONLY|O_CREAT|O_APPEND,S_IREAD|S_IWRITE);
  //if error occurs while opening
  if(fd < 0){
    char error[]={"Error opening file...\n"};
    write(STDERR_FILENO,error,sizeof(error));
    exit(-1);
  }

  char message[]={"****Enter text, Ctrl+D to quit****\n"};
  write(STDOUT_FILENO,message,sizeof(message));
  
  char buffer;
  int rbytes=0, wbytes=0;
  //read one byte into buffer
  while( (rbytes = read(STDIN_FILENO,&buffer,1)) >0){
    if(buffer!=EOF){
        //encrypt input
	char encrypt = buffer^key[0];
        //write encrypted input to stdout
        wbytes = write(fd,&encrypt,rbytes);
    }else if(buffer == EOF){
      //once ctrl+d is pressed end program
      break;
    }else if(rbytes < 0){
      char Rerror[]={"Error reading form file...\n"};
      write(STDERR_FILENO,Rerror,sizeof(Rerror));
      exit(-1);
    }else if(wbytes < 0){
      char Werror[]={"Error writing to file...\n"};
      write(STDERR_FILENO,Werror,sizeof(Werror));
      exit(-2);
    }   
  }
  close(fd);

  return 0;
}
