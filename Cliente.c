
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#define SA struct sockaddr

void init_attr(void) ;
void close_attr(void) ;
int kbhit(void) ;

struct termios old_attr, new_attr;

void init_attr(void) {
    /* Obtém as configurações atuais. */
    tcgetattr(0,&old_attr);
    new_attr=old_attr;

    /* Desliga modo canônico. */
    new_attr.c_lflag &=~ICANON ;

    /* Desliga ecoamento. */
    new_attr.c_lflag &= ~ECHO;

    new_attr.c_cc[VTIME]=0 ;
    new_attr.c_cc[VMIN]=0 ;
}

/* Retorna configurações antigas. */
void close_attr(void) {
    tcsetattr(STDIN_FILENO,TCSANOW,&old_attr);
}

int kbhit(void) {
    int c ;

    tcsetattr(STDIN_FILENO,TCSANOW,&new_attr);
    c = getchar() ; /* retorna EOF se nada foi pressionado */
    tcsetattr(STDIN_FILENO,TCSANOW,&old_attr);

    return c ;
}



// fiz a echo só pra rodar mesmo mas por enquanto ela não faz nada sem o cliente então ignorem,precisamos saber mais sobre a mesma
// COMENTÁRIO EM MAIUSCULO SÃO COISAS QUE EU JULGUEI SEREM INTERESSANTES DE RESSALTAR.DEPOIS EU ORGANIZO.
void str_cli(FILE *fp, int meusocket)
{
  int flag_ler_nome = 0;
  char  msgreceb[4096], user[4096],msgant[4096], msg[4096]; // dois chars
  int i,j, ult;
  int p;
  printf("Escolha um nome de usuario: ");
  
  fgets(user, 500, stdin);

  printf("Esse foi o nome que eu peguei: %s\n", user ); 

  write(meusocket, user, strlen(user));
//RECEBE AS INFORMAÇÕES DO BLOCO DE NOTAS E ATRIBUI AO msgant
  read(meusocket, msgreceb, 4096);
  for (i=0; i<strlen (msgreceb); i++)
  {
      msgant [i]= msgreceb [i];
      ult = i;
  }

  char msgenviada [4096];

init_attr() ;
  while(1)
  {

    flag_ler_nome = kbhit() ;

        if(flag_ler_nome != EOF) {
            
    for(i=0;msg[i] != '\0';i++)
      {
        msg[i] = 0;
        msgenviada[i] = 0;
      }
      printf("\n");
    //printf("Digite uma mensagem: ");
    
      printf ("Você: ");
    //PEGA  A STRING MSGENVIADA
    fgets(msgenviada,4096,stdin);

    //COLOCA O NOME DO USUÁRIO NA STRING MSG
      for(i=0;user[i] != '\n';i++)
      {
        msg[i] = user[i];
      }
      //COLOCA DOIS PONTOS E \n AO FINAL DA STRING MSG
      msg[i] = ':';
      i++;
      msg[i] = '\n';

      //APOS O NOME DO USUÁRIO, COLOCAR A MENSAGEM ESCRITA NA MSGENVIADA NA STRING MSG
      for(j=0;msgenviada[j] != '\n'; j++)
      {
        msg[i] = msgenviada[j];
        i++;
      }
      msg[i] = '\n';

      //SUBSTITUI O \N NO FINAL DA STRING POR #
      for(i=0;i<strlen(msg);i++)
      {
        if(msg[i] == '\n') msg[i] = '#';
      }
      //A STRING MSGANT RECEBE A MENSAGEM QUE SERÁ ENVIADA
    for (i=ult+1; i<strlen (msg); i++)
  {
      msgant [i]= msgreceb [i];
      ult = i;
  }

  //  printf("Mensagem enviada : %s\n", msg);

      //ENVIA A MENSAGEM PARA O SERVIDOR, EX.: MAXWELL: mensagem...
    write(meusocket, msg, strlen(msg));// Envia mensagem para servidor

    
        }
        else
        {
          //MANDAR O CARACTERE INVÁLIDO
        msg [0] = '&';
        write(meusocket, msg, strlen(msg));
        } 

//  RECEBE AS INFORMAÇÕES DO SERVIDOR

       read(meusocket, msgreceb, 4096); // Recebe mensagens do servidor
       
       //RECEBE AS INFORMAÇÕES E FAZ A VERIFICAÇÃO
       if (msgreceb [0] != '&')
    { 

        for(i=0;i< strlen(msgant);i++)
       {
        if(msgant[i] == '#') 
        {
        msgant[i] = '\n';
        }     
       }
   
    //O QUE TIVER '#' SERÁ SUBUSTITUIDO POR \n NA MENSAGEM RECEBIDA DO SERVIDOR
    for(i=0;i< strlen(msgreceb);i++)
      {
        if(msgreceb[i] == '#') 
        {
        msgreceb[i] = '\n';
        ult = i; //   FICA COM O INDICE DO ÚLTIMO '#', OU SEJA, O INDICE DE ONDE TERMINOU A MENSAGEM
        //msgant[i] = '\n';
        }     
      }
      //printf("Mensagem recebida:  %s \n Mensagem anterior : %s \n", msgreceb, msgant);
    for(i=0; i < ult;i++)
      {
        if(msgreceb[i] != msgant[i])
        {
          printf("%c", msgreceb[i]);
        //  printf("< %c >", msgant[i]);
        }
      }
    //printf(" %s\n", msgreceb);//printa mensagem recebida
    for(i=0;i < 4096;i++)
      {
        msgant[i] = msgreceb[i];
      //  printf("<%c>", msgant[i]);
        
      }
      for(i=0;i < 4096;i++)
      {
        msgreceb[i] = 0;
      //  printf("<%c>", msgant[i]);
        
      }
    }
  }      
        /*
       char msgenviada [4096] ="MAXWELL";
       write (meusocket, msgenviada, strlen(msgenviada));
       printf("Você enviou: %s\n", msgenviada );

       read(meusocket, msgreceb, 4096);
       printf("Você recebeu: %s\n");
    */
   
        
    
    close_attr() ;
    
    
  }


