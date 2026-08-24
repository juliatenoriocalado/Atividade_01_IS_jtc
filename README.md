Para compilar o programa:

```bash
make
```

O executável gerado será `processflow`.

## Execução

### Modo interativo

```bash
./processflow
```

Nesse modo será apresentado o prompt:

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

### Execução simples

```text
task listar /bin/ls -l
run listar
```

### Execução sequencial

```text
run sequential tarefa1 tarefa2 tarefa3
```

### Execução paralela

```text
run parallel tarefa1 tarefa2 tarefa3
```

### Pipeline

```text
run pipe listar ordenar contar
```

### Redirecionamentos

```text
input contar teste.txt
output listar resultado.txt
append listar resultado.txt
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

Os testes realizados durante o desenvolvimento estão registrados no arquivo `evidencias.log`.

## GitHub

https://github.com/juliatenoriocalado/Atividade_01_IS
