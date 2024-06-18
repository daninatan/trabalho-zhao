#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

bool acc_reg = false;

//FUNÇÃO PARA O INÍCIO DO APLICATIVO
void initialPage(char answer_have_acc, char conf[10]) {
    printf("===========================================\n");
    printf("              INITIAL PAGE\n");
    printf("===========================================\n\n\n");

    printf("Do you have an account?  (Y/N)  ");
    scanf("%9s", conf);
    getchar();
}

// FUNÇÃO PARA REGISTRAR USUÁRIO
void func_reg(char ruser[50], char rpass[50]) {

    char confirm_reg[10], v;

    do {
        system("cls");

        printf("===========================================\n");
        printf("              REGISTER SCREEN\n");
        printf("===========================================\n\n\n");

        printf("Enter your username:  ");
        fgets(ruser, 50, stdin);

        printf("\nEnter your password:  ");
        scanf("%50s", rpass);
        getchar();

        printf("\n\nUSER: %s\nPASS: %s", ruser, rpass);

        do{
            printf("\n\nConfirm?  (Y/N)  ");

            scanf("%9s", confirm_reg);
            getchar();

//VERIFICA SE O USUÁRIO ESTÁ RESPONDENDO CORRETAMENTE A PERGUNTA

            if (strcasecmp(confirm_reg, "y") == 0) {
                v = 'Y';
            } else if (strcasecmp(confirm_reg, "n") == 0) {
                v = 'N';
            } else {
                printf("\nPlease, enter Y or N. Press ENTER to continue.\n");
                getchar();
            }

            if (v == 'N') {
                printf("\n\nUser not defined. Press ENTER to continue.");
                getchar();
            } else {
                acc_reg = true;
            }
        }while(v != 'Y' && v!= 'N');
    } while (v != 'Y');
}

// FUNÇÃO PARA FAZER LOGIN
void func_login(char ruser[50], char rpass[50]) {
    if (!acc_reg) {
        printf("\nNo accounts registered. Press ENTER to go to the register screen.\n");
        getchar();
        func_reg(ruser, rpass);
        func_login(ruser, rpass);
        return;
    }

    system("cls");
    char user[50], pass[50];

//ENQUANTO O USUÁRIO NÃO DIGITAR A FUNÇÃO CORRETAMENTE, O LOOP NÃO IRÁ ACABAR
    while (1) {
        printf("===========================================\n");
        printf("              LOGIN SCREEN\n");
        printf("===========================================\n\n\n");
        printf("Enter your username:  ");
        fgets(user, 50, stdin);
        printf("Enter your password:  ");
        scanf("%50s", &pass);

        if (strcmp(user, ruser) == 0 && strcmp(pass, rpass) == 0) {
            printf("\nLogin seccessful!\n\n");
            printf("Press ENTER to continue...");
            getchar();
            break;
        } else {
            printf("\n\nIncorrect user/password. Press ENTER to continue.\n");
            getchar();
            getchar();
            system("cls");
        }
    }
}

//FUNÇÃO PARA CADASTRAR O NÚMERO DE ITENS
int add_items_number() {
    int num_of_items = 0;

//O LOOP OCORRE ENQUANTO O USUÁRIO NÃO INSERIR UM NÚMERO VÁLIDO PARA A QUANTIDADE DE ITENS
    do {
        printf("==========================\n");
        printf("        ADD ITEMS          \n");
        printf("==========================\n\n");
        printf("Enter the number of items you want to add: ");
        scanf("%d", &num_of_items);
        getchar();

        printf("\n%d\n\n", num_of_items);

        if (num_of_items <= 0)
            system("cls");
        printf("Error! Try inserting a positive number.\n");

    } while (num_of_items <= 0);

    system("cls");

    return num_of_items;
}

//FUNÇÃO PARA CADASTRAR OS ITENS
void add_items(int num_of_items, int item_count, char item_name[num_of_items][20], char item_availabitity_answer[num_of_items], char item_code[num_of_items][10], float item_price[num_of_items], bool availability[num_of_items], char isAvailable[num_of_items][4]) {
    int run = 1;

    printf("==========================\n");
    printf("        ADD ITEMS          \n");
    printf("==========================\n\n");

    printf("Items %d/%d\nAdd item...\n\nEnter item name: ", (item_count + 1), num_of_items);
    scanf(" %[^\n]", item_name[item_count]);

    printf("Enter item code: ");
    scanf("%s", item_code[item_count]);

    printf("Enter item price: ");
    scanf("%f", &item_price[item_count]);

//ENQUANTO O USUÁRIO NÃO RESPONDE CORRETAMENTE O LOOP CONTINUA
    while (run) {
        printf("Enter item availability [y/n]: ");
        scanf(" %c", &item_availabitity_answer[item_count]);
        if (item_availabitity_answer[item_count] == 'y' || item_availabitity_answer[item_count] == 'Y') {
            availability[item_count] = 1;
            strcpy(isAvailable[item_count], "Yes");
            run = 0;
        } else if (item_availabitity_answer[item_count] == 'n' || item_availabitity_answer[item_count] == 'N') {
            availability[item_count] = 0;
            strcpy(isAvailable[item_count], "No");
            run = 0;
        } else {
            system("cls");
            printf("Error! Try Again\n");
        }
    }

    system("cls");

//FAZ UMA CONFIRMAÇÃO DAS INFORMAÇÕES QUE FORAM ADICIONADAS PARA O ITEM
    printf("==========================\n");
    printf("        ADD ITEMS          \n");
    printf("==========================\n\n");

    printf("Items: %d/%d\n\nName: %s\n", (item_count + 1), num_of_items, item_name[item_count]);
    printf("Code: %s\nPrice: R$%.2f\n", item_code[item_count], item_price[item_count]);

    if (availability[item_count])
        printf("The item is available.\n\n");
    else
        printf("The item is not available.\n\n");

    printf("Press ENTER key to confirm your action\n");
    getchar();
    getchar();

    system("cls");
}

