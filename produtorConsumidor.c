#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

// Variaveis Globais para o tamanho do buffer e para o quanto é consumido e produzido por vez.
int tamanho_buffer, itens_por_producao, consumidos_por_vez, conteudo_buffer;

// Buffer é um vetor de inteiros simples onde 1 significa que o espaço foi preenchido e 0 não. 
// Esse buffer será usado através das funções adiciona_ao_buffer e remove_do_buffer como uma pilha,
// essas funções por sua vez serão chamadas pelo produtor e consumidor respectivamente.
int *buffer;

void adiciona_ao_buffer()
{
    // Função usada para adicionar ao buffer uma unidade
    // a função procura pelo primeiro espaço vazio e transforma ele em 1.
    int index_vazio = 0;
    for (int i = 0; buffer[i] == 1; i++)
    {
        index_vazio = i + 1;
    }
    if (index_vazio >= tamanho_buffer)
    {
        printf("Buffer cheio :p\n");
        // Mensagem para caso o buffer esteja cheio.
    }
    else
    {
        buffer[index_vazio] = 1;
        conteudo_buffer += 1;
    }    

}

void remove_do_buffer()
{   
    // Função usada para remover o ultimo elemento do buffer.
    for (int i = tamanho_buffer - 1; i > 0; i--)
    {
        if (buffer[i] == 1)
        {
            buffer[i] = 0;
            conteudo_buffer -= 1;
            break;
        }        
    }
    
}

void * produtor()
{
    // Ciclo de vida do consumidor, caso a quantidade que o consumidor adiciona ao buffer não o faça ultrapassar o limite
    // então o produtor adiciona e dorme, caso contrário ele apenas dorme e espera outra oportunidade.
    while(1)
    {   
        // Adiciona caso tenha espaço
        if ((conteudo_buffer + itens_por_producao) <= tamanho_buffer)
        {   
            printf("**********************************************\n");
            printf("Produtor esta produzindo %i ao buffer. \n", itens_por_producao);
            for (int i = 0; i < itens_por_producao; i++)
            {
                adiciona_ao_buffer();
            }
            int tempo = (rand() % 10) + 1;
            printf("Produtor dormindo por %i segundos. \n", tempo);
            printf("**********************************************\n\n");
            sleep(tempo);
        }
        else // Apenas dorme e espera uma outra oportunidade
        {   
            int tempo = (rand() % 10) + 1;
            printf("**********************************************\n");
            printf("Produtor dormindo por %i segundos. \n\n", tempo);
            printf("**********************************************\n\n");
            sleep(tempo);
        }
    }
}

void * consumidor()
{   
    // Ciclo de vida do consumidor, caso o total no buffer seja igual ou superio ao tanto que ele consome por vez,
    // então o consumidor consome e dorme, caso contrário ele dorme e aguarda outra oportunidade.
    while (1)
    {   
        // Condição para que ele consuma o buffer.
        if (conteudo_buffer >= consumidos_por_vez)
        {     
            printf("==============================================\n");
            printf("Consumidor consumindo %i do buffer. \n", consumidos_por_vez);
            for (int i = 0; i < consumidos_por_vez; i++)
            {
                remove_do_buffer();
            }
            int tempo = (rand() % 10) + 1;
            printf("Consumidor dormindo por %i segundos. \n", tempo);
            printf("==============================================\n\n");
            sleep(tempo);
        }
        else // Apenas dorme e espera outra oportunidade
        {
            int tempo = (rand() % 10) + 1;
            printf("==============================================\n");
            printf("Consumidor dormindo por %i segundos. \n\n", tempo);
            printf("==============================================\n\n");
            sleep(tempo);
        }
    }
    
}

int main(int argc, char **argv)
{

    tamanho_buffer = (int) atoi(argv[1]);        // Converte os parametros passados na linha de
    itens_por_producao = (int) atoi(argv[2]);   // comando para inteiro e atribui as variáveis
    consumidos_por_vez = (int) atoi(argv[3]);

    conteudo_buffer = 0; // Variável de controle usada para saber quando o buffer está vazio ou quantos elementos tem.

    // Aloca o espaço para o buffer e inicializa ele como 0 em todas as posições.
    buffer = malloc(sizeof(int) * tamanho_buffer);
    for (int i = 0; i < tamanho_buffer; i++)
    {
        buffer[i] = 0; // Inicializando o buffer todo igual a 0
    }

    // Mensagem de inicialização do programa.
    printf("\n");
    printf("Iniciando algoritmo 'Produtor Consumidor' com:\nBuffer de tamanho:%i\nItens Produzidos por vez:%i\nItens Consumidos por vez:%i\n\n",tamanho_buffer,itens_por_producao,consumidos_por_vez);
    
    // Criação das structs usadas nas threads, uma para o consumidor e outra para o produtor.
    pthread_t consumidor_t;
    pthread_t produtor_t;

    // Criação e inicialização das threads.
    pthread_create(&consumidor_t, NULL, consumidor, NULL);
    pthread_create(&produtor_t, NULL, produtor, NULL);

    // Função para não permitir que a main seja terminada antes das duas threads.
    pthread_exit(NULL);
    return 0;
}
