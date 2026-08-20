#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

    //inicia o processflow

    //PID = Process IDentification => Processo de IDentificação 
    //PID retorno < 0 : erro ao criar o processo filho
    //PID retorno = 0 : processo filho (estado) 
    //PID retorno > 0 = processo pai (estado)

    //ISSO SOU EU ESTUDANDO NÃO É IA

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
        char *args[] = {"./TestandoExec", "Marcelo", "MEODEIA", NULL};
        execvp(args[0], args); //execvp diz : execute ./TestandoExec usando esse vetor de argumentos

        perror("o exec falhou.");
        exit(1); //encerra o processo filho
    
        //exec vai (substituir o processo filho pelo processo chamado exec) -> se o exec funcionar, tudo o que estiver depois do filho nçao será executado 

    } 

    return 0;
}