//APENAS UMA CONFIRMAÇÃO PARA QUANDO O ITEM FOR ADICIONADO E PARA QUANDO CHEGAR AO NÚMERO MÁXIMO DE ITENS ADICIONADOS
void confirm_add_items(int item_count, int num_of_items) {
    printf("==========================\n");
    printf("        ADD ITEMS          \n");
    printf("==========================\n\n");

    printf("Item added");

    if ((item_count + 1) == num_of_items) {
        printf("\n\nYou reached your max items.\n");
    } else{
        printf("\n\nPress ENTER key to add another item...");
        getchar();

        system("cls");

        item_count++;
    }
}

//FUNÇÃO PARA APRESENTAR OS ITENS E SUAS RESPECTIVAS INFORMAÇÕES
void items_list(int num_of_items, int item_count, char item_name[num_of_items][20], char item_code[num_of_items][10], float item_price[num_of_items], bool availability[num_of_items], char isAvailable[num_of_items][4], float medium_price) {
    printf("====================================\n");
    printf("         INSERTED ITEMS\n");
    printf("====================================\n\n");

    for(int i = 0; i < num_of_items; i++) {
        usleep(0.3 * 1000000); //FUNÇÃO PARA TER UM DELAY NA APRESENTAÇÃO DE CADA ITEM (ESTÉTICO)
        printf("%d - %s | R$ %.2f | Code: %s | Available: %s\n\n", i + 1, item_name[i], item_price[i], item_code[i], isAvailable[i]);
        medium_price += item_price[i];
    }

    medium_price /= num_of_items;

    printf("\nThe medium price is: R$ %.2f\n\n", medium_price);
}

// CORPO PRINCIPAL DO CÓDIGO
int main() {

    //DECLARAÇÃO DAS VARIÁVEIS RESPONSAVEIS PELO REGISTRO E LOGIN DO USUÁRIO
    char user[50], pass[50], answer_have_acc, conf[10], ruser[50] = "", rpass[50] = "";

    //ENQUANTO O USUÁRIO NÃO RESPONDER CORRETAMENTE A PERGUNTA, O LOOP NÃO IRÁ TERMINAR, PERGUNTANDO VÁRIAS VEZES
    while (1) {
        initialPage(answer_have_acc, conf);

        if (strcasecmp(conf, "y") == 0) {
            answer_have_acc = 'Y';
        } else if (strcasecmp(conf, "n") == 0) {
            answer_have_acc = 'N';
        } else {
            printf("\nPLEASE ENTER Y OR N. PRESS ENTER TO CONTINUE.\n");
            getchar();
            system("cls");
        }

        if (answer_have_acc == 'Y') {
            func_login(ruser, rpass);
            break;
        } else if (answer_have_acc == 'N') {
            func_reg(ruser, rpass);
            func_login(ruser, rpass);
            break;
        }
    }

    getchar();
    system("cls");

    //DECLARAÇÃO DAS VARIÁVEIS RESPONSÁVEIS PELO CONTROLE DO NÚMERO DE ITENS ADICIONADOS
    int num_of_items = 0, item_count = 0;

    //VARIÁVEL RESPONSÁVEL PELO NÚMERO DE ITENS ADICIONADOS
    num_of_items = add_items_number();

    //VARIÁVEIS RESPONSÁVEIS PELO ARMAZENAMENTO DAS INFORMAÇÕES DOS ITENS
    char item_name[num_of_items][20], item_availabitity_answer[num_of_items], item_code[num_of_items][10], isAvailable[num_of_items][4];
    float item_price[num_of_items], medium_price = 0;
    bool availability[num_of_items];

    //O FOR RODA PARA CADA ITEM QUE DESEJA SER ADICIONADO, CHAMANDO A FUNÇÃO E PASSANDO AS VARIÁVEIS
    for(int i = 0; i < num_of_items; i++) {
        add_items(num_of_items, item_count, item_name, item_availabitity_answer, item_code, item_price, availability, isAvailable);
        confirm_add_items(item_count, num_of_items);
        item_count++;
    }

    system("cls");

    //CHAMA A FUNÇÃO PARA EXIBIR O RESUMO DOS ITENS
    items_list(num_of_items, item_count, item_name, item_code, item_price, availability, isAvailable, medium_price);

    getchar();

    return 0;
}
