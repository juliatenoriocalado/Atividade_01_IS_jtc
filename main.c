#include <stdio.h>
#include <stdlib.h>
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
    
    if (pid != 0){
        //Estamos no processo pai
        printf("Processo Pai PID = %d Processo pai espera pelo Filho PID = %d\n", getpid(), pid);
        wait(NULL ); //Criar filho com a CS fork, o pai vai iniciar o código e quando chegar na CA wait ele vai esperar o filho pra continuar a execução
        printf("Processo pai encerrou.\n");
    }else{ //Senão estou no processo filho
        printf("Processo Filho PID = %d Pai possui PID = %d\n", getpid(), getppid()); //getppid conseguir PID do processo pai
        sleep(5); //filho dormindo
        execl("/bin/echo", "echo", "Teste marcelo me odeia", NULL); //sistema exec (pro processo filho empenhar outro processo)
        //execl vai (substituir o processo filho pelo processo chamado exec) -> o exec mata o filho e executa um novo processo no lugar dele, por isso que o filho não dorme, ele morreu ne, ai não espera os 5 segundos.
        sleep(5);
        printf("Filho encerrou.\n");
    } 

    return 0;
}