#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SENSORES 10
#define MAX_LEITURAS 10000

// Estrutura para guardar a leitura
typedef struct {
    long timestamp;
    float valor;
} Leitura;

// Estrutura para guardar os dados do Sensor
typedef struct {
    char nome[20];
    Leitura leituras[MAX_LEITURAS];
    int quantidade;
} Sensor;

int main() {
    Sensor sensores[MAX_SENSORES];
    int totalSensores = 0;

    // Fazendo os sensores inicialirem
    for (int i = 0; i < MAX_SENSORES; i++) {
        sensores[i].quantidade = 0;
    }

    FILE *arquivo = fopen("arquivoSensores.txt", "r");
    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo.\n");
        return 1;
    }

    long tempo;
    char nomeSensor[20];
    float valorLido;

    // Leitura do arquivo linha por linha
    while (fscanf(arquivo, "%ld %s %f", &tempo, nomeSensor, &valorLido) == 3) {
        int achei = 0;

        // Verifica se o sensor já existe
        for (int i = 0; i < totalSensores; i++) {
            if (strcmp(sensores[i].nome, nomeSensor) == 0) {
                int pos = sensores[i].quantidade;
                sensores[i].leituras[pos].timestamp = tempo;
                sensores[i].leituras[pos].valor = valorLido;
                sensores[i].quantidade++;
                achei = 1;
                break;
            }
        }

        // caso não achar, cria novo sensor
        if (!achei && totalSensores < MAX_SENSORES) {
            strcpy(sensores[totalSensores].nome, nomeSensor);
            sensores[totalSensores].leituras[0].timestamp = tempo;
            sensores[totalSensores].leituras[0].valor = valorLido;
            sensores[totalSensores].quantidade = 1;
            totalSensores++;
        }
    }

    fclose(arquivo);

    // Ordena e escreve os arquivos de saída
    for (int i = 0; i < totalSensores; i++) {
        // Ordena por timestamp 
        for (int j = 0; j < sensores[i].quantidade - 1; j++) {
            for (int k = 0; k < sensores[i].quantidade - 1 - j; k++) {
                if (sensores[i].leituras[k].timestamp > sensores[i].leituras[k+1].timestamp) {
                    Leitura temp = sensores[i].leituras[k];
                    sensores[i].leituras[k] = sensores[i].leituras[k+1];
                    sensores[i].leituras[k+1] = temp;
                }
            }
        }

        char nomeArquivo[30];
        sprintf(nomeArquivo, "%s.txt", sensores[i].nome);
        FILE *saida = fopen(nomeArquivo, "w");

        if (saida == NULL) {
            printf("Erro ao criar o arquivo %s\n", nomeArquivo);
            continue;
        }

        for (int j = 0; j < sensores[i].quantidade; j++) {
            fprintf(saida, "%ld %.2f\n", sensores[i].leituras[j].timestamp, sensores[i].leituras[j].valor);
        }

        fclose(saida);
    }

    printf("Todos os dados foram salvos em arquivos individuais.\n");
    return 0;
}
