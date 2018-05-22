
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h> // coloquei pra bzero
#include <unistd.h>
#include <string.h>
#define SA struct sockaddr
// fiz a echo só pra rodar mesmo mas por enquanto ela não faz nada sem o cliente então ignorem,precisamos saber mais sobre a mesma
// COMENTÁRIO EM MAIUSCULO SÃO COISAS QUE EU JULGUEI SEREM INTERESSANTES DE RESSALTAR.DEPOIS EU ORGANIZO.
void str_echo(int sockfd)

{
	ssize_t n;
	char buf[4096], h[1];
	FILE *pfile;
	h[0] ='\n';
	h[1] ='\0';
	int i;
	char msgenviada [4096];
//CLIENTE ENVIA AS INFORMAÇÕES DO LOG.TXT PARA O MSGANT RECEBER TUDO O QUE JÁ FOI DITO
	pfile = fopen("log.txt", "r");
    while (fgets(msgenviada,4096, pfile) != NULL){}
    fclose(pfile);
	printf("Mensagem enviada : %s\n", msgenviada);
	write(sockfd, msgenviada,4096);

	while(strcmp(buf, "xau") != 0)
	{


		//Zerou o buff
		for(i=0;i < 4096; i++)
		{
			buf[i] = 0;
		}
		
		read(sockfd,buf,4096);// le oque ele escreve

		if (buf[0] != '&')
		{

			pfile = fopen("log.txt", "a");//abre arquivo
		fputs(buf, pfile);//coloca buf no arquivo
		fclose(pfile);//fecha arquivo
		

		//printf("Mensagem recebida : %s\n", buf);
    	
    	pfile = fopen("log.txt", "r");
    	
    	while (fgets(msgenviada,4096, pfile) != NULL){}
    	
    	fclose(pfile);
		//printf("Mensagem enviada : %s\n", msgenviada);
		write(sockfd, msgenviada,4096);

		}
		else
		{
			msgenviada [0] = '&';
			write(sockfd, msgenviada,4096);
		}
		
		

	}

}
int main()
{
 	int meusocket, resposta_server, a=0,i;
 	pid_t  criacao_filhos;
 	socklen_t tamanho_cliente;//coloquei o socklen aqui, QUE TEM PELO MENOS 32 BITS,TAMANHO DE ALGUMA COISA,NO CASO ELE ENTRA NO ACCEPT ENTÃO É DO ENDEREÇO
 	char buff[50],user[50];  
 	struct sockaddr_in cliaddr, servaddr;// CRIAÇÃO DA VARIÁVEL TIPO STRUCT
 	 	

 	meusocket = socket(AF_INET, SOCK_STREAM, 0);
 	bzero(&servaddr, sizeof(servaddr));
 	servaddr.sin_family = AF_INET;
 	servaddr.sin_addr.s_addr = htonl(0);// DIZ NO LIVRO QUE PODE SER QUALQUER VALOR AQUI QUE NORMALMENTE É A CONSTANTE NUMERICA 0  
 	servaddr.sin_port = htons(9503);//HTONS E HTONL RETORNAM O VALOR NA ORDEM DE BYTES DA REDE.

 	bind(meusocket, (SA*)&servaddr, sizeof(servaddr));
	// bind(): QUANDO UM SOCKET É CRIADO,ELE TEM UM NOME(ENDEREÇO DA FAMILIA/PROTOCOLO DA FAMILIA) PORÉM ELE NÃO TEM UM ENDEREÇO.
	//BIND VAI FAZER COM QUE SEJA DESIGNADO UM ENDEREÇO LOCAL( OU PORTA LOCAL) PARA ESSE SOCKET ESPECIFICADO PELO ADDR REFERENTE PELO DESCRITOR(QUE FOI CRIADO PELO AF_INET,QUE DEVER O SOCKFD??) DO ARQUIVO.
	//O BIND DEVE SER EXECUSerTADO ANTES DE QUALQUER CONEXÃO DO CLIENTE COM O SERVIDOR,PORQUE AÍ O SERV VAI TER UM ENDEREÇO,SÓ ENTÃO ELE DEVE ACEITAR CONEXÕES.
	//É O BIND QUE VAI VER SE A PORTA JÁ ESTÁ SENDO USADA POR OUTRO PROCESSO.
	// BASICAMENTE, O PAPEL DO BIND É DESIGNAR UM ENDEREÇO AO SOCKET PARA O CLIENTE PODER USAR O MESMO PARA SE CONECTAR AO SERVIDOR.É USADO PRA DEFINIR O PONTO FINAL DA COMUNICAÇÃO(ACHO QUE ONDE A COMUNICAÇÃO VAI OCORRER??).
 	
 	listen(meusocket, 1024);

 	
  	for(;;)
 	{
 		printf("aguardando cliente\n");
 		tamanho_cliente = sizeof(cliaddr);
 		resposta_server = accept( meusocket,(SA*)&cliaddr, &tamanho_cliente);
 		if (resposta_server==-1) printf("Erro na função accept\n");
 		if((criacao_filhos = fork()) == 0)
 		{
 			printf("Fork criado com sucesso\n");
 			close(meusocket);
 			
 			read(resposta_server,user,4096); // le o nome do usuario

 			printf("Nome do usuário lido\n");
 		
 		//	write(resposta_server,user,4096); // envia o nome do usuario


 			
 			printf("Cliente conectado com sucesso! %s\n", user);
 			str_echo(resposta_server);
 			exit(0);
 		}
 		else
 		{
 			//printf("falha no fork!");
 		}
 		close(resposta_server);
 	}
 	// acho que tá tudo direitinho...
	// acho que o programa cai no loop quando executado.
	return 0;
}
