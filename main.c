#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

typedef struct Comando {
    char nome[30];
    char programa[30];
    char argumentos[10][30];
    int quantidadeDeArgumentos;
} Comando;

int main(int argc, char *argv[]){

    char linhaDeComandoEscrita[100];
    int achouOuNao = 0;
    int quantidadeDeTarefas = 0;
    Comando tarefas[10];


    while (achouOuNao == 0){

        printf("processflow> ");

        fgets(linhaDeComandoEscrita, 100, stdin);
        linhaDeComandoEscrita[strcspn(linhaDeComandoEscrita, "\n")] = '\0';
        int comparacao = strcmp(linhaDeComandoEscrita, "exit");

        if (comparacao == 0){
            achouOuNao = 1;
        }else{

            int contadorDeEntradas = 0;
            Comando ComandoAtual;
            ComandoAtual.quantidadeDeArgumentos = 0;
            char *ponteiroEntradaSerQuebrada;
            ponteiroEntradaSerQuebrada = strtok(linhaDeComandoEscrita, " "); //aspa dupla pq lê string
            
            int qualComandoQueEscolheram = 0;

            while(ponteiroEntradaSerQuebrada){
               
                printf("token: %s\n",  ponteiroEntradaSerQuebrada);
               
                if (contadorDeEntradas == 0){
                    if(strcmp(ponteiroEntradaSerQuebrada, "task") == 0){
                        qualComandoQueEscolheram = 1;
                    }

                    if (strcmp(ponteiroEntradaSerQuebrada, "run") == 0){
                        qualComandoQueEscolheram = 2;
                    }
                }

                if (contadorDeEntradas == 1){
                    strcpy(ComandoAtual.nome, ponteiroEntradaSerQuebrada);
                }
                
                if (qualComandoQueEscolheram == 1 && contadorDeEntradas == 2){
                    strcpy(ComandoAtual.programa, ponteiroEntradaSerQuebrada);
                }
                
                if (qualComandoQueEscolheram == 1 && contadorDeEntradas >= 3){
                    strcpy(ComandoAtual.argumentos[ComandoAtual.quantidadeDeArgumentos], ponteiroEntradaSerQuebrada);
                    ComandoAtual.quantidadeDeArgumentos++;
                } 

                contadorDeEntradas++;
                ponteiroEntradaSerQuebrada = strtok(NULL, " ");
                
    }

            int PosicaoEncontrada = -1; //nao encontrou    

            if (qualComandoQueEscolheram == 1){
                tarefas[quantidadeDeTarefas] = ComandoAtual;
                quantidadeDeTarefas++;
            }

            if (qualComandoQueEscolheram == 2){
                for (int i=0; i<quantidadeDeTarefas; i++){
                    if (strcmp(tarefas[i].nome, ComandoAtual.nome) == 0){
                    printf("tarefa na posicao %d\n", i);
                    PosicaoEncontrada = i;
                    }
                }
            }
        
        printf("nome: %s\n", ComandoAtual.nome);
        printf("programa: %s\n", ComandoAtual.programa);
        printf("quantidades de argumentos: %d\n", ComandoAtual.quantidadeDeArgumentos);
        for (int i=0; i< ComandoAtual.quantidadeDeArgumentos; i++){
            printf("argumento(s)[%d]: %s\n", i, ComandoAtual.argumentos[i]);
        }

        if (qualComandoQueEscolheram == 2 && PosicaoEncontrada != -1){

            pid_t pid = fork(); //O retorno da função fork corresponde ao comportamento ocorrido 

            if (pid < 0){
                perror("Fork falhou.");
                exit(1); //Encerrar processo atual
            } 
            
            if (pid > 0){
                //Estamos no processo pai
                printf("Processo Pai PID = %d Processo pai espera pelo Filho PID = %d\n", getpid(), pid);
                wait(NULL ); //Criar filho com a CS fork, o pai vai iniciar o código e quando chegar na CA wait ele vai esperar o filho pra continuar a execução
                printf("Processo pai encerrou.\n");
            }else{ //Senão estou no processo filho

                printf("Processo Filho PID = %d Pai possui PID = %d\n", getpid(), getppid()); //getppid conseguir PID do processo pai
                sleep(5); //filho dormindo

                //com um l: argumentos separados com virgula (passa diretamente), com v: vetor de strings (se cria, voce gera), p: permite procurar o executável nos diretórios configurados na variável PATH
                char *args[12];
                args[0] = tarefas[PosicaoEncontrada].programa;

                for (int i=0; i < tarefas[PosicaoEncontrada].quantidadeDeArgumentos; i++){
                    args[i + 1] = tarefas[PosicaoEncontrada].argumentos[i];                                   
                }

                args[tarefas[PosicaoEncontrada].quantidadeDeArgumentos + 1] = NULL;
                execvp(args[0], args); //execvp diz : execute ./TestandoExec usando esse vetor de argumentos

                perror("o exec falhou.");
                exit(1); //encerra o processo filho
            
                //exec vai (substituir o processo filho pelo processo chamado exec) -> se o exec funcionar, tudo o que estiver depois do filho nçao será executado 

                } 
            }   
        }
    }   
    return 0;
}