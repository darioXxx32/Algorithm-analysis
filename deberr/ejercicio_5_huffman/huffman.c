#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TREE_NODES 50

typedef struct HuffmanNode {
    char symbol;
    double freq;
    struct HuffmanNode *left;
    struct HuffmanNode *right;
} HuffmanNode;

typedef struct {
    char symbol;
    char code[20];
} CodeEntry;

HuffmanNode* create_node(char symbol, double freq, HuffmanNode *left, HuffmanNode *right) {
    HuffmanNode *node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    node->symbol = symbol;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

void generate_codes(HuffmanNode *root, char *current_code, int depth, CodeEntry codes[], int *count) {
    if (!root) return;

    // Si es una hoja
    if (!root->left && !root->right) {
        current_code[depth] = '\0';
        codes[*count].symbol = root->symbol;
        strcpy(codes[*count].code, current_code);
        (*count)++;
        return;
    }

    if (root->left) {
        current_code[depth] = '0';
        generate_codes(root->left, current_code, depth + 1, codes, count);
    }
    if (root->right) {
        current_code[depth] = '1';
        generate_codes(root->right, current_code, depth + 1, codes, count);
    }
}

const char* find_code(CodeEntry codes[], int count, char ch) {
    for (int i = 0; i < count; i++) {
        if (codes[i].symbol == ch) {
            return codes[i].code;
        }
    }
    return "";
}

char* encode_message(const char *msg, CodeEntry codes[], int count) {
    static char encoded[500];
    encoded[0] = '\0';
    for (int i = 0; msg[i] != '\0'; i++) {
        const char *c = find_code(codes, count, msg[i]);
        strcat(encoded, c);
    }
    return encoded;
}

void decode_bitstream(HuffmanNode *root, const char *bitstream) {
    printf("Decodificando secuencia de bits: %s\n", bitstream);
    printf("Proceso paso a paso:\n");

    HuffmanNode *curr = root;
    char decoded[100];
    int dec_idx = 0;

    int bit_start = 0;
    for (int i = 0; bitstream[i] != '\0'; i++) {
        if (bitstream[i] == '0') {
            curr = curr->left;
        } else if (bitstream[i] == '1') {
            curr = curr->right;
        }

        if (!curr->left && !curr->right) {
            // Se llego a una hoja
            printf("  Bits [%d..%d]: ", bit_start, i);
            for (int k = bit_start; k <= i; k++) printf("%c", bitstream[k]);
            printf(" -> Simbolo '%c'\n", curr->symbol);

            decoded[dec_idx++] = curr->symbol;
            curr = root;
            bit_start = i + 1;
        }
    }
    decoded[dec_idx] = '\0';
    printf("------------------------------------------------------------\n");
    printf("Mensaje Decodificado Final: \"%s\"\n", decoded);
    printf("------------------------------------------------------------\n");
}

int main(void) {
    printf("============================================================\n");
    printf("         LAB 09 - EJERCICIO 5: CODIFICACION DE HUFFMAN      \n");
    printf("============================================================\n\n");

    // Datos del problema 5.1
    // symbol:    A     B     C     D     _
    // frequency: 0.4   0.1   0.2   0.15  0.15
    char symbols[] = {'A', 'B', 'C', 'D', '_'};
    double freqs[] = {0.40, 0.10, 0.20, 0.15, 0.15};
    int n = 5;

    printf("--- 5.1 CONSTRUCCION DEL ARBOL DE HUFFMAN ---\n");
    printf("Datos iniciales:\n");
    for (int i = 0; i < n; i++) {
        printf("  Simbolo '%c' -> Frecuencia: %.2f\n", symbols[i], freqs[i]);
    }
    printf("\n");


    /*
       Paso 1: Se unen B (0.10) y D (0.15) -> Subarbol BD (0.25)
       Paso 2: Se unen _ (0.15) y C (0.20) -> Subarbol _C (0.35)
       Paso 3: Se unen BD (0.25) y _C (0.35) -> Subarbol BD_C (0.60)
       Paso 4: Se unen BD_C (0.60) y A (0.40) -> Raiz (1.00)
    */
    HuffmanNode *node_B = create_node('B', 0.10, NULL, NULL);
    HuffmanNode *node_D = create_node('D', 0.15, NULL, NULL);
    HuffmanNode *node_BD = create_node('*', 0.25, node_B, node_D); // B: 000, D: 001

    HuffmanNode *node_space = create_node('_', 0.15, NULL, NULL);
    HuffmanNode *node_C = create_node('C', 0.20, NULL, NULL);
    HuffmanNode *node_spaceC = create_node('*', 0.35, node_space, node_C); // _: 010, C: 011

    HuffmanNode *node_left = create_node('*', 0.60, node_BD, node_spaceC);
    HuffmanNode *node_A = create_node('A', 0.40, NULL, NULL);
    HuffmanNode *root = create_node('*', 1.00, node_left, node_A); // A: 1

    printf("Pasos de fusion voraz (Greedy Merging):\n");
    printf("  1. Fusionar 'B' (0.10) y 'D' (0.15) -> Nodo 'BD' con frecuencia 0.25\n");
    printf("  2. Fusionar '_' (0.15) y 'C' (0.20) -> Nodo '_C' con frecuencia 0.35\n");
    printf("  3. Fusionar 'BD' (0.25) y '_C' (0.35) -> Nodo 'BD_C' con frecuencia 0.60\n");
    printf("  4. Fusionar 'BD_C' (0.60) y 'A' (0.40) -> Raiz con frecuencia 1.00\n\n");

    CodeEntry codes[10];
    int code_count = 0;
    char buffer[20];
    generate_codes(root, buffer, 0, codes, &code_count);

    printf("Tabla de Codigos de Huffman Generados:\n");
    printf("----------------------------------------\n");
    printf("Simbolo    Frecuencia    Codigo    Longitud\n");
    printf("----------------------------------------\n");
    double avg_len = 0.0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < code_count; j++) {
            if (codes[j].symbol == symbols[i]) {
                int len = strlen(codes[j].code);
                printf("   %-8c %-13.2f %-9s %d bits\n", symbols[i], freqs[i], codes[j].code, len);
                avg_len += freqs[i] * len;
            }
        }
    }
    printf("----------------------------------------\n");
    printf("Longitud Media Ponderada: %.2f bits/simbolo\n", avg_len);
    printf("Longitud con codigo fijo (3 bits): 3.00 bits/simbolo\n");
    printf("Ahorro de compresion: %.2f%%\n\n", (1.0 - avg_len / 3.0) * 100.0);

    // ---------------------------------------------------------
    // 5.2 CODIFICACION DE ABACABAD
    // ---------------------------------------------------------
    printf("--- 5.2 CODIFICACION DE LA CADENA \"ABACABAD\" ---\n");
    const char *text_to_encode = "ABACABAD";
    printf("Cadena a codificar: %s\n", text_to_encode);
    printf("Detalle por caracter:\n");
    for (int i = 0; text_to_encode[i] != '\0'; i++) {
        printf("  '%c' -> %s\n", text_to_encode[i], find_code(codes, code_count, text_to_encode[i]));
    }
    char *encoded_result = encode_message(text_to_encode, codes, code_count);
    printf("------------------------------------------------------------\n");
    printf("Cadena binaria resultante: %s\n", encoded_result);
    printf("Longitud: %zu bits (con codigo fijo de 3 bits serian %d bits)\n", 
           strlen(encoded_result), (int)strlen(text_to_encode) * 3);
    printf("------------------------------------------------------------\n\n");

    // ---------------------------------------------------------
    // 5.3 DECODIFICACION DE 100010111001010
    // ---------------------------------------------------------
    printf("--- 5.3 DECODIFICACION DE LA SECUENCIA \"100010111001010\" ---\n");
    const char *stream_to_decode = "100010111001010";
    decode_bitstream(root, stream_to_decode);

    return 0;
}
