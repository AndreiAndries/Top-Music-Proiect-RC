#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 4444
char Loggedusr[1000]="no_user_logged";
char Loggedid[100]="no_id";
int main()
{
    int clientSocket,ret;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == -1)
    {
        printf("[-]Eroare la conexiune\n");
        exit(0);
    }
    printf("[+]Socket-ul cluentului s-a creeat cu succes!\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = connect(clientSocket,(struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret == -1)
    {
        printf("[-]Eroare la conexiune!\n");
        exit(1);
    }
    printf("[+]Conectat la Server!\n");
    while(1)
    {
        if(strcmp(Loggedusr,"no_user_logged")==0)
        {
            printf("Client: Dati o comanda valida:");
        }
        else {
            printf("%s:Dati o comanda valida:",Loggedusr);
        }
        scanf("%s", &buffer[0]);
        if(strcmp(buffer,"Login-usr")==0)
        {
            strcat(buffer," ");
            printf("username: ");
            char a[20];
            scanf("%s",&a[0]);
            strcat(buffer,a);
            strcat(buffer," ");
            printf("passwd: ");
            char b[20];
            scanf("%s",&b[0]);
            strcat(buffer,b);
            strcat(buffer," ");
            strcat(buffer, Loggedusr);
            strcat(buffer," ");
            strcat(buffer,Loggedid);
            strcat(buffer," ");
        }
        else if(strcmp(buffer,"Login-adm")==0)
        {
            strcat(buffer," ");
            printf("username: ");
            char a[20];
            scanf("%s",&a[0]);
            strcat(buffer,a);
            strcat(buffer," ");
            printf("passwd: ");
            char b[20];
            scanf("%s",&b[0]);
            strcat(buffer,b);
            strcat(buffer," ");
            strcat(buffer, Loggedusr);
            strcat(buffer," ");
            strcat(buffer,Loggedid);
            strcat(buffer," ");
        }
        else if(strcmp(buffer,"Signup-usr")==0)
        {
            strcat(buffer," ");
            printf("username: ");
            char a[20];
            scanf("%s",&a[0]);
            strcat(buffer,a);
            strcat(buffer," ");
            printf("passwd: ");
            char b[20];
            scanf("%s",&b[0]);
            strcat(buffer,b);
            strcat(buffer," ");
            strcat(buffer, Loggedusr);
            strcat(buffer," ");
            strcat(buffer,Loggedid);
            strcat(buffer," ");
        }
        else if(strcmp(buffer,"Help")==0)
        {
            printf("\n\n---------------------------\n\n");
            printf("Urmatoarele comenzi sunt valide\n");
            printf("Login-usr : username : passwd\n");
            printf("Login-adm : username : passwd\n");
            printf("Signup-usr : username : passwd\n");
            printf("Help\n");
            printf("Like : song id  - aceasta comanda e folosita pentru a putea vota o piesa\n");
            printf("Comm : song-name : comment  - aceasta comanda e folosita pentru a adauga un comentariu unei melodii\n");
            printf("Show : OPTIONS  - comanda show vine cu mai multe functionalitati iar acesta sunt:\n"); 
            printf(" 1) Show : song-nr-likes : song-id  - arata numarul de like-uri ale unei melodii\n");       
            printf(" 2) Show : song-details : song name  - afiseaza detaliile tuturor melodiilor ce au numele dat\n");
            printf(" 3) Show : song-nr-comms : song-id  -vor fi afisate comentariile unei melodii\n");
            printf(" 4) Show : type : gen_muzical  - vor fi afisate toate melodiile ce se incadreaza in genul muzical dat\n");
            printf(" 5) Show : Song : songname  - vor fi afisate melodiile ce au numele dat impreuna cu artistii care le-au compus si link-urile de yt\n");
            printf(" 6) Show : artist : nume_artist  - afiseaza toate melodiile unui artist\n");
            printf(" 7) Show : all  - afiseaza intreg topul muzical\n");
            printf("Disconnect\n");
            printf("addsong  - un cluent poate adauga o noua melodie in functie de intrebarile care ii sunt puse la apelul comenzii\n");
            printf("\n COMENZILE AFISATE PANA LA ACEASTA LINIE SUNT DISPONIBILE PENTRU TOTI UTILIZATORII\n");
            printf("\n COMENZILE CE URMEAZA POT FI FOLOSITE DOAR DE ADMINISTRATORI\n\n");
            printf("Ban-all-likes : user-id  - aceasta comanda restrictioneaza toate drepturile unui utilizator de a da like-uri\n");
            printf("Ban-likes : gen_muzical  - aceasta comanda restrictioneaza drepturile unui utilizator de a da like-uri melodiilor dintr-un anumit gen muzical\n");
            printf("Delete-song : song-id  - comanda aceasta sterge o melodie din top\n");
            printf("buff : user-id  - aceasta comanda permite unui admin sa promoveze un user obisnuit intr-un admin\n");
            printf("\nPENTRU A FOLOSI COMENZILE TASTATI DOAR PRIMELE PARTI ALE FIECAREI COMENZI\n");
            printf("RESTUL INTEROGETIILOR LE VETI PRIMI PE PARCURS\n");
            printf("\n\n---------------------------\n\n");
            strcat(buffer," ");
        }
        else if(strcmp(buffer,"Comm")==0)
        {
            strcat(buffer," ");
            printf("song_id: ");
            char a[20];
            scanf("%s",&a[0]);
            strcat(buffer,a);
            strcat(buffer," ");
            printf("comment: ");
            char b[20];
            scanf("%s",&b[0]);
            strcat(buffer,b);
            strcat(buffer," ");
            strcat(buffer, Loggedusr);
            strcat(buffer," ");
            strcat(buffer,Loggedid);
            strcat(buffer," ");
        }
        else if(strcmp(buffer,"Like")==0)
        {
            strcat(buffer," ");
            printf("song_id: ");
            char a[20];
            scanf("%s",&a[0]);
            strcat(buffer,a);
            strcat(buffer," ");
            strcat(buffer, Loggedusr);
            strcat(buffer," ");
            strcat(buffer,Loggedid);
            strcat(buffer," ");
        }
        else if(strcmp(buffer,"addsong")==0)
        {
            strcat(buffer," ");
            printf("dati numele melodiei: ");
            char a[20];
            scanf("%s",&a[0]);
            strcat(buffer,a);
            strcat(buffer," ");
            printf("dati numele artistului care a compus melodia in formatul <nume de scena> sau <nume>_<prenume>: ");
            char b[20];
            scanf("%s",&b[0]);
            strcat(buffer,b);
            strcat(buffer," ");
            printf("dati link-ul de youtube al melodiei: ");
            char c[20];
            scanf("%s",&c[0]);
            strcat(buffer,c);
            strcat(buffer," ");
            printf("dati genul muzical in care se incadreaza melodia: ");
            char d[20];
            scanf("%s",&d[0]);
            strcat(buffer,d);
            strcat(buffer," ");
            strcat(buffer, Loggedusr);
            strcat(buffer," ");
            strcat(buffer,Loggedid);
            strcat(buffer," ");
        }
        else if(strcmp(buffer,"Ban-all-likes")==0)
        {
            strcat(buffer," ");
            printf("dati id-ul user-ului: ");
            char a[20];
            scanf("%s",&a[0]);
            strcat(buffer,a);
            strcat(buffer," ");
            strcat(buffer, Loggedusr);
            strcat(buffer," ");
            strcat(buffer,Loggedid);
            strcat(buffer," ");
        }
        else if(strcmp(buffer,"Ban-likes")==0)
        {
            strcat(buffer," ");
            printf("dati genul muzical: ");
            char a[20];
            scanf("%s",&a[0]);
            strcat(buffer,a);
            strcat(buffer," ");
            printf("dati id-ul user-ului: ");
            char b[20];
            scanf("%s",&b[0]);
            strcat(buffer,b);
            strcat(buffer," ");
            strcat(buffer, Loggedusr);
            strcat(buffer," ");
            strcat(buffer,Loggedid);
            strcat(buffer," ");
        }
        else if(strcmp(buffer,"Delete-song")==0)
        {
            strcat(buffer," ");
            printf("dati id-ul melodiei: ");
            char a[20];
            scanf("%s",&a[0]);
            strcat(buffer,a);
            strcat(buffer," ");
            strcat(buffer, Loggedusr);
            strcat(buffer," ");
            strcat(buffer,Loggedid);
            strcat(buffer," ");
        }
        else if(strcmp(buffer,"buff")==0)
        {
            strcat(buffer," ");
            printf("dati id-ul user-ului: ");
            char a[20];
            scanf("%s",&a[0]);
            strcat(buffer,a);
            strcat(buffer," ");
            strcat(buffer, Loggedusr);
            strcat(buffer," ");
            strcat(buffer,Loggedid);
            strcat(buffer," ");
        }
        else if(strcmp(buffer,"Show")==0)
        {
            strcat(buffer," ");
            printf("dati obtiunea dorita: ");
            char a[70];
            scanf("%s",&a[0]);
            strcat(buffer,a);
            char b[40];int ok=0;
            while(ok==0)
            {
                if(strcmp(a,"song-nr-likes")==0)
                {
                    printf("dati id-ul melodiei: ");
                    scanf("%s",&b[0]);
                    ok=1;
                }
                else if(strcmp(a,"song-details")==0)
                {
                    printf("dati numele melodiei: ");
                    scanf("%s",&b[0]);
                    ok=1;
                }
                else if(strcmp(a,"song-nr-comms")==0)
                {
                    printf("dati numele melodiei: ");
                    scanf("%s",&b[0]);
                    ok=1;
                }
                else if(strcmp(a,"type")==0)
                {
                    printf("dati genul muzical: ");
                    scanf("%s",&b[0]);
                    ok=1;
                }
                else if(strcmp(a,"Song")==0)
                {
                    printf("dati numele melodiei: ");
                    scanf("%s",&b[0]);
                    ok=1;
                }
                else if(strcmp(a,"artist")==0)
                {
                    printf("dati numele artistului in format\n <nume de scena> sau <nume>_<prenume>: ");
                    scanf("%s",&b[0]);
                    ok=1;
                }
                else if(strcmp(a,"all")==0)
                {
                    printf("\n\nTOP MUZICAL\n");
                    ok=1;
                }
                else printf("obtiune invalida incercati alta: ");
            }
            strcat(buffer," ");
            strcat(buffer,b);
            strcat(buffer," ");
            strcat(buffer, Loggedusr);
            strcat(buffer," ");
            strcat(buffer,Loggedid);
            strcat(buffer," ");
        }
        send(clientSocket, buffer, strlen(buffer), 0);
        if(strcmp(buffer, "disconnect") == 0)
        {
            close(clientSocket);
            printf("[-]Deconectat de la server.\n");
            exit(2);
        }
        bzero(buffer,1024);
        if(recv(clientSocket, buffer,1024,0)<0)
        {
            printf("[-]Eroare la primirea datelor!\n");
            exit(3);
        }
        else{
            printf("Server: \t%s\n", buffer);
            if(strstr(buffer,"conectat cu succes")!=NULL)
            {
                char cp[100],up[1000];
                int k1=0,k2=0,l,r;
                for(l=0;buffer[l]!=' ';++l)
                {
                    cp[k1++]=buffer[l];
                }
                cp[l]='\0';
                for(r=l+1;buffer[r]!=' ';++r)
                {
                    up[k2++]=buffer[r];
                }
                up[k2]='\0';
                strcpy(Loggedusr,cp);
                strcpy(Loggedid,up);
                bzero(buffer,1024);
            }
        }
    }
    return 0;
}