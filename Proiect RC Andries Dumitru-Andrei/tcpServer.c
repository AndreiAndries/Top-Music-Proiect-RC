#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <sqlite3.h>
#define PORT 4444

void rand_id(char v[])
{
    int i = rand() % 10000;
    int j = rand() % i;
    sprintf(v, "%d" , j);
}

int main()
{
    int sockfd, ret;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;
    socklen_t addr_size;
    char buffer[1024];
    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        printf("[-]Eroare la conexiune\n");
        exit(0);
    }
    printf("[+]Socket-ul serverului s-a creeat cu succes!\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0)
    {
        printf("[-]Eroare la bind!\n");
        exit(1);
    }
    printf("[+]Bind to port %d\n", 4444);
    if(listen(sockfd,10) == 0)
    {
        printf("[+]Listening...\n");
    }else{
        printf("[-]Eroare la bind!\n");
    }

    while(1){
        newSocket = accept(sockfd,(struct sockaddr*)&newAddr, &addr_size);
        if(newSocket < 0)
        {
            exit(1);
        }
        printf("Connection accepted form %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

        if((childpid = fork())==0)
        {
            close(sockfd);

            while(1)
            {
                recv(newSocket, buffer, 1024, 0);
                if(strcmp(buffer, "disconnect")==0){
                    printf("Disconnected from form %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
                    break;
                }
                else 
                {
                    if(strstr(buffer,"Login-usr")!=NULL)
                    {
                        char comm[1000]="",user[100],passwd[100],actualusr[100],actualid[100];
                        int k1=0,k2=0,k3=0,k4=0,i,j,l,r;
                        printf("Clien: %s\n",buffer);
                        strcpy(comm,"Userul v-a fi conectat in functie de baza de date\n");
                        for(i=10;buffer[i]!=' ';++i)
                        {
                            user[k1++]=buffer[i];
                        }
                        user[k1]='\0';
                        for(j=i+1;buffer[j]!=' ';++j)
                        {
                            passwd[k2++]=buffer[j];
                        }
                        passwd[k2]='\0';
                        for(l=j+1;buffer[l]!=' ';++l)
                        {
                            actualusr[k3++]=buffer[l];
                        }
                        actualusr[k3]='\0';
                        for(r=l+1;buffer[r]!=' ';++r)
                        {
                            actualid[k4++]=buffer[r];
                        }
                        actualid[k4]='\0';
                        passwd[k2]='\0';
                        if(strcmp(actualusr,"no_user_logged")!=0)
                        {
                            strcpy(comm,"un alt user este deja conectat!\n");
                        }
                        else
                        {
                            int u=0,p=0;
                            sqlite3 *db;
                            sqlite3_open("proiectRC.db", &db);
                            char command[1000]="select username,passwd,id_user from Utilizatori where username = '";
                            strcat(command,user);
                            strcat(command,"'");
                            sqlite3_stmt *stmt;
                            const char* col1 ;
                            const char* col2;
                            const char* col3;
                            sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                            while (sqlite3_step(stmt) != SQLITE_DONE)
                            {
                                col1= sqlite3_column_text(stmt,0);
                                col2= sqlite3_column_text(stmt,1);
                                col3= sqlite3_column_text(stmt,2);
                                if(strcmp(col1,user)==0)u=1;
                                if(strcmp(col2,passwd)==0)p=1;
                                strcpy(actualid,col3);
                            }
                            sqlite3_finalize(stmt);
                            sqlite3_close(db);
                            if(u==1 && p==1)
                            {
                                strcpy(comm,user);
                                strcat(comm," ");
                                strcat(comm,actualid);
                                strcat(comm," ");
                                strcat(comm,"a fost conectat cu succes\n");
                            }
                            else{
                                strcpy(comm,"username sau parola gresita\n");
                            }
                        }
                        send(newSocket, comm, strlen(comm),0);
                        bzero(buffer, 1024);
                        bzero(comm,sizeof(comm));
                        bzero(user,sizeof(user));
                        bzero(passwd,sizeof(passwd));
                        bzero(actualid,sizeof(actualid));
                        bzero(actualusr,sizeof(actualusr));
                    }
                    else if(strstr(buffer,"Signup-usr")!=NULL)
                    {
                        char comm[1000],user[100],passwd[100],actualusr[100],actualid[100];
                        int k1=0,k2=0,k3=0,k4=0,l,r,i,j;
                        printf("Clien: %s\n",buffer);
                        for(i=11;buffer[i]!=' ';++i)
                        {
                            user[k1++]=buffer[i];
                        }
                        user[k1]='\0';

                        for(j=i+1;buffer[j]!=' ';++j)
                        {
                            passwd[k2++]=buffer[j];
                        }
                        passwd[k2]='\0';
                        for(l=j+1;buffer[l]!=' ';++l)
                        {
                            actualusr[k3++]=buffer[l];
                        }
                        actualusr[k3]='\0';
                        for(r=l+1;buffer[r]!=' ';++r)
                        {
                            actualid[k4++]=buffer[r];
                        }
                        actualid[k4]='\0';
                        if(strcmp(actualusr,"no_user_logged")!=0)
                        {
                            strcpy(comm,"un alt user este deja conectat!\n");
                        }
                        else
                        {
                            int u=1;
                            sqlite3 *db;
                            sqlite3_open("proiectRC.db", &db);
                            char command[1000]="select username from Utilizatori";
                            sqlite3_stmt *stmt;
                            const char* col1 ;
                            const char* col2;
                            sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                            while (sqlite3_step(stmt) != SQLITE_DONE)
                            {
                                col1= sqlite3_column_text(stmt,0);
                                if(col1!=NULL)
                                {
                                	if(strcmp(col1,user)==0)u=0;
                                }
                            }
                            if(u==0)
                            {
                                strcat(comm,"Username indisponibil\n");
                            }
                            else 
                            {	
                                char id1[100];
                                rand_id(id1);
                                char command1[1000]="INSERT INTO Utilizatori VALUES( '";
                                strcat(command1,id1);
                                strcat(command1,"' , '");
                                strcat(command1,user);
                                strcat(command1,"' , '");
                                strcat(command1,passwd);
                                strcat(command1,"' , 'normal' , NULL );");
                                printf("%s\n",command1);
                                if(sqlite3_exec(db,command1,NULL,NULL,NULL))
                                {
                                    strcat(comm,"Eroare la inregistrare\n");
                                }
                                else
                                {
                                    strcpy(comm,user);
                                    strcat(comm," ");
                                    strcat(comm,id1);
                                    strcat(comm," ");
                                    strcat(comm,"a fost conectat cu succes\n");
                                }
                            }
                            printf("%s",comm);
                            sqlite3_finalize(stmt);
                            sqlite3_close(db); 
                        }
                        send(newSocket, comm, strlen(comm),0);
                        bzero(buffer, 1024);
                        bzero(comm,sizeof(comm));
                        bzero(user,sizeof(user));
                        bzero(passwd,sizeof(passwd));
                        bzero(actualid,sizeof(actualid));
                        bzero(actualusr,sizeof(actualusr));
                    }
                    else if(strstr(buffer,"Login-adm")!=NULL)
                    {
                        char comm[1000]="",user[1000],passwd[1000],actualusr[100],actualid[100];
                        int k1=0,k2=0,k3=0,k4=0,l,r,i,j;
                        printf("Clien: %s\n",buffer);
                        for(i=10;buffer[i]!=' ';++i)
                        {
                            user[k1++]=buffer[i];
                        }
                        user[k1]='\0';
                        for(j=i+1;buffer[j]!=' ';++j)
                        {
                            passwd[k2++]=buffer[j];
                        }
                        passwd[k2]='\0';
                        for(l=j+1;buffer[l]!=' ';++l)
                        {
                            actualusr[k3++]=buffer[l];
                        }
                        actualusr[k3]='\0';
                        for(r=l+1;buffer[r]!=' ';++r)
                        {
                            actualid[k4++]=buffer[r];
                        }
                        actualid[k4]='\0';
                        if(strcmp(actualusr,"no_user_logged")!=0)
                        {
                            strcpy(comm,"un alt user este deja conectat!\n");
                        }
                        else
                        {
                            int u=0,p=0,a=0;
                            sqlite3 *db;
                            sqlite3_open("proiectRC.db", &db);
                            char command[1000]="select username,passwd,id_user,tipul_utilizatorului from Utilizatori where username = '";
                            strcat(command,user);
                            strcat(command,"'");
                            sqlite3_stmt *stmt;
                            const char* col1;
                            const char* col2;
                            const char* col3;
                            const char* col4;
                            sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                            while (sqlite3_step(stmt) != SQLITE_DONE)
                            {
                                col1= sqlite3_column_text(stmt,0);
                                col2= sqlite3_column_text(stmt,1);
                                col3= sqlite3_column_text(stmt,2);
                                col4= sqlite3_column_text(stmt,3);
                                if(strcmp(col1,user)==0)u=1;
                                if(strcmp(col2,passwd)==0)p=1;
                                if(strcmp(col4,"admin")==0)a=1;
                                strcpy(actualid,col3);
                            }
                            sqlite3_finalize(stmt);
                            sqlite3_close(db);
                            if(u==1 && p==1 && a==1)
                            {
                                strcpy(comm,user);
                                strcat(comm," ");
                                strcat(comm,actualid);
                                strcat(comm," ");
                                strcat(comm,"a fost conectat cu succes\n");
                            }
                            else{
                                strcpy(comm,"username sau parola gresita contu nu este de administrator\n");
                            }
                        }
                        send(newSocket, comm, strlen(comm),0);
                        bzero(buffer, 1024);
                        bzero(comm,sizeof(comm));
                        bzero(user,sizeof(user));
                        bzero(passwd,sizeof(passwd));
                        bzero(actualid,sizeof(actualid));
                        bzero(actualusr,sizeof(actualusr));

                    }
                    else if(strstr(buffer,"Help")!=NULL)
                    {
                        char comm[1000];
                        printf("Clien: %s\n",buffer);
                        strcpy(comm,"SPERAM CA ACEASTA COMANDA VA AJUTAT DESTUL DE MULT\n");
                        send(newSocket, comm, strlen(comm),0);
                        bzero(buffer, 1024);
                        bzero(comm,sizeof(comm));
                    }
                    else if(strstr(buffer,"Like")!=NULL)
                    {
                        char comm[1000]="",song_id[100],actualusr[100],actualid[100];
                        int k=0,i,k3=0,k4=0,l,r;
                        printf("Clien: %s\n",buffer);
                        strcpy(comm,"Votul dumneavoastra va fi inregistrat in caz ca aveti drept de vot\n");
                        for(int i=5;buffer[i]!=' ';++i)
                        {
                            song_id[k++]=buffer[i];
                        }
                        song_id[k]='\0';
                        for(l=i+1;buffer[l]!=' ';++l)
                        {
                            actualusr[k3++]=buffer[l];
                        }
                        actualusr[k3]='\0';
                        for(r=l+1;buffer[r]!=' ';++r)
                        {
                            actualid[k4++]=buffer[r];
                        }
                        actualid[k4]='\0';
                        if(strcmp(actualusr,"no_user_logged")==0)
                        {
                            strcpy(comm,"Nu sunteti conectat la server!\n");
                        }
                        else
                        {
                            int u=1;
                            sqlite3 *db;
                            sqlite3_open("proiectRC.db", &db);
                            char command[1000]="select id_user,id_melodie from Recenzii where like is not null";
                            sqlite3_stmt *stmt;
                            const char* col1;
                            const char* col2;
                            sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                            while (sqlite3_step(stmt) != SQLITE_DONE)
                            {
                                col1= sqlite3_column_text(stmt,0);
                                col2= sqlite3_column_text(stmt,1);
                                if(strcmp(col1,actualusr)==0 && strcmp(col2,song_id)==0)u=0;
                            }
                            sqlite3_reset(stmt);
                            if(u==0)
                            {
                                strcpy(comm,"deja ati dat like acestei melodii\n");
                            }
                            else 
                            {
                                strcpy(command,"select id_user , permisiuni from Utilizatori where permisiuni is not null");
                                sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                                const char* colp1;
                                const char* colp2;
                                char perm[100]="";
                                while (sqlite3_step(stmt) != SQLITE_DONE)
                                {
                                    colp1= sqlite3_column_text(stmt,0);
                                    colp2= sqlite3_column_text(stmt,1);
                                    if(strcmp(colp1,actualusr)==0)
                                    {
                                    	strcat(perm,colp2);
                                    }
                                }
                                if(strcmp(perm,"")==0)strcat(perm,"nu sunt restrictii");
                                sqlite3_reset(stmt);
                                strcpy(command,"select gen_muzical from Melodii where id_melodie ='");
                                strcat(command,song_id);
                                strcat(command,"'");
                                sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                                const char* colp3;
                                char*gen;
                                int ok=1;
                                while (sqlite3_step(stmt) != SQLITE_DONE)
                                {
                                    colp3= sqlite3_column_text(stmt,0);
                                    if(strcmp(colp3,perm)==0 || strcmp(perm,"all")==0)
		                        {
		                            strcpy(comm,"Nu aveti permisiune de a da like-uri acestui gen muzical");
		                            ok=0;
		                        }
                                }
                                if(ok==1)
                                {
                                    strcpy(command,"insert into Recenzii VALUES ( '");
                                    strcat(command,actualusr);
                                    strcat(command,"' , '");
                                    strcat(command,song_id);
                                    strcat(command,"' , ");
                                    strcat(command,"null");
                                    strcat(command," , 'a');");
                                    sqlite3_exec(db,command,NULL,NULL,NULL);
                                    strcpy(comm,"ati dat like melodiei cu id-ul ");
                                    strcat(comm,song_id);
                                    strcat(comm,"\n");
                                }
                            }
                            sqlite3_finalize(stmt);
                            sqlite3_close(db);
                            
                        }
                        send(newSocket, comm, strlen(comm),0);
                        bzero(buffer, 1024);
                        bzero(comm,sizeof(comm));
                        bzero(song_id,sizeof(song_id));
                        bzero(actualusr,sizeof(actualusr));
                        bzero(actualid,sizeof(actualid));
                    }
                    else if(strstr(buffer,"Comm")!=NULL)
                    {
                        char comm[1000]="",song_id[100],com[500],actualusr[100],actualid[100];
                        int k1=0,k2=0,i,j,l,r,k3=0,k4=0;
                        for(i=5;buffer[i]!=' ';++i)
                        {
                            song_id[k1++]=buffer[i];
                        }
                        song_id[k1]='\0';
                        for(j=i+1;buffer[j]!=' ';++j)
                        {
                            com[k2++]=buffer[j];
                        }
                        for(l=j+1;buffer[l]!=' ';++l)
                        {
                            actualusr[k3++]=buffer[l];
                        }
                        actualusr[k3]='\0';
                        for(r=l+1;buffer[r]!=' ';++r)
                        {
                            actualid[k4++]=buffer[r];
                        }
                        actualid[k4]='\0';
                        com[k2]='\0';
                        printf("Clien: %s\n",buffer);
                        if(strcmp(actualusr,"no_user_logged")==0)
                        {
                            strcpy(comm,"Nu sunteti conectat la server!\n");
                        }
                        else 
                        {
                            sqlite3 *db;
                            sqlite3_open("proiectRC.db", &db);
				 	        char command1[1000]="INSERT INTO Recenzii VALUES( '";
                            strcat(command1,actualusr);
                            strcat(command1,"' , '");
                            strcat(command1,song_id);
                            strcat(command1,"' , '");
                            strcat(command1,com);
                            strcat(command1,"' , null);");
                            printf("%s\n",command1);
                            if(sqlite3_exec(db,command1,NULL,NULL,NULL))
                            {
                                strcat(comm,"Eroare la inregistrare\n");
                            }
                            else
                            {
				 	  	        strcat(comm,"like inregistrat\n");
				 	        }
                           	sqlite3_close(db);
                        }
                        send(newSocket, comm, strlen(comm),0);
                        bzero(buffer, 1024);
                        bzero(comm,sizeof(comm));
                        bzero(song_id,sizeof(song_id));
                        bzero(com,sizeof(com));
                        bzero(actualusr,sizeof(actualusr));
                        bzero(actualid,sizeof(actualid));
                    }
                    else if(strstr(buffer,"addsong")!=NULL)
                    {
                        char comm[1000]="",nume[30],link[100],artist[30],gen[40],actualusr[100],actualid[100];
                        int k1=0,k2=0,k3=0,k4=0,k5=0,k6=0,i,j,l,r,q,p;
                        for(i=8;buffer[i]!=' ';++i)
                        {
                            nume[k1++]=buffer[i];
                        }
                        nume[k1]='\0';
                        for(j=i+1;buffer[j]!=' ';++j)
                        {
                            artist[k2++]=buffer[j];
                        }
                        artist[k2]='\0';
                        for(l=j+1;buffer[l]!=' ';l++)
                        {
                            link[k3++]=buffer[l];
                        }
                        link[k3]='\0';
                        for(r=l+1;buffer[r]!=' ';r++)
                        {
                            gen[k4++]=buffer[r];
                        }
                        gen[k4]='\0';
                        for(q=r+1;buffer[q]!=' ';++q)
                        {
                            actualusr[k5++]=buffer[q];
                        }
                        actualusr[k5]='\0';
                        for(p=q+1;buffer[p]!=' ';++p)
                        {
                            actualid[k6++]=buffer[p];
                        }
                        actualid[k6]='\0';
                        printf("Clien: %s\n",buffer);
                        if(strcmp(actualusr,"no_user_logged")==0)
                        {
                            strcpy(comm,"Nu sunteti conectat la server!\n");
                        }
                        else 
                        {
                            sqlite3 *db;
                            char id1[100];
                            rand_id(id1);
                            sqlite3_open("proiectRC.db", &db);
                            char command1[1000]="INSERT INTO Melodii VALUES( '";
                            strcat(command1,id1);
                            strcat(command1,"' , '");
                            strcat(command1,nume);
                            strcat(command1,"' , '");
                            strcat(command1,artist);
                            strcat(command1,"' , '");
                            strcat(command1,link);
                            strcat(command1,"' , '");
                            strcat(command1,gen);
                            strcat(command1,"' );");
                            printf("%s\n",command1);
                            if(sqlite3_exec(db,command1,NULL,NULL,NULL))
                            {
                                strcat(comm,"Eroare la inregistrare\n");
                            }
                            else
                            {
                                strcat(comm,"melodie inregistrata\n");
                            }
                            sqlite3_close(db);

                        }
                        send(newSocket, comm, strlen(comm),0);
                        bzero(buffer, 1024);
                        bzero(comm,sizeof(comm));
                        bzero(nume,sizeof(nume));
                        bzero(artist,sizeof(artist));
                        bzero(link,sizeof(link));
                        bzero(gen,sizeof(gen));
                        bzero(actualusr,sizeof(actualusr));
                        bzero(actualid,sizeof(actualid));
                    }
                    else if(strstr(buffer,"Ban-all-likes")!=NULL)
                    {
                        char comm[1000]="",usr_id[20],actualusr[100],actualid[100];
                        int k=0,k3=0,k4=0,i,l,r;
                        for(i=14;buffer[i]!=' ';++i)
                        {
                            usr_id[k++]=buffer[i];
                        }
                        for(l=i+1;buffer[l]!=' ';++l)
                        {
                            actualusr[k3++]=buffer[l];
                        }
                        actualusr[k3]='\0';
                        for(r=l+1;buffer[r]!=' ';++r)
                        {
                            actualid[k4++]=buffer[r];
                        }
                        actualid[k4]='\0';
                        usr_id[k]='\0';
                        if(strcmp(actualusr,"no_user_logged")==0)
                        {
                            strcpy(comm,"Nu sunteti conectat la server!\n");
                        }
                        else 
                        {
                            int u=1;
                            sqlite3 *db;
                            sqlite3_open("proiectRC.db", &db);
                            char command[1000]="select tipul_utilizatorului from Utilizatori where id_user = '";
                            strcat(command,actualid);
                            strcat(command,"'");
                            sqlite3_stmt *stmt;
                            const char* col1;
                            sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                            while (sqlite3_step(stmt) != SQLITE_DONE)
                            {
                                col1= sqlite3_column_text(stmt,0);
                                if(strcmp(col1,"admin")!=0)
                                {
                                	u=0;
                                	strcat(comm,"Nu sunteti administrator, nu puteti efectua comanda!\n");
                                }
                            }
                            sqlite3_reset(stmt);
                            
                            if(u==1) 
                            {                                
                                strcpy(command,"UPDATE Utilizatori set permisiuni = 'all' WHERE id_user = '");
                                strcat(command,usr_id);
                                strcat(command,"' ;");
                                sqlite3_exec(db,command,NULL,NULL,NULL);
                                strcpy(comm,"ati luat dreptul user-ului ");
                                strcat(comm,usr_id);
                                strcat(comm," de a da like-uri\n");
                            }
                            sqlite3_finalize(stmt);
                            sqlite3_close(db);
                        }
                        send(newSocket, comm, strlen(comm),0);
                        bzero(buffer, 1024);
                        bzero(comm,sizeof(comm));
                        bzero(usr_id,sizeof(usr_id));
                        bzero(actualid,sizeof(actualid));
                        bzero(actualusr,sizeof(actualusr));
                    }
                    else if(strstr(buffer,"Ban-likes")!=NULL)
                    {
                        char comm[1000]="",usr_id[20],gen[30],actualusr[100],actualid[100];
                        int i,j,l,r,k1=0,k2=0,k3=0,k4=0;
                        for(i=10;buffer[i]!=' ';++i)
                        {
                            gen[k1++]=buffer[i];
                        }
                        gen[k1]='\0';
                        for(j=i+1;buffer[j]!=' ';++j)
                        {
                            usr_id[k2++]=buffer[j];
                        }
                        usr_id[k2]='\0';
                        for(l=j+1;buffer[l]!=' ';++l)
                        {
                            actualusr[k3++]=buffer[l];
                        }
                        actualusr[k3]='\0';
                        for(r=l+1;buffer[r]!=' ';++r)
                        {
                            actualid[k4++]=buffer[r];
                        }
                        actualid[k4]='\0';
                        if(strcmp(actualusr,"no_user_logged")==0)
                        {
                            strcpy(comm,"Nu sunteti conectat la server!\n");
                        }
                        else 
                        {
                            int u=1;
                            sqlite3 *db;
                            sqlite3_open("proiectRC.db", &db);
                            char command[1000]="select tipul_utilizatorului from Utilizatori where id_user = '";
                            strcat(command,actualid);
                            strcat(command,"'");
                            sqlite3_stmt *stmt;
                            const char* col1;
                            sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                            while (sqlite3_step(stmt) != SQLITE_DONE)
                            {
                                col1= sqlite3_column_text(stmt,0);
                                if(strcmp(col1,"admin")!=0)
                                {
                                	u=0;
                                	strcat(comm,"Nu sunteti administrator, nu puteti efectua comanda!\n");
                                }
                            }
                            sqlite3_reset(stmt);
                            
                            if(u==1) 
                            {                                
                                strcpy(command,"UPDATE Utilizatori set permisiuni = '");
                                strcat(command,gen);
                                strcat(command,"' WHERE id_user = '");
                                strcat(command,usr_id);
                                strcat(command,"' ;");
                                sqlite3_exec(db,command,NULL,NULL,NULL);
                                strcpy(comm,"ati luat dreptul user-ului ");
                                strcat(comm,usr_id);
                                strcat(comm," de a da like-uri genului muzical : ");
                                strcat(comm,gen);
                                strcat(comm,"\n");
                            }
                            sqlite3_finalize(stmt);
                            sqlite3_close(db);
                        }
                        send(newSocket, comm, strlen(comm),0);
                        bzero(buffer, 1024);
                        bzero(comm,sizeof(comm));
                        bzero(usr_id,sizeof(usr_id));
                        bzero(gen,sizeof(gen));
                        bzero(actualid,sizeof(actualid));
                        bzero(actualusr,sizeof(actualusr));
                    }
                    else if(strstr(buffer,"Delete-song")!=NULL)
                    {
                        char comm[1000],song_id[20],actualusr[100],actualid[100];
                        int k=0,k3=0,k4=0,l,r,i;
                        for(i=12;buffer[i]!=' ';++i)
                        {
                            song_id[k++]=buffer[i];
                        }
                        song_id[k]='\0';
                        for(l=i+1;buffer[l]!=' ';++l)
                        {
                            actualusr[k3++]=buffer[l];
                        }
                        actualusr[k3]='\0';
                        for(r=l+1;buffer[r]!=' ';++r)
                        {
                            actualid[k4++]=buffer[r];
                        }
                        actualid[k4]='\0';
                        printf("Clien: %s\n",buffer);
                        if(strcmp(actualusr,"no_user_logged")==0)
                        {
                            strcpy(comm,"Nu sunteti conectat la server!\n");
                        }
                        else 
                        {
                            int u=1;
                            sqlite3 *db;
                            sqlite3_open("proiectRC.db", &db);
                            char command[1000]="select tipul_utilizatorului from Utilizatori where id_user = '";
                            strcat(command,actualid);
                            strcat(command,"'");
                            sqlite3_stmt *stmt;
                            const char* col1;
                            sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                            while (sqlite3_step(stmt) != SQLITE_DONE)
                            {
                                col1= sqlite3_column_text(stmt,0);
                                if(strcmp(col1,"admin")!=0)
                                {
                                	u=0;
                                	strcat(comm,"Nu sunteti administrator, nu puteti efectua comanda!\n");
                                }
                            }
                            sqlite3_reset(stmt);
                            
                            if(u==1) 
                            {                                
                                strcpy(command,"DELETE FROM Melodii WHERE id_melodie= '");
                                    strcat(command,song_id);
                                    strcat(command,"' ;");
                                    sqlite3_exec(db,command,NULL,NULL,NULL);
                                    strcpy(comm,"ati sters melodia cu id-ul ");
                                    strcat(comm,song_id);
                                    strcat(comm,"\n");
                                    strcpy(command,"DELETE FROM Recenzii WHERE id_melodie= '");
                                    strcat(command,song_id);
                                    strcat(command,"' ;");
                                    sqlite3_exec(db,command,NULL,NULL,NULL);
                            }
                            sqlite3_finalize(stmt);
                            sqlite3_close(db);
                            printf("%s",comm);
                        }
                        send(newSocket, comm, strlen(comm),0);
                        bzero(buffer, 1024);
                        bzero(comm,sizeof(comm));
                        bzero(song_id,sizeof(song_id));
                        bzero(actualid,sizeof(actualid));
                        bzero(actualusr,sizeof(actualusr));
                    }
                    else if(strstr(buffer,"buff")!=NULL)
                    {
                        char comm[1000]="",usr_id[20],actualusr[100],actualid[100];
                        int k=0,k3=0,k4=0,i,l,r;
                        printf("Clien: %s\n",buffer);
                        for(int i=5;buffer[i]!=' ';++i)
                        {
                            usr_id[k++]=buffer[i];
                        }
                        usr_id[k]='\0';
                        for(l=i+1;buffer[l]!=' ';++l)
                        {
                            actualusr[k3++]=buffer[l];
                        }
                        actualusr[k3]='\0';
                        for(r=l+1;buffer[r]!=' ';++r)
                        {
                            actualid[k4++]=buffer[r];
                        }
                        actualid[k4]='\0';
                        if(strcmp(actualusr,"no_user_logged")==0)
                        {
                            strcpy(comm,"Nu sunteti conectat la server!\n");
                        }
                        else 
                        {
                            int u=1;
                            sqlite3 *db;
                            sqlite3_open("proiectRC.db", &db);
                            char command[1000]="select tipul_utilizatorului from Utilizatori where id_user = '";
                            strcat(command,actualusr);
                            strcat(command,"'");
                            sqlite3_stmt *stmt;
                            const char* col1;
                            sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                            while (sqlite3_step(stmt) != SQLITE_DONE)
                            {
                                col1= sqlite3_column_text(stmt,0);
                                if(strcmp(col1,"admin")!=0)
                                {
                                	u=0;
                                	strcat(comm,"Nu sunteti administrator, nu puteti efectua comanda!\n");
                                }
                            }
                            sqlite3_reset(stmt);
                            
                            if(u==1) 
                            {                                
                                strcpy(command,"UPDATE Utilizatori SET tipul_utilizatorului = 'admin' WHERE id_user = '");
                                strcat(command,usr_id);
                                strcat(command,"' ;");
                                sqlite3_exec(db,command,NULL,NULL,NULL);
                                strcpy(comm,"l-ati facut pe");
                                strcat(comm,usr_id);
                                strcat(comm," administrator");
                                strcat(comm,"\n");
                            }
                            sqlite3_finalize(stmt);
                            sqlite3_close(db);
                        }
                        send(newSocket, comm, strlen(comm),0);
                        bzero(buffer, 1024);
                        bzero(comm,sizeof(comm));
                        bzero(usr_id,sizeof(usr_id));
                        bzero(actualusr,sizeof(actualusr));
                        bzero(actualid,sizeof(actualid));
                    }
                    else if(strstr(buffer,"Show")!=NULL)
                    {
                        if(strstr(buffer,"song-nr-likes")!=NULL)
                        {
                            char comm[1000]="",song_id[20],actualusr[100],actualid[100];
                            int k=0,k3=0,k4=0,i,l,r;;
                            for(i=19;buffer[i]!=' ';++i)
                            {
                                song_id[k++]=buffer[i];
                            }
                            song_id[k]='\0';
                            for(l=i+1;buffer[l]!=' ';++l)
                            {
                                actualusr[k3++]=buffer[l];
                            }
                            actualusr[k3]='\0';
                            for(r=l+1;buffer[r]!=' ';++r)
                            {
                                actualid[k4++]=buffer[r];
                            }
                            actualid[k4]='\0';
                            printf("Clien: %s\n",buffer);
                            if(strcmp(actualusr,"no_user_logged")==0)
                            {
                                strcpy(comm,"Nu sunteti conectat la server!\n");
                            }
                            else 
                            {
                                sqlite3 *db;
                                char cpy[10];
                                sqlite3_open("proiectRC.db", &db);
                                char command[1000]="select count(like),id_melodie from Recenzii GROUP BY id_melodie HAVING id_melodie = '";
                                strcat(command,song_id);
                                strcat(command,"'");
                                sqlite3_stmt *stmt;
                                int col1 ;
                                const char* col2;
                                sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                                while (sqlite3_step(stmt) != SQLITE_DONE)
                                {
                                    col1= sqlite3_column_int(stmt,0);
                                    col2= sqlite3_column_text(stmt,1);
                                    sprintf(cpy,"%d",col1);
                                }
                                sqlite3_finalize(stmt);
                                sqlite3_close(db);
                                strcpy(comm,song_id);
                                strcat(comm," are");
                                strcat(comm,cpy);
                                strcat(comm,"like-uri");
                            }
                            send(newSocket, comm, strlen(comm),0);
                            bzero(buffer, 1024);
                            bzero(comm,sizeof(comm));
                            bzero(song_id,sizeof(song_id));
                            bzero(actualusr,sizeof(actualusr));
                            bzero(actualid,sizeof(actualid));
                        }
                        else if(strstr(buffer,"song-details")!=NULL)
                        {
                            char comm[1000]="",nume[100],actualusr[100],actualid[100];
                            int k=0,k3=0,k4=0,i,l,r;;
                            printf("Clien: %s\n",buffer);
                            for(i=18;buffer[i]!=' ';++i)
                            {
                                nume[k++]=buffer[i];
                            }
                            nume[k]='\0';
                            for(l=i+1;buffer[l]!=' ';++l)
                            {
                                actualusr[k3++]=buffer[l];
                            }
                            actualusr[k3]='\0';
                            for(r=l+1;buffer[r]!=' ';++r)
                            {
                                actualid[k4++]=buffer[r];
                            }
                            actualid[k4]='\0';
                            if(strcmp(actualusr,"no_user_logged")==0)
                            {
                                strcpy(comm,"Nu sunteti conectat la server!\n");
                            }
                            else 
                            {
                                sqlite3 *db;
                                sqlite3_open("proiectRC.db", &db);
                                char command[1000]="select * from Melodii where nume_melodie = '";
                                strcat(command,nume);
                                strcat(command,"'");
                                sqlite3_stmt *stmt;
                                const char* col1 ;
                                const char* col2;
                                const char* col3;
                                const char* col4;
                                const char* col5;
                                sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                                while (sqlite3_step(stmt) != SQLITE_DONE)
                                {
                                    col1= sqlite3_column_text(stmt,0);
                                    if(col1!=NULL){
                                        col2= sqlite3_column_text(stmt,1);
                                        col3= sqlite3_column_text(stmt,2);
                                        col4= sqlite3_column_text(stmt,3);
                                        col5= sqlite3_column_text(stmt,4);
                                        strcat(comm,col1);
                                        strcat(comm," ");
                                        strcat(comm,col2);
                                        strcat(comm," ");
                                        strcat(comm,col3);
                                        strcat(comm," ");
                                        strcat(comm,col4);
                                        strcat(comm," ");
                                        strcat(comm,col5);
                                        strcat(comm,"\n");
                                    }
                                    else
                                    {
                                        strcat(comm,"Nu exista o astfel de melodie\n");
                                        break;
                                    }
                                        
                                }
                                sqlite3_finalize(stmt);
                                sqlite3_close(db);
                            }
                            send(newSocket, comm, strlen(comm),0);
                            bzero(buffer, 1024);
                            bzero(comm,sizeof(comm));
                            bzero(nume,sizeof(nume));
                            bzero(actualusr,sizeof(actualusr));
                            bzero(actualid,sizeof(actualid));
                        }
                        else if(strstr(buffer,"song-nr-comms")!=NULL)
                        {
                            char comm[1000]="",song_id[20],actualusr[100],actualid[100];
                            int k=0,k3=0,k4=0,l,r,i;
                            for(i=19;buffer[i]!=' ';++i)
                            {
                                song_id[k++]=buffer[i];
                            }
                            song_id[k]='\0';
                            for(l=i+1;buffer[l]!=' ';++l)
                            {
                                actualusr[k3++]=buffer[l];
                            }
                            actualusr[k3]='\0';
                            for(r=l+1;buffer[r]!=' ';++r)
                            {
                                actualid[k4++]=buffer[r];
                            }
                            actualid[k4]='\0';
                            printf("Clien: %s\n",buffer);
                            if(strcmp(actualusr,"no_user_logged")==0)
                            {
                                strcpy(comm,"Nu sunteti conectat la server!\n");
                            }
                            else 
                            {
                                int u=0,p=0;
                                sqlite3 *db;
                                sqlite3_open("proiectRC.db", &db);
                                char command[1000]="select id_melodie,comm FROM Recenzii WHERE id_melodie='";
                                strcat(command,song_id);
                                strcat(command,"' and comm is not null");
                                sqlite3_stmt *stmt;
                                const char* col1 ;
                                const char* col2;
                                sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                                while (sqlite3_step(stmt) != SQLITE_DONE)
                                {
                                    col1= sqlite3_column_text(stmt,0);
                                    if(col1!=NULL){
                                        col2= sqlite3_column_text(stmt,1);
                                        strcat(comm,col1);
                                        strcat(comm," ");
                                        strcat(comm,col2);
                                        strcat(comm,"\n");	
                                        }
                                    else 
                                    {
                                        strcat(comm,"Melodia nu are comentarii\n");
                                        break;
                                    }
                                        
                                }
                                sqlite3_finalize(stmt);
                                sqlite3_close(db);
                            }
                            send(newSocket, comm, strlen(comm),0);
                            bzero(buffer, 1024);
                            bzero(comm,sizeof(comm));
                            bzero(song_id,sizeof(song_id));
                            bzero(actualusr,sizeof(actualusr));
                            bzero(actualid,sizeof(actualid));
                        }
                        else if(strstr(buffer,"type")!=NULL)
                        {
                            char comm[1000]="",gen[100],actualusr[100],actualid[100];
                            int k=0,k3=0,k4=0,i,l,r;;
                            for(i=10;buffer[i]!=' ';++i)
                            {
                                gen[k++]=buffer[i];
                            }
                            gen[k]='\0';
                            for(l=i+1;buffer[l]!=' ';++l)
                            {
                                actualusr[k3++]=buffer[l];
                            }
                            actualusr[k3]='\0';
                            for(r=l+1;buffer[r]!=' ';++r)
                            {
                                actualid[k4++]=buffer[r];
                            }
                            actualid[k4]='\0';
                            printf("Clien: %s\n",buffer);
                            if(strcmp(actualusr,"no_user_logged")==0)
                            {
                                strcpy(comm,"Nu sunteti conectat la server!\n");
                            }
                            else 
                            {
                                sqlite3 *db;
                                sqlite3_open("proiectRC.db", &db);
                                char command[1000]="select artist , nume_melodie from Melodii WHERE gen_muzical= '";
                                strcat(command,gen);
                                strcat(command,"'");
                                sqlite3_stmt *stmt;
                                const char* col1 ;
                                const char* col2;
                                sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                                while (sqlite3_step(stmt) != SQLITE_DONE)
                                {
                                    col1= sqlite3_column_text(stmt,0);
                                    if(col1!=NULL)
                                    {
                                        col2= sqlite3_column_text(stmt,1);
                                        strcat(comm,col1);
                                        strcat(comm," ");
                                        strcat(comm,col2);
                                        strcat(comm,"\n");
                                    }
                                    else
                                    {
                                        strcat(comm,"Nu exista acest gen muzical\n");
                                    }                                    
                                }
                                sqlite3_finalize(stmt);
                                sqlite3_close(db);

                            }
                            send(newSocket, comm, strlen(comm),0);
                            bzero(buffer, 1024);
                            bzero(comm,sizeof(comm));
                            bzero(actualusr,sizeof(actualusr));
                            bzero(actualid,sizeof(actualid));
                        }
                        else if(strstr(buffer,"Song")!=NULL)
                        {
                            char comm[1000],nume[50],actualusr[100],actualid[100];
                            int k=0,k3=0,k4=0,i,l,r;;
                            for(i=10;buffer[i]!=' ';++i)
                            {
                                nume[k++]=buffer[i];
                            }
                            nume[k]='\0';
                            for(l=i+1;buffer[l]!=' ';++l)
                            {
                                actualusr[k3++]=buffer[l];
                            }
                            actualusr[k3]='\0';
                            for(r=l+1;buffer[r]!=' ';++r)
                            {
                                actualid[k4++]=buffer[r];
                            }
                            actualid[k4]='\0';
                            if(strcmp(actualusr,"no_user_logged")==0)
                            {
                                strcpy(comm,"Nu sunteti conectat la server!\n");
                            }
                            else 
                            {
                                sqlite3 *db;
                                sqlite3_open("proiectRC.db", &db);
                                char command[1000]="select nume_melodie , artist,  link_YT from Melodii where nume_melodie = '";
                                strcat(command,nume);
                                strcat(command,"'");
                                sqlite3_stmt *stmt;
                                const char* col1 ;
                                const char* col2;
                                const char* col3;
                                sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                                while (sqlite3_step(stmt) != SQLITE_DONE)
                                {
                                    col1= sqlite3_column_text(stmt,0);
                                    if(col1!=NULL){
                                        col2= sqlite3_column_text(stmt,1);
                                        col3= sqlite3_column_text(stmt,2);
                                        strcat(comm,col1);
                                        strcat(comm,"-");
                                        strcat(comm,col2);
                                        strcat(comm,"      ");
                                        strcat(comm,col3);
                                        strcat(comm," ");
                                        strcat(comm,"\n");
                                        }
                                    else
                                    {
                                        strcat(comm,"Nu exista o astfel de melodie\n");
                                        break;
                                    }
                                        
                                }
                                printf("%s",comm);
                                sqlite3_finalize(stmt);
                                sqlite3_close(db);
                            }
                            send(newSocket, comm, strlen(comm),0);
                            bzero(buffer, 1024);
                            bzero(comm,sizeof(comm));
                            bzero(nume,sizeof(nume));
                            bzero(actualusr,sizeof(actualusr));
                            bzero(actualid,sizeof(actualid));
                        }
                        else if(strstr(buffer,"artist")!=NULL)
                        {
                            char comm[1000]="",artist[50],actualusr[100],actualid[100];
                            int k=0,k3=0,k4=0,i,l,r;
                            for(i=12;buffer[i]!=' ';++i)
                            {
                                artist[k++]=buffer[i];
                            }
                            artist[k]='\0';
                            for(l=i+1;buffer[l]!=' ';++l)
                            {
                                actualusr[k3++]=buffer[l];
                            }
                            actualusr[k3]='\0';
                            for(r=l+1;buffer[r]!=' ';++r)
                            {
                                actualid[k4++]=buffer[r];
                            }
                            actualid[k4]='\0';
                            printf("Clien: %s\n",buffer);
                            if(strcmp(actualusr,"no_user_logged")==0)
                            {
                                strcpy(comm,"Nu sunteti conectat la server!\n");
                            }
                            else 
                            {
                                sqlite3 *db;
                                sqlite3_open("proiectRC.db", &db);
                                char command[1000]="select nume_melodie,artist from Melodii WHERE artist = '";
                                strcat(command,artist);
                                strcat(command,"'");
                                sqlite3_stmt *stmt;
                                const char* col1 ;
                                const char* col2;
                                sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                                while (sqlite3_step(stmt) != SQLITE_DONE)
                                {
                                    col1= sqlite3_column_text(stmt,0);
                                    if(col1!=NULL){
                                        col2= sqlite3_column_text(stmt,1);
                                        strcat(comm,col1);
                                        strcat(comm," ");
                                        strcat(comm,col2);
                                        strcat(comm,"\n");
                                        }
                                    else
                                    {
                                        strcat(comm,"Nu exista o astfel de melodie\n");
                                        break;
                                    }
                                        
                                }
                                sqlite3_finalize(stmt);
                                sqlite3_close(db);
                            }
                            send(newSocket, comm, strlen(comm),0);
                            bzero(buffer, 1024);
                            bzero(comm,sizeof(comm));
                            bzero(artist,sizeof(artist));
                            bzero(actualusr,sizeof(actualusr));
                            bzero(actualid,sizeof(actualid));
                        }
                        else if(strstr(buffer,"all")!=NULL)
                        {
                            char comm[1000]="",artist[50],actualusr[100],actualid[100];
                            int k=0,k3=0,k4=0,i,l,r;
                            for(i=9;buffer[i]!=' ';++i)
                            {
                                actualusr[k++]=buffer[i];
                            }
                            actualusr[k]='\0';
                            printf("Clien: %s\n",buffer);
                            if(strcmp(actualusr,"no_user_logged")==0)
                            {
                                strcpy(comm,"Nu sunteti conectat la server!\n");
                            }
                            else 
                            {
                                sqlite3 *db;
                                sqlite3_open("proiectRC.db", &db);
                                char command[1000]="select m.nume_melodie ,m.artist,count(r.like),m.id_melodie FROM Recenzii r JOIN Melodii m ON r.id_melodie=m.id_melodie GROUP BY r.id_melodie ORDER BY count(r.like) DESC";
                                sqlite3_stmt *stmt;
                                const char* col1 ;
                                const char* col2;
                                const char* col3;
                                const char* col4;
                                sqlite3_prepare_v2(db, command, -1, &stmt, 0);
                                while (sqlite3_step(stmt) != SQLITE_DONE)
                                {
                                    col1= sqlite3_column_text(stmt,0);
                                    if(col1!=NULL){
                                        col2= sqlite3_column_text(stmt,1);
                                        col3= sqlite3_column_text(stmt,2);
                                        col4= sqlite3_column_text(stmt,3);
                                        strcat(comm,col1);
                                        strcat(comm," ");
                                        strcat(comm,col2);
                                        strcat(comm," ");
                                        strcat(comm,col3);
                                        strcat(comm," ");
                                        strcat(comm,col4);
                                        strcat(comm,"\n");
                                        }
                                    else
                                    {
                                        strcat(comm,"Nu exista o astfel de melodie\n");
                                        break;
                                    }
                                        
                                }
                                sqlite3_finalize(stmt);
                                sqlite3_close(db);
                            }
                            send(newSocket, comm, strlen(comm),0);
                            bzero(buffer, 1024);
                            bzero(comm,sizeof(comm));
                            bzero(artist,sizeof(artist));
                            bzero(actualusr,sizeof(actualusr));
                            bzero(actualid,sizeof(actualid));
                        }
                    }
                    else 
                    {
                        char comm[1000];
                        printf("Clien: %s\n",buffer);
                        strcpy(comm,"COMANDA INVALIDA!\n");
                        send(newSocket, comm, strlen(comm),0);
                        bzero(buffer, 1024);
                        bzero(comm,sizeof(comm));
                    }
                }
            }
        }
    }
    close(newSocket);

    return 0;
}