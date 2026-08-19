git status#include <stdlib.h>

int main(int argc, char *argv[]){

    //PID = Process IDentification => Processo de IDentificação 
    //PID retorno < 0 : erro ao criar o processo filho
    //PID retorno = 0 : processo filho (estado) 
    //PID retorno > 0 = processo pai (estado)

    pid_t pid = fork(); //O retorno da função fork corresponde ao comportamento ocorrido 

    if (pid < 0){
        perror("Fork falhou.");
        exit(l);

    }else if (pid == 0){
        //Estamos no processo filho
        execv(args[0], args);
    }



    return 0;
}