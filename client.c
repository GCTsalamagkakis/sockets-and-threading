#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<time.h>

int main()
    {
    int sock,i, num, to_send;
    struct sockaddr_in server;
    srand(time(NULL));
    //tcp socket
    sock= socket(AF_INET, SOCK_STREAM, 0);
    if(sock==-1){
        printf("could not create socket");
        exit(-1);
    }
    puts("socket created\n");
    
    //server info
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    
    //connect to server
    connect(sock, (struct sockaddr *)&server, sizeof(server)); 
    puts("connected\n");
    //stelnw gia ka8e polh th timh pou epi8umw na mpei ston pinaka fuel
    for(i=0;i<9;i++)
	{
		num=rand()%100+1;
        to_send=htonl(num);
        write(sock, &to_send, sizeof(to_send));
        
    }
    printf("client done");
    close(sock);
    return 0;
}
