#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>

int counter=0;
int client_num=0;
int cities[10][10]; 
int fuel[10][10];
int cost[10][10];
int numbers[9];

//h function gia na eksuphretountai polloi clients
void *thread_func(void* client_sock)
    {
    	
    //typecast gia na mporesw na parw to socket tou client
    int sock = *(int*)client_sock;
    int received, read_num, num, return_status, i, j;
    
    //oso kapoios stelnei, egw diavazw
    while(received=read(sock, &read_num, sizeof(int)))
	{
        if(received>0)
		{
		    //metatroph tou ari8mou apo network se host
            num=ntohl(read_num);
            printf("received number is: %d\n", num);
            printf("count: %d\n", counter);
            numbers[counter]=num;
            counter++;
        }
    }
    //dinw tis times ekei poy mou edeikse o client
    j=0;
    for(i=0;i<10;i++){
        if(i!=client_num){
            fuel[client_num][i]=numbers[j];
            fuel[i][client_num]=numbers[j];
            j++;
        }
    }
    for(i=0;i<10;i++){
     for(j=0;j<10;j++) printf("%d \t",fuel[i][j]);
     printf("\n");
    }
    client_num++;
    counter=0;
    
    return 0;
}

//function gia na upologizei diarkws ta kosth
void *thread_func2(void *_)
    {
    	
    
    int i,j;
    int flag, random;
    while(1){
        //elegxw an opoiadhpote 8esh einai 0 pera apo autes pou prepei na einai
        flag=0;
        for(j=0;j<9;j++){
            for(i=j+1;i<10;i++){
                if(fuel[i][j]!=0) flag++;
            }
        }
        if(flag<45){
         printf("could not calculate the costs..sleeping for random amount of time\n");
         random=rand()%5+1;
         sleep(random);
        }
        else{
            for(j=0;j<9;j++){
                for(i=j+1;i<10;i++){
                    cost[i][j]=fuel[i][j]*cities[i][j];
                    cost[j][i]=fuel[j][i]*cities[j][i];
                }
            }
            for(j=0;j<9;j++){
                for(i=j+1;i<10;i++){
                    printf("cost from city %d to city %d and vice versa: %d\n", i, j, cost[i][j]);
                    
                }
            }
            return 0;
        }
    }
}


int main(){
    int sock, client_sock, c, *new, i, j, random;
    struct sockaddr_in server, client;
    pthread_t thread2;
    
    srand(time(NULL));
    
    //initialize tous pinakes
    for(i=0;i<10;i++) for(j=0;j<10;j++) fuel[i][j]=0;
    
    for(i=0;i<9;i++) cities[i][i]=0;
    
    for(j=0;j<9;j++){
        for(i=j+1;i<10;i++){
            random=rand()%100+1; //vazw tuxaies times stis poleis
            cities[i][j]=random; //kai ftiaxnw ton summetriko pinaka
            cities[j][i]=random;
        }
    }
    
    for(i=0;i<10;i++){
     for(j=0;j<10;j++) printf("%d  \t",cities[i][j]);
     printf("\n");
    }

    //dhmiourgw ena tcp socket
    sock=socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
	{
        printf("could not create socket");
        exit(-1);
    } 
    puts("Socket created!\n");
    
    //plhrofories gia ton server
    server.sin_family = AF_INET;
    server.sin_addr.s_addr= INADDR_ANY;
    server.sin_port = htons(8888);
    
    //enwnw to socket me tis plhrofories
    if( bind(sock,(struct sockaddr *)&server, sizeof(server)) <0 )
	{
        perror("bind failed. Error");
        return 1;
    }
    
    puts("bind done");
    
    //perimenw gia threads, mexri 10 se anamonh
    listen(sock, 10);
    //kalw thn function gia ton upologismo tou kostous 
    pthread_create(&thread2, NULL, thread_func2, NULL);
    puts("Waiting for incoming connections...");
    c= sizeof(struct sockaddr_in);
    //plhrofories gia ton client
    while( (client_sock = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
        puts("connection accepted");
        pthread_t thread;
        
        new = malloc(1);
        *new= client_sock;
        //gia kathe thread kalw th function gia na treksoun tautoxrona
        if( pthread_create(&thread, NULL, thread_func, (void*) new) <0 )
		{
            perror("could not create thread");
            return 1;
        }
        
        //den uparxei join gia ton thread2 gia na mporei na upologizei se tuxaia diasthmata to kostos
        pthread_join(thread, NULL);
        
        puts("Handler assigned");
    }
    
    if(client_sock < 0)
	{
        perror("accept failed");
        return 1;
    }

    return 0;
}

