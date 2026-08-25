# ProcessFlow

ProcessFlow é um orquestrador de processos desenvolvido em C para a disciplina de Infraestrutura de Software.

O programa permite cadastrar tarefas e executá-las de forma simples, sequencial, paralela, utilizando pipelines, redirecionamentos e execução em background. Também possui suporte a arquivos de workflow e alteração do diretório de trabalho.

## Compilação

Para compilar o programa:

```bash
make clean
make
```

O executável gerado será:

```text
processflow
```

## Execução

### Modo interativo

```bash
./processflow
```

O programa apresentará o prompt:

```text
processflow>
```

### Modo workflow

```bash
./processflow teste.pf
```

Nesse modo, os comandos presentes no arquivo `.pf` são lidos e processados sem apresentar o prompt interativo.

## Testes

Os testes foram realizados manualmente pelo terminal.

### Cadastro e execução simples

```text
task listar /bin/ls -l
run listar
```

### Execução sequencial

Após cadastrar as tarefas:

```text
run sequential tarefa1 tarefa2 tarefa3
```

### Execução paralela

Após cadastrar as tarefas:

```text
run parallel tarefa1 tarefa2 tarefa3
```

### Pipeline

Após cadastrar as tarefas listar, ordenar e contar:

```text
run pipe listar ordenar contar
```

### Redirecionamentos

Os comandos de redirecionamento configuram a tarefa. A execução acontece posteriormente com `run`.

```text
output escrever saida.txt
run escrever

append adicionar saida.txt
run adicionar

input mostrar saida.txt
run mostrar
```

### Mudança de diretório

```text
workdir /tmp
```

### Execução em background

```text
start tarefa
jobs
wait <jobId>
```

### Encerrar o programa

```text
exit
```

Os testes realizados durante o desenvolvimento estão registrados no arquivo `evidencias.log`.

## GitHub

https://github.com/juliatenoriocalado/Atividade_01_IS_jtc
