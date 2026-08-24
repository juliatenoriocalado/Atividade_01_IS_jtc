# ProcessFlow

ProcessFlow é um orquestrador de processos desenvolvido em C para cadastrar e executar tarefas por meio de processos filhos. O programa possui modo interativo e modo workflow, além de execução simples, sequencial, paralela, pipelines, redirecionamentos, mudança de diretório e execução em background.

## Arquivos

- `main.c` - implementação do ProcessFlow.
- `Makefile` - responsável pela compilação e limpeza do executável.
- `README.md` - instruções para compilação, execução e testes.
- `evidencias.log` - registro dos comandos e testes realizados durante o desenvolvimento.
- `teste.pf` - arquivo utilizado para testar o modo workflow.
- `teste.txt` - arquivo utilizado nos testes de redirecionamento de entrada.

## Sistema operacional

O programa foi desenvolvido e testado em Ubuntu através do WSL no Windows.

## Compilação

Para remover o executável gerado anteriormente:

```bash
make clean