void clientegroup()
{
  int meusocket;
  char ippp[16];
  char* ipp;
  const char* ip;


  printf("Digite o ip do chat em grupo : \n");
  fgets(ippp,16,stdin);
  ippp[strlen(ippp)] = 0;
  ipp = ippp;
  ip = ipp;
  struct sockaddr_in servaddr;

  /*
  if(argc != 2)//se você não digitar ip ele fecha
  {
    return 0;
  }
  */


  meusocket = socket(AF_INET, SOCK_STREAM, 0); // abre o socket

  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET; // familia do socket

  servaddr.sin_port = htons(9503); // porta do socket

  inet_pton(AF_INET, ip, &servaddr.sin_addr); // 
  
  connect(meusocket, (SA *) &servaddr, sizeof(servaddr)); //conetcando cliente em determidado socket,
                              // usando o valore de retorno do socket, a suct do socket e o tamanho 
                              // da struct severaddr 
  
  str_cli(stdin, meusocket); //entra na função que envia e recebe mensagens

  exit(0);
}
void serverd()
{
  int meusocket, resposta_server, port = 9502, a=0;
  pid_t  criacao_filhos;
  socklen_t tamanho_cliente;//coloquei o socklen aqui, QUE TEM PELO MENOS 32 BITS,TAMANHO DE ALGUMA COISA,NO CASO ELE ENTRA NO ACCEPT ENTÃO É DO ENDEREÇO

  struct sockaddr_in cliaddr, servaddr;// CRIAÇÃO DA VARIÁVEL TIPO STRUCT

  meusocket = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(0);// DIZ NO LIVRO QUE PODE SER QUALQUER VALOR AQUI QUE NORMALMENTE É A CONSTANTE NUMERICA 0  
  servaddr.sin_port = htons(port);//HTONS E HTONL RETORNAM O VALOR NA ORDEM DE BYTES DA REDE.

  bind(meusocket, (SA*)&servaddr, sizeof(servaddr));
  // bind(): QUANDO UM SOCKET É CRIADO,ELE TEM UM NOME(ENDEREÇO DA FAMILIA/PROTOCOLO DA FAMILIA) PORÉM ELE NÃO TEM UM ENDEREÇO.
  //BIND VAI FAZER COM QUE SEJA DESIGNADO UM ENDEREÇO LOCAL( OU PORTA LOCAL) PARA ESSE SOCKET ESPECIFICADO PELO ADDR REFERENTE PELO DESCRITOR(QUE FOI CRIADO PELO AF_INET,QUE DEVER O SOCKFD??) DO ARQUIVO.
  //O BIND DEVE SER EXECUTADO ANTES DE QUALQUER CONEXÃO DO CLIENTE COM O SERVIDOR,PORQUE AÍ O SERV VAI TER UM ENDEREÇO,SÓ ENTÃO ELE DEVE ACEITAR CONEXÕES.
  //É O BIND QUE VAI VER SE A PORTA JÁ ESTÁ SENDO USADA POR OUTRO PROCESSO.
  // BASICAMENTE, O PAPEL DO BIND É DESIGNAR UM ENDEREÇO AO SOCKET PARA O CLIENTE PODER USAR O MESMO PARA SE CONECTAR AO SERVIDOR.É USADO PRA DEFINIR O PONTO FINAL DA COMUNICAÇÃO(ACHO QUE ONDE A COMUNICAÇÃO VAI OCORRER??).
  
  listen(meusocket, 1024);
  
  printf("Escolha uma sala porta para se concetar: ");
  scanf("%d", &port);
  for(;;)
  {
    printf("aguardando cliente\n");
    tamanho_cliente = sizeof(cliaddr);
    resposta_server = accept( meusocket,(SA*)&cliaddr, &tamanho_cliente); 
    if((criacao_filhos = fork()) == 0)
    {
      close(meusocket);
      printf("Cliente conectado com sucesso!\n");
      msgservd(resposta_server);
      exit(0);
    }
    else
    {
      printf("falha no fork!");
    }
    close(resposta_server);
  }
  // acho que tá tudo direitinho...
  // acho que o programa cai no loop quando executado.
}
void msgservd(int sockfd)
{
  ssize_t n;
  char buf[4096];
  while(strcmp(buf, "xau") != 0)
  {
    //printf("Mensgam recebida : %s \n", buf);
    //fgets(buf, 4096, stdin);
    printf("aguardando mensagem ...\n");
    printf("%d", sockfd);
    read(sockfd,buf,4096);// le oque ele escreve

    printf("Mensagem recebida : %s\n", buf);

    printf("Digite uma mensagem : ");

    fgets(buf, 4096, stdin);

    write(sockfd,buf,4096);

    printf("Mensagem enviada : %s\n", buf);
  }
}
void clid()
{
  int meusocket, port = 9502;
  char ippp[50];
  char* ipp;
  const char* ip;
  struct sockaddr_in servaddr;

  fgets(ippp,13,stdin);
  
  ipp = ippp;
  ip = ipp;

  /*
  if(argc != 2)//se você não digitar ip ele fecha
  {
    return 0;
  }
  */
  meusocket = socket(AF_INET, SOCK_STREAM, 0); // abre o socket

  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET; // familia do socket

  servaddr.sin_port = htons(port); // porta do socket

  inet_pton(AF_INET, ip, &servaddr.sin_addr); // 
  
  printf("por favor escolha uma porta : ");

  scanf("%d", &port);
  
  connect(meusocket, (SA *) &servaddr, sizeof(servaddr)); //conetcando cliente em determidado socket,
                              // usando o valore de retorno do socket, a suct do socket e o tamanho 
                              // da struct severaddr 
  
  msgclid(stdin, meusocket); //entra na função que envia e recebe mensagens

  exit(0);
}
void msgclid(FILE *fp, int meusocket)
{
  char  msgenviada[4096], msgreceb[4096], user[500]; // dois chars
  int i;
  int p;
  
  printf("Escolha um nome de usuario: ");
  getchar();
  fgets(user, 500, stdin);
  
  while(strcmp(msgenviada, "xau") != 0 && strcmp(msgreceb, "xau") != 0)
  {
    

    printf("Digite uma mensagem: ");
    
    fgets(msgenviada,4096,fp);
    
    printf("%s%s\n", user,msgenviada);

    write(meusocket, msgenviada, strlen(msgenviada));
    
    printf("aguardando mensagem ...\n");
  
    read(meusocket, msgreceb, 4096);

    printf(" Mensagem recebida: %s\n", msgreceb);
    //printf("Mensagem enviada: %s ", msgenviada);
    
  }
}
int main()
{
  int op;
  printf("Bem vindo ao chat!!\nSe quiser entrar na sala de grupo digite 1\nSe quiser criar uma sala privada digite 2\nSe quiser entrar numa sala privada digite 3\n");
  scanf("%d", &op);
  if(op == 1)
  {
    getchar();
    clientegroup();
  }
  if(op == 2)
  {
    getchar();
    serverd();
  }
  if(op == 3)
  {
    getchar();
    clid();
  }

  return 0;
}
