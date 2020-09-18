
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_LINE 256
#define SERVER_PORT 5432    /* unused port number */

int main(int argc, const char * argv[]) {
    

    struct hostent *hp;      /* host's information*/
    struct sockaddr_in sin;  /* address data structure */
    char *host;
    char buf[MAX_LINE];
    int s;
    int len;
    
    
    if(argc==2){
        host=argv[1];
    }
    else{
        fprintf(stderr,"usage: simplex-talk host\n");
        return -1;
    }
    
    hp=gethostbyname(host);
    if(!hp){
        fprintf(stderr, "simplex-talk:unknown host:%s\n",host);
        return -1;
    }
    
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family=AF_INET;  /* Use the socket to connect to the Internet */
    bcopy(hp->h_addr_list[0],(char *)&sin.sin_addr,hp->h_length);
    sin.sin_port=htons(SERVER_PORT);
    
    if((s=socket(PF_INET, SOCK_STREAM, 0))<0){     /* TCP is used here */
        perror("simplex-talk: socket");
        return -1;
    }
    if(connect(s, (struct sockaddr *)&sin, sizeof(sin))<0){
        perror("simplex-talk: socket");
        close(s);
        return -1;
    }
    
    while(fgets(buf,sizeof(buf),stdin)){
        buf[MAX_LINE-1]='\0';
        len=strlen(buf)+1;
        send(s,buf,len,0);
    }
    
    return 0;
}
