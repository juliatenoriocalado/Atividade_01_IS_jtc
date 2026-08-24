#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

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
    int sequencial = 0, paralelo = 0;
    int runQuantidade = 0;
    char nomesDoRun[20][30];
    int modoPipe = 0;
    char nomeDoArquivo[30];
    int ModoAppend = 0;
    int ModoOutput = 0;
    int ModoInput = 0;
    int workflow = 0;
    FILE *arquivoWorkflow;
    int modoWorkDir = 0;
    char diretorioWorkDir[30]; 
    char buffer[1024]; //criação de um buffer (array de caracteres) com 1024 caracteres para armazenas o diretori ode trabalho atual
    int modoStart = 0;
    int quantidadeDeJobs = 0;
    pid_t background[30];
    int modoJobs = 0;
    int modoWait = 0;
    pid_t pidProcurado = 0;

    if (argc == 2){
        arquivoWorkflow = fopen(argv[1], "r"); //r abrir arquivo p leitura

        if (arquivoWorkflow == NULL){
            perror("erro ao abrir o arquivo");
            exit(1);
        }
        
        else{
            workflow = 1;
        }
    }
    
    else if (argc > 2){
        printf("chamou argumentos demais\n");
        exit(1);
    }
    
    else{

        //faço nada pq vai pro interativo

    }

    while (achouOuNao == 0){

        if (workflow == 1){
            //imprimir cada linha de comando antes de ser processada porque marcelo pediu
            if (fgets(linhaDeComandoEscrita, 100, arquivoWorkflow) == NULL){
                break;
            }
            
            else{
                printf("%s", linhaDeComandoEscrita);
            }

        }

        if (workflow == 0){
            printf("processflow> ");
            fgets(linhaDeComandoEscrita, 100, stdin);
        }

        linhaDeComandoEscrita[strcspn(linhaDeComandoEscrita, "\n")] = '\0';
        int comparacao = strcmp(linhaDeComandoEscrita, "exit");

        if (comparacao == 0){
            achouOuNao = 1;
        }else{

            modoPipe = 0;
            sequencial = 0;
            paralelo = 0;
            runQuantidade = 0;
            ModoAppend = 0;
            ModoInput = 0;
            ModoOutput = 0;
            modoWorkDir = 0;
            modoStart = 0;
            modoJobs = 0;
            modoWait = 0;
            pidProcurado = 0;

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

                    if (strcmp(ponteiroEntradaSerQuebrada, "input") == 0){
                        ModoInput = 1;
                    }

                    if (strcmp(ponteiroEntradaSerQuebrada, "output") == 0){
                        ModoOutput = 1;
                    }

                    if (strcmp(ponteiroEntradaSerQuebrada, "append") == 0){
                        ModoAppend = 1;
                    }

                    if (strcmp(ponteiroEntradaSerQuebrada, "workdir") == 0){
                        modoWorkDir = 1;
                    }

                    if (strcmp(ponteiroEntradaSerQuebrada, "start") == 0){
                        modoStart = 1;
                    }

                    if (strcmp(ponteiroEntradaSerQuebrada, "jobs") == 0){
                        modoJobs = 1;
                    }

                    if (strcmp(ponteiroEntradaSerQuebrada, "wait") == 0){
                        modoWait = 1;
                    }

                }

                if (contadorDeEntradas == 1){
                    strcpy(ComandoAtual.nome, ponteiroEntradaSerQuebrada);
                    if (strcmp(ComandoAtual.nome, "sequential") == 0){
                        sequencial = 1;
                    }if (strcmp(ComandoAtual.nome, "parallel") == 0){
                        paralelo = 1;
                    }if (strcmp(ComandoAtual.nome, "pipe") == 0){
                        modoPipe = 1;
                    }if (modoWorkDir == 1){
                        strcpy(diretorioWorkDir,  ponteiroEntradaSerQuebrada);
                    }if (modoWait == 1){
                        pidProcurado = atoi(ponteiroEntradaSerQuebrada);
                    }
                    
                }

                if (contadorDeEntradas == 2 && ModoAppend == 1){
                    strcpy(nomeDoArquivo, ponteiroEntradaSerQuebrada);
                    
                }

                if (contadorDeEntradas == 2 && ModoInput == 1){
                    strcpy(nomeDoArquivo, ponteiroEntradaSerQuebrada);
                }

                if (contadorDeEntradas == 2 && ModoOutput == 1){
                    strcpy(nomeDoArquivo, ponteiroEntradaSerQuebrada);
                }
                
                if (qualComandoQueEscolheram == 1 && contadorDeEntradas == 2){
                    strcpy(ComandoAtual.programa, ponteiroEntradaSerQuebrada);
                }
                
                if (qualComandoQueEscolheram == 1 && contadorDeEntradas >= 3){
                    strcpy(ComandoAtual.argumentos[ComandoAtual.quantidadeDeArgumentos], ponteiroEntradaSerQuebrada);
                    ComandoAtual.quantidadeDeArgumentos++;
                }
                
                if (qualComandoQueEscolheram == 2 && contadorDeEntradas >= 2){
                    strcpy(nomesDoRun[runQuantidade], ponteiroEntradaSerQuebrada);
                    runQuantidade++;
                }

                contadorDeEntradas++;
                ponteiroEntradaSerQuebrada = strtok(NULL, " ");
            }

        if (sequencial){
            for (int i=0; i<runQuantidade; i++){
                int PosicaoEncontrada = -1;
                for (int j=0; j<quantidadeDeTarefas; j++){
                    if (strcmp(tarefas[j].nome, nomesDoRun[i]) == 0){
                        PosicaoEncontrada = j;
                    }
                }

                if (PosicaoEncontrada != -1){

                    pid_t pid = fork();
                    if (pid < 0){
                        perror("Fork falhou.");
                        exit(1); //Encerrar processo atual
                    }

                    if (pid > 0){
                        //Estamos no processo pai
                        printf("Processo Pai PID = %d Processo pai espera pelo Filho PID = %d\n", getpid(), pid);
                        wait(NULL);                        
                        printf("Processo pai encerrou.\n");
                    }

                    else { //Senão estou no processo filho
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
                }
            }
        }
    }           
                
        if (paralelo){
            int quantidadeFilhos = 0;
            for (int i=0; i<runQuantidade; i++){
                int PosicaoEncontrada = -1;
                for (int j=0; j<quantidadeDeTarefas; j++){
                    if (strcmp(tarefas[j].nome, nomesDoRun[i]) == 0){
                        PosicaoEncontrada = j;
                    }
                }

            if (PosicaoEncontrada != -1){

                pid_t pid = fork();
                if (pid < 0){
                    perror("Fork falhou.");
                    exit(1); //Encerrar processo atual
                }

                if (pid > 0){
                    //Estamos no processo pai
                    quantidadeFilhos++;
                    printf("Processo Pai PID = %d Processo pai espera pelo Filho PID = %d\n", getpid(), pid);
                    printf("Processo pai encerrou.\n");
                }

            else { //Senão estou no processo filho
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

                   }
                }
            }

            for (int i=0; i<quantidadeFilhos; i++){
                wait(NULL);
            }
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
                    wait(NULL);
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

        if (modoPipe){
            int quantidadeFilhos = 0;
            int guardarCoisasParaOPipe[runQuantidade - 1][2]; //o primeiro indice escolhe qual pipe e o segundo escolhe qual ponta ([i][0] se for leitura e [i][1] se for escrita)
            for (int i=0; i<runQuantidade - 1; i++){
                if (pipe(guardarCoisasParaOPipe[i]) == -1){
                    printf("deu erro ai no pipe\n");
                    return 1;
                }
            }

            for (int i=0; i<runQuantidade; i++){
                int PosicaoEncontrada = -1;
                for (int j=0; j<quantidadeDeTarefas; j++){
                    if (strcmp(tarefas[j].nome, nomesDoRun[i]) == 0){
                        PosicaoEncontrada = j;
                    }
                }

            if (PosicaoEncontrada != -1){

                pid_t pid = fork();
                if (pid < 0){
                    perror("Fork falhou.");
                    exit(1); //Encerrar processo atual
                }

                if (pid > 0){
                    //Estamos no processo pai
                    quantidadeFilhos++;
                    printf("Processo Pai PID = %d Processo pai espera pelo Filho PID = %d\n", getpid(), pid);
                    printf("Processo pai encerrou.\n");
                }

            else { //Senão estou no processo filho
                printf("Processo Filho PID = %d Pai possui PID = %d\n", getpid(), getppid()); //getppid conseguir PID do processo pai
                sleep(5); //filho dormindo
                
                if (i > 0){
                    dup2(guardarCoisasParaOPipe[i - 1][0], STDIN_FILENO); //STDIN constante do sistema que representa entrada padrão
                }

                if (i < (runQuantidade - 1)) {
                    dup2(guardarCoisasParaOPipe[i][1], STDOUT_FILENO); //STDOUT constante do sistema que representa saída padrão
                }

                for (int j=0; j<runQuantidade - 1; j++){
                    close(guardarCoisasParaOPipe[j][0]);
                    close(guardarCoisasParaOPipe[j][1]);
                }

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

                   }
                }
            }

            for (int i=0; i<runQuantidade - 1; i++){
                close(guardarCoisasParaOPipe[i][0]);
                close(guardarCoisasParaOPipe[i][1]);
            }

            for (int i=0; i<quantidadeFilhos; i++){
                wait(NULL);
            }
        }

        if (ModoInput){

            int PosicaoEncontrada = -1;
            for (int j=0; j<quantidadeDeTarefas; j++){
                if (strcmp(tarefas[j].nome, ComandoAtual.nome) == 0){
                    PosicaoEncontrada = j;
                }

            if (PosicaoEncontrada != -1){

                pid_t pid = fork();
                if (pid < 0){
                    perror("Fork falhou.");
                    exit(1); //Encerrar processo atual
                }

                if (pid > 0){
                    //Estamos no processo pai
                    printf("Processo Pai PID = %d Processo pai espera pelo Filho PID = %d\n", getpid(), pid);
                    wait(NULL);
                    printf("Processo pai encerrou.\n");
                }

            else { //Senão estou no processo filho

                int ArquivoSimOuNao = open(nomeDoArquivo, O_RDONLY); //Esse RDONLY serve somente p ler

                if (ArquivoSimOuNao == -1){
                    perror("erro ao abrir arquivo");
                    exit(1);
                }

                dup2(ArquivoSimOuNao, STDIN_FILENO); //STDIN constante do sistema que representa entrada padrão
                close(ArquivoSimOuNao);

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

                   }
                }
            }   
        }

        if (ModoOutput){

            int PosicaoEncontrada = -1;
            for (int j=0; j<quantidadeDeTarefas; j++){
                if (strcmp(tarefas[j].nome, ComandoAtual.nome) == 0){
                    PosicaoEncontrada = j;
                }
            }    

            if (PosicaoEncontrada != -1){

                pid_t pid = fork();
                if (pid < 0){
                    perror("Fork falhou.");
                    exit(1); //Encerrar processo atual
                }

                if (pid > 0){
                    //Estamos no processo pai
                    printf("Processo Pai PID = %d Processo pai espera pelo Filho PID = %d\n", getpid(), pid);
                    wait(NULL);
                    printf("Processo pai encerrou.\n");
                }

            else { //Senão estou no processo filho

                int ArquivoSimOuNao = open(nomeDoArquivo, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
                //0644 é um número que representa as permissoes para um grupo, tipo nesse caso é um terceiro argumento pra dizer: 6 = 4 + 2 (leitura e escrita) 4 = leitura 4 = leitura, o zero diz que o numero está sendo escrito em octal (nao sei o que é só aceitei)
                //Abrir pra escrever, apenas ; se nao existir arquivo cria ; apaga o conteudo que tinha 

                if (ArquivoSimOuNao == -1){
                    perror("erro ao abrir arquivo");
                    exit(1);
                }

                dup2(ArquivoSimOuNao, STDOUT_FILENO); //STDOUT constante do sistema que representa saída padrão
                close(ArquivoSimOuNao);
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

                   }
                }
            }   
       
        if (ModoAppend){

            int PosicaoEncontrada = -1;
            for (int j=0; j<quantidadeDeTarefas; j++){
                if (strcmp(tarefas[j].nome, ComandoAtual.nome) == 0){
                    PosicaoEncontrada = j;
                }
            }    

            if (PosicaoEncontrada != -1){

                pid_t pid = fork();
                if (pid < 0){
                    perror("Fork falhou.");
                    exit(1); //Encerrar processo atual
                }

                if (pid > 0){
                    //Estamos no processo pai
                    printf("Processo Pai PID = %d Processo pai espera pelo Filho PID = %d\n", getpid(), pid);
                    wait(NULL);
                    printf("Processo pai encerrou.\n");
                }

            else { //Senão estou no processo filho

                int ArquivoSimOuNao = open(nomeDoArquivo, O_WRONLY | O_CREAT | O_APPEND, 0644); 
                //0644 é um número que representa as permissoes para um grupo, tipo nesse caso é um terceiro argumento pra dizer: 6 = 4 + 2 (leitura e escrita) 4 = leitura 4 = leitura, o zero diz que o numero está sendo escrito em octal (nao sei o que é só aceitei)
                //Abrir pra escrever, apenas ; se nao existir arquivo cria ; acrescenta no final sem apagar o que tinha antes

                if (ArquivoSimOuNao == -1){
                    perror("erro ao abrir arquivo");
                    exit(1);
                }

                dup2(ArquivoSimOuNao, STDOUT_FILENO); //STDOUT constante do sistema que representa saída padrão
                close(ArquivoSimOuNao);
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

                   }
                }
            }   
     
        if (modoStart){     

            PosicaoEncontrada = -1;
            
            for (int i=0; i<quantidadeDeTarefas; i++){
                if (strcmp(tarefas[i].nome, ComandoAtual.nome) == 0){
                    PosicaoEncontrada = i;
                }
            }    

            if (PosicaoEncontrada != -1){
                pid_t pid = fork(); //O retorno da função fork corresponde ao comportamento ocorrido 

                if (pid < 0){
                    perror("Fork falhou.");
                    exit(1); //Encerrar processo atual
                } 
                
                if (pid > 0){
                    //Estamos no processo pai
                    printf("Processo Pai PID = %d Processo pai nao espera pelo Filho PID = %d\n", getpid(), pid);
                    background[quantidadeDeJobs] = pid;
                    quantidadeDeJobs++;

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
        
        if (modoJobs){
            for (int i=0; i<quantidadeDeJobs; i++){
                printf("%d\n", background[i]);
            }
        }

        if (modoWait){
            waitpid(pidProcurado, NULL, 0);
        }

                if (modoWorkDir){
            if (getcwd(buffer, 1024) == NULL){
                printf("deu erro ao tentar trocar de diretorio\n");
                exit(1);
            }

            else{

                printf("diretorio de trabalho atual: \n\n%s\n\n", buffer);

                if (chdir(diretorioWorkDir) == -1){
                    printf("erro ao tentar trocar de diretorio");
                    exit(1);
                }

                if (getcwd(buffer, 1024) == NULL){
                    printf("deu erro ao tentar trocar de diretorio\n");
                    exit(1);
                }

                else{
                    printf("diretorio de trabalho atual: \n\n%s\n\n", buffer); //verificar se deu certo ao trocar             
                }
            }    
        }
    }
}  

    return 0;
}