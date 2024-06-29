/*Grupo:
Daniel Natan dos Santos Brito - 15446902
Gabriel Hupfer Righi - 15508612
Guilherme Borges de Pádua Barbosa - 15653045
Pedro Pacini de Resende - 15484415
Mikhael Lazarev Nogueira Barbosa - 15480495
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


//STRUCTS**********************************************************************

typedef struct _usuario{
    char username[100];
    char senha[100];
    int id;
} Usuario;


typedef struct _jogo{
    char nome[100];
    float preco;
    float tamanho; //em GBs
    char genero[100]; //fantasia terror etc.
    bool disponivel;
    int id;   //disponibilidade
}Jogo;

//DECLARAÇÃO DAS FUNÇÕES
void pausar(char *texto);
void escreverUsuariosArquivo(Usuario *users, FILE *arqUsuarios, int num);
void pegarUsuariosArquivo(Usuario **users, int *numUsuarios, FILE *arqUsers, int*id);
int chamaTelaInicial(FILE *arqUsers, Usuario *users, int *numUsuariosTotal, int*id);
void chamaTelaCadastro(int num,Usuario **users, FILE *arqUsuarios, int *id);
bool chamaTelaLogin(int numUsuariosTotal, Usuario *users, int *idLogado);
int chamaTelaUsuarios(Usuario *users, int numUsuariosTotal);
void editarOuExcluirUsuario(FILE *arqUsers, int *numUsuariosTotal, Usuario *users, int*id);
void adicionarJogo(Jogo *jogos, int  *i, int idLogado);
void verJogos(Jogo *jogos, int *index);
void editarJogo(Jogo *jogos, int indexJogo, int idLogado);
void editarJogosArquivo(Jogo *jogos, int indexAlvo);
void editarJogosArquivo(Jogo *jogos, int indexAlvo);
void escreverJogosArquivo(Jogo *jogos, int indexJogo);
void pegarJogosArquivo(Jogo *jogos, int *indexJogo, FILE *arq);
void verJogosUsuario(Jogo *jogos, int *index, int idLogado);


//FUNÇÕES**********************************************************************


void pausar(char *texto) {
    if (strlen(texto) > 0) {
        printf("%s", texto);     //Função para pausar o programa em certo ponto, para que o usuario possa tomar uma ação.
    }
    getchar();
}

int chamaTelaInicial(FILE *arqUsers, Usuario *users, int *numUsuariosTotal, int *id){
    bool run = true;
    char destino[100];
    int querEditarOuExcluir, *idLogado;
    idLogado = (int*)malloc(sizeof(int));
    Usuario **ponteiroParaOVetorDeUsers;
    ponteiroParaOVetorDeUsers = &users;

    while(run){ // variavel run serve para checar quando o programa deve continuar rodando nessa tela (será false quando o usuario fizer login ou quando sair do programa)

        system("cls || clear");
        printf("------------------------------------------------\n");
        printf("------------------------------------------------\n");
        printf("                  TELA INICIAL                  \n");
        printf("------------------------------------------------\n");
        printf("----------------- USPGames v1.0 ----------------\n\n");
        printf("  Login                         (1)\n");
        printf("  Registrar novo usuario        (2)\n");
        printf("  Gerenciar usuarios            (3)\n");
        printf("  Sair                          (0)");
        printf("\n\nEscolha o que gostaria de fazer: ");

        fgets(destino,100,stdin);
        if(destino[0]=='0') exit(0);

        if (destino[1] != '\n' || (destino[0] < '0' || destino[0] > '3')) {
                printf("\n--------------------------------------------------------");
                printf("\n\nPor favor, entre apenas com os inteiros: 0, 1, 2, ou 3.");
                printf("\n\n--------------------------------------------------------");
                pausar("\n\nPressione ENTER para voltar");
                system("cls || clear");
                destino[0] = '.';
        }

        switch(destino[0]){
                case '1':
                    run = chamaTelaLogin(*numUsuariosTotal, users, idLogado); // caso o login for efetuado com sucesso, chamaTelaLogin() irá retornar false, e ira parar o loop
                    break;
                case '2':
                    chamaTelaCadastro(*numUsuariosTotal, ponteiroParaOVetorDeUsers, arqUsers, id);
                    (*numUsuariosTotal)++; //o numero de usuarios cadastrados ira aumentar cada vez que a função for chamada
                    break;
                case '3':
                    querEditarOuExcluir = chamaTelaUsuarios(users, *numUsuariosTotal); // a função irá retornar false ou true, dependendo se o usuario quiser manipular os usuarios ou apenas visualizá-los
                    if(querEditarOuExcluir) editarOuExcluirUsuario(arqUsers, numUsuariosTotal, users, id);
                    break;
                case '0':
                    fclose(arqUsers);
                    run = false;
                    exit(0);
                    break;
            }

        if(destino[1]!='\n'||(destino[0]<'0'||destino[0]>'3')) run = true; //checa se o usuário está interagindo corretamente com o programa
    }
    return *idLogado; //idLogado ira retornar para que os jogos pertencentes ao usuario possa ser identificado, independente se o usuario editou o seu username
    free(idLogado); //liberando memoria alocada para a variavel
}

void chamaTelaCadastro(int num, Usuario **users, FILE *arqUsuarios, int *id){
    if(num > 0) *users = (Usuario *)realloc(*users, (num+1) * sizeof(Usuario));//adiciona um pois num começa no zero, entao para realocar mais memoria precisa começar no 1, pois na primeira vez ainda nao vai ter nenhum usuario
    char aux[10]; //variavel auxiliar para checar as ações do usuario
    bool cadastroValido = true; //variavel para checar se o usuario concluiu o cadastro ou nao

    do{
        cadastroValido = true;

        system("cls || clear");
        printf("------------------------------------------------\n");
        printf("------------------------------------------------\n");
        printf("                 TELA DE REGISTRO                \n");
        printf("------------------------------------------------\n");
         printf("---------------- USPGames v1.0 ----------------");

        char usernameAux[100], senhaAux[100];

        printf("\n\n\nUsername: \n");
        fgets(usernameAux,100,stdin);
        usernameAux[strcspn(usernameAux,"\n")]='\0';

        for(int i = 0; i < num; i++){
            if(strcmp(usernameAux, (*users)[i].username) == 0){
                printf("\n\nUsuario ja existe...Tente novamente"); //checa se ja existe algum usuario cadastrado com o mesmo nome, caso sim, retorna que o usuario ja existe
                cadastroValido = false;
                break;
            }else cadastroValido = true;
        }

        if(cadastroValido){
            printf("\nSenha: \n");
            fgets(senhaAux,100,stdin);
            senhaAux[strcspn(senhaAux,"\n")]='\0';

            printf("\n\nConfirmar (1)\nTrocar usuario ou senha (0)\n");
            fgets(aux,10,stdin);                                         //confirma se o usuario gostaria de continuar ou colocar outro username/senha para sua conta
            aux[strcspn(aux,"\n")]='\0';

            if(aux[0]=='1'){
                cadastroValido=true;
                strcpy((*users)[num].username, usernameAux);
                strcpy((*users)[num].senha, senhaAux);

            }else cadastroValido = false;

            if(aux[0]!='1'&&aux[0]!='0'){
                printf("Resposta invalida, tente novamente");
            }
        }

        pausar("\n\nPressione ENTER para continuar...");

    }while(cadastroValido==false);

    (*id)++; //para cada usuario cadastrado o id aumenta um, ou seja, nunca tera dois usuarios com o mesmo id
    (*users)[num].id = *id;
    escreverUsuariosArquivo(*users, arqUsuarios, num); //chama a função para escrever esse usuario cadastrado no arquivo do programa
}



bool chamaTelaLogin(int numUsuariosTotal, Usuario *users, int *idLogado){
    int numTentativasLogin=0;
    char usernameLogin[100], senhaLogin[100];
    bool verificaLogin=false; //variavel para checar se o login foi efetuado com sucesso ou n o



    do{
        verificaLogin = false;
        system("cls || clear");

        printf("------------------------------------------------\n");
        printf("------------------------------------------------\n");
        printf("                  TELA DE LOGIN                  \n");
        printf("------------------------------------------------\n");
        printf("----------------- USPGames v1.0 ----------------");

        if(numUsuariosTotal==0){
            printf("\n\nAinda nao existe usuarios. Por favor, registre uma conta primeiro.");
            pausar("\n\nPressione ENTER para continuar...");                                    //caso o usuario tente logar sem nenhum usuario cadastrado, essa mensagem sera exibida
            return true;
        }

        printf("\n\n\nNome do usuario: \n");
        fgets(usernameLogin,100,stdin);
        usernameLogin[strcspn(usernameLogin,"\n")]='\0';
        printf("\nSenha: \n");
        fgets(senhaLogin,100,stdin);
        senhaLogin[strcspn(senhaLogin,"\n")]='\0';

        for(int j=0;j<numUsuariosTotal;j++){
            if(strcmp(users[j].username,usernameLogin)== 0 && strcmp(users[j].senha,senhaLogin)==0){   //checa se o username e a senha correspondem a algum usuario no banco de dados
                verificaLogin = true;
                *idLogado = users[j].id; //passa o id do usuario logado para a variavel, para que possa identificar qual usuario esta logando no momento
                break;
            }
        }

        if(!verificaLogin){
            printf("\nUsuario ou senha incorretos, tente novamente...\n\n");  //caso nenhum dos usuarios e senhas se correspondem, essa mensagem sera exibida
            printf("\nTentativa de login (%d/3)\n",numTentativasLogin+1);
            if(numTentativasLogin==2){
                printf("\n\nNumero maximo de tentativas de login alcancado!\n");
                pausar("\n\nPressione ENTER para continuar...");
                return true;
            }
            pausar("\n\nPressione ENTER para continuar...");
            numTentativasLogin++;

        }

    }while(verificaLogin==false);

    printf("\nBem vindo!"); //mensagem para que o usuario saiba que seu login foi efetuado com sucesso
    pausar("\n\nPressione ENTER para continuar...");

    return false;
}

void pegarUsuariosArquivo(Usuario **users, int *numUsuarios, FILE *arqUsers, int *id){
    char dados[100];
    int idMax=0;

    while(fgets(dados, 100, arqUsers) != NULL){
        sscanf(dados, "%[^&]&%[^&]&%d", (*users)[*numUsuarios].username, (*users)[*numUsuarios].senha, &(*users)[*numUsuarios].id);
        if((*users)[*numUsuarios].id > idMax){
            idMax = (*users)[*numUsuarios].id;
        }
        //sscanf serve para ler e extrair os dados de cada linha do arquivo(string pega pelo fgets), no qual guardamos as informações da seguinte maneira: NOME&SENHA&ID
        //ou seja, a funcão irá ler até o primeiro "&", no qual será o nome, depois, ira ler um "&". Assim sucessivamente, até ler o "%d", que é o id do usuario
        *users=(Usuario *)realloc(*users,(*numUsuarios+1)*sizeof(Usuario)); //realocamos o tamanho do vetor para cada usuario lido no arquivo
        (*numUsuarios)++; //aumentamos o numero de usuarios

    }
    *id=idMax;  //aumenta o ID, para que nenhum usuario tenha o mesmo id
    fclose(arqUsers);

}

void escreverUsuariosArquivo(Usuario *users, FILE *arqUsuarios, int num){
    arqUsuarios = fopen("usuarios.txt", "a+");
    fprintf(arqUsuarios, "%s&%s&%d\n", users[num].username, users[num].senha, users[num].id); //escreve os dados no arquivo da seguinte maneira: NOME&SENHA&ID
    fclose(arqUsuarios);
}

int chamaTelaUsuarios(Usuario *users, int numUsuariosTotal){
    int querEditarOuExcluirAux=0;
    char querEditarOuExcluir[10];
    do{
        system("cls || clear");

        printf("------------------------------------------------\n");
        printf("------------------------------------------------\n");
        printf("                    USUARIOS                    \n");
        printf("------------------------------------------------\n");
        printf("----------------- USPGames v1.0 ----------------\n\n");

        if(numUsuariosTotal > 0){
            for(int i = 0; i < numUsuariosTotal; i++){
                printf("----------------------------------\n");
                printf("%d  -  %s | ID: %d\n", i+1, users[i].username, users[i].id);
                printf("----------------------------------\n");
            }
        }else{
            printf("Ainda nao existe nenhum usuario cadastrado");
            pausar("\n\nPressione ENTER para voltar...");          //caso nao exista usuario cadastrado, exibe essa mensagem
            return 0;
        }


        printf("\n\nPara editar ou excluir algum usuario (1) Para voltar (0): ");  //usuario decide se quer voltar ou editar/excluir algum usuario
        fgets(querEditarOuExcluir,10,stdin);

        if(querEditarOuExcluir[1] != '\n' || (querEditarOuExcluir[0] != '0' && querEditarOuExcluir[0] != '1')){
            printf("\n\nPor favor, entre apenas com um dos inteiros: 0 ou 1.\n");
            pausar("\n\nPressione ENTER para continuar");
        }

    }while((querEditarOuExcluir[1] != '\n' || (querEditarOuExcluir[0] != '0' && querEditarOuExcluir[0] != '1')));

    if(querEditarOuExcluir[0] == '0') querEditarOuExcluirAux=0;
    if(querEditarOuExcluir[0] == '1') querEditarOuExcluirAux=1;
    pausar("\n\nPressione ENTER para continuar");

    return querEditarOuExcluirAux; //ir  retornar a sua decis o
}

void editarOuExcluirUsuario(FILE *arqUsers, int *numUsuariosTotal, Usuario *users, int*id){
    FILE *arqTemp; // arquivo temporario para ajudar a manipular o arquivo original
    system("cls || clear");
    printf("------------------------------------------------\n");
    printf("------------------------------------------------\n");
    printf("                    USUARIOS                    \n");
    printf("------------------------------------------------\n");
    printf("----------------- USPGames v1.0 ----------------\n\n");

    for(int i = 0; i < *numUsuariosTotal; i++){
        printf("----------------------------------\n");
        printf("%d  -  %s\n", i+1, users[i].username);
        printf("----------------------------------\n");
    }

    int indexUsuario;
    printf("\n\nDiga qual usuario gostaria de editar ou excluir (Insira o numero correspondente ao usuario): ");
    scanf("%d", &indexUsuario);
    fflush(stdin);

    while(indexUsuario < 0 || indexUsuario > *numUsuariosTotal){
        printf("\n\nUsuario invalido, tente novamente:");          //checa se o escolheu um usuario existente
        scanf("%d", &indexUsuario);
        fflush(stdin);
    }

    int escolha;
    printf("\n\nPara editar (1) Para excluir (2): ");
    scanf("%d", &escolha);

    while(escolha != 1 && escolha != 2){
        printf("\n\nPor favor, digite um valor valido: ");   //checa se fez uma das escolhas possiveis
        scanf("%d", &escolha);
    }

    fflush(stdin);

    //caso queira editar, isso ocorrerá
    if(escolha == 1){
        char aux[100], usernameAux[100], senhaAux[100]; //variaveis para auxiliar na manipulação dos dados

        system("clear || cls");

        bool cadastroValido = false; //checa se o usuario pode editar

        while(!cadastroValido){

            printf("Digite o novo Username: \n");
            fgets(usernameAux,100,stdin);
            usernameAux[strcspn(usernameAux,"\n")]='\0';

            for(int i = 0; i < *numUsuariosTotal; i++){
                if(strcmp(usernameAux, users[i].username) == 0 && strcmp(usernameAux, users[indexUsuario - 1].username) != 0){
                    printf("\n\nUsuario ja existe...Tente novamente\n\n");                                                      //checa se ja existe algum usuario com o mesmo username
                    cadastroValido = false;
                    break;
                }else cadastroValido = true;
            }

            if(cadastroValido){
                printf("\nDigite a nova Senha: \n");
                fgets(senhaAux,100,stdin);
                senhaAux[strcspn(senhaAux,"\n")]='\0';

                printf("\n\nConfirmar (1)\nTrocar usuario ou senha (0)\n");
                fgets(aux,10,stdin);
                aux[strcspn(aux,"\n")]='\0';

                if(aux[0]=='1'){
                    cadastroValido=true;
                    strcpy(users[indexUsuario-1].username, usernameAux);   //caso tudo ocorra corretamente, os valores de ediçao sao passados para a struct
                    strcpy(users[indexUsuario-1].senha, senhaAux);

                }else cadastroValido = false;

                if(aux[0]!='1'&&aux[0]!='0'){
                    printf("\nResposta invalida, tente novamente\n\n");
                }
            }
        }

        arqUsers = fopen("usuarios.txt", "w"); //o arquivo irá ser reinicializado, por conta do "w"
        for(int i = 0; i < *numUsuariosTotal; i++){
            fprintf(arqUsers, "%s&%s&%d\n", users[i].username, users[i].senha, users[i].id = i+1); //os novos valores sao passados novamente para o arquivo
        }
        fclose(arqUsers);
    }else{ //caso o usuario decida excluir, isso ocorrerá
        arqTemp = fopen("arqTemp.txt", "w");
        arqUsers = fopen("usuarios.txt", "r");

        char string[100];
        int linhaArquivo = 0;

        while(fgets(string, 100, arqUsers) != NULL){    //essa função basicamente passa os dados para um arquivo temporario, menos os dados do usuario que quer ser removido;
                                                        //logo após isso, reinicializa o arquivo original e passa os dados novamente para ele, que no caso, nao contém o usuario a ser removido
            linhaArquivo++;
            if(indexUsuario != linhaArquivo){
                fputs(string, arqTemp);
            }
        }

        fclose(arqTemp);
        fclose(arqUsers);
        arqUsers = fopen("usuarios.txt", "w");
        arqTemp = fopen("arqTemp.txt", "r");

        while(fgets(string, 100, arqTemp) != NULL){
            fputs(string, arqUsers);
        }

        fclose(arqUsers);
        fclose(arqTemp);
        remove("arqTemp.txt");
        *numUsuariosTotal = 0; //reseta o numero de usuarios
        users=(Usuario *)realloc(users, 1*sizeof(Usuario)); //realoca o vetor de structs para o tamanho 1 novamente
        arqUsers = fopen("usuarios.txt", "a+");
        pegarUsuariosArquivo(&users, numUsuariosTotal, arqUsers, id); //chama a funçao para pegar os usuarios novamente do arquivo, para atualizar os dados do programa e remover o usuario do vetor
    }

}


void adicionarJogo(Jogo *jogos, int  *i, int idLogado){
    char _bool; //variavel temporaria para bool
    getchar();
    system("cls || clear");
    Jogo _jogos; //variavel temporaria

    int e=0;
    while(e==0){
        printf("Nome do Jogo: ");       //checar se já existe um jogo com mesmo nome
        scanf("%[^\n]", _jogos.nome);
        getchar();
        e=1;
        for(int j=0; j<*i; j++){
            if(strcmp(_jogos.nome, jogos[j].nome)==0){
              //  system("cls || clear");
                printf("Ja existe um jogo com esse nome\n");
                e=0;
                break;
            }
        }
    }

    printf("O jogo e gratuito?: (s/n)   ");
    char verificador;
    scanf("%c", &verificador);
    getchar();

    if(verificador != 'S' && verificador != 's'){
        do {
        printf("Qual o valor do jogo? (em R$): ");
        _jogos.preco = 0;
        if(scanf("%f", &(_jogos.preco)) != 1 || _jogos.preco <= 0){
            printf("\nValor invalido, insira um numero maior que zero.\n\n");
            while (getchar() != '\n');
        } else {
            getchar();
            }
        } while (_jogos.preco <= 0);
    } else {
        _jogos.preco = 0;
    }

    do{
    printf("Qual sera o tamanho do jogo?(em GBs): ");
    _jogos.tamanho = 0;
    if(scanf("%f",&(_jogos.tamanho)) != 1 || _jogos.tamanho <= 0){
        printf("\nValor invalido, insira um numero maior que zero.\n\n");
        while(getchar() != '\n');
    } else {
        getchar();
        }
    } while (_jogos.tamanho <= 0);

    printf("Qual sera o genero do jogo?: ");
    scanf("%[^\n]", _jogos.genero);
    getchar();


    printf("O jogo ja foi lancado?(s/n) ");

    if(scanf("%c", &_bool) && (_bool == 's' || _bool == 'S'))
        _jogos.disponivel = true;

    else
        _jogos.disponivel = false;

    _jogos.id = idLogado;
    jogos[*i] = _jogos; //adiciona ao vetor de indice i (indexJogo) as info
}

void verJogos(Jogo *jogos, int *index){ //essa função mostra todos os jogos de todos os usuários

    printf("------------------------------------------------\n");
    printf("------------------------------------------------\n");
    printf("                JOGOS NO CATALOGO               \n");
    printf("------------------------------------------------\n");
    printf("----------------- USPGames v1.0 ----------------\n\n");

    for(int i = 0; i < *index; i++){
        printf("Jogo %d/%d:\n\n", i+1, *index);
        printf("Nome do Jogo: %s\n", jogos[i].nome);
        printf("Preco: %.2fR$\n", jogos[i].preco);
        printf("Armazenamento necessario: %.2fGBs\n", jogos[i].tamanho);
        printf("Genero do Jogo: %s\n", jogos[i].genero);

        if(jogos[i].disponivel)
            printf("O jogo esta disponivel para a compra!\n\n\n");

        else
            printf("O jogo ainda nao foi lancado e nao esta disponivel para a compra.\n\n\n");
    }

    pausar("Pressione ENTER para voltar ao menu");
}

void editarJogo(Jogo *jogos, int indexJogo, int idLogado){
    system("cls || clear");
    int quantJogosUsuario = 0;

    for(int i = 0; i < indexJogo; i++){
        if(jogos[i].id == idLogado){      //Verifica quantos jogos o usuario possui adicionado, caso nenhum, retorna para o menu.
            quantJogosUsuario++;
        }
    }

    if(quantJogosUsuario == 0){
        pausar("Usuario nao possui nenhum jogo... Pressione ENTER para voltar ao menu.");
        return;
    }

    char buscaJogo[100];
    int indexAlvo=-1, resposta=-1, deOutraPessoa = 0;

    while(indexAlvo==-1 || deOutraPessoa == 1){
        system("cls || clear");
        indexAlvo = -1;
        deOutraPessoa = 0;
        printf("Insira o nome do jogo que deseja editar: ");
        scanf("%[^\n]", buscaJogo);
        getchar();

        for(int i=0; i<indexJogo; i++){
            if(strcmp(jogos[i].nome, buscaJogo)==0){
                    if(jogos[i].id != idLogado) deOutraPessoa = 1;       //procura o jogo que o usuario gostaria de editar
                indexAlvo=i;
                break;
            }
        }
        if(indexAlvo==-1){
            system("clear || cls");
            pausar("Esse jogo nao existe... Pressione ENTER para voltar\n"); //essa mensagem é exibida caso nao exista esse jogo
        }
        if(deOutraPessoa){
            system("clear || cls");
            pausar("Esse jogo pertence a outro usuario... Pressione ENTER para voltar\n");  //essa mensagem é exibida caso o jogo exista mas nao pertence ao usuario logado
        }
    }

    while(resposta != 0){
        system("clear || cls");
        printf("------------------------------------------------\n");
        printf("------------------------------------------------\n");
        printf("                 MENU DE EDICAO                \n");
        printf("------------------------------------------------\n");
        printf("----------------- USPGames v1.0 ----------------\n\n");
        printf("Escolha o que deseja editar\n");
        printf("1 - Nome: %s\n", jogos[indexAlvo].nome);
        printf("2 - Preco: %.2fR$\n", jogos[indexAlvo].preco);
        printf("3 - Armazenamento necessario: %.2fGBs\n", jogos[indexAlvo].tamanho);
        printf("4 - Genero: %s\n", jogos[indexAlvo].genero);
        if(jogos[indexAlvo].disponivel)
            printf("5 - Disponibilidade: disponivel\n\n");

        else
            printf("5 - Disponibilidade: nao disponivel\n\n");

        printf("0 - Fim da edicao\n");

        scanf("%d", &resposta);
        getchar();

        int e=0;    //condição para o while em case 1
        char nome[100];
        switch(resposta){
        case 1:
            while(e==0){    //condicao para checar se já existe um jogo com mesmo nome
                printf("Novo nome: ");
                scanf("%[^\n]", nome);
                getchar();
                e=1;
                for(int j=0; j<indexJogo; j++){
                    if(strcmp(nome, jogos[j].nome)==0){
                      //  system("cls || clear");
                        printf("Ja existe um jogo com esse nome\n");
                        e=0;
                        break;
                    }
                }
            }
            strcpy(jogos[indexAlvo].nome, nome);
            pausar("\nNome atualizado. Pressione ENTER para voltar ao menu de edicao.");

            break;
        case 2:
            printf("O jogo sera gratuito?: (s/n)   ");
            char verificador;
            scanf("%c", &verificador);
            getchar();

            if(verificador != 'S' && verificador != 's'){
                do {
                printf("Qual o novo valor do jogo? (em R$): ");
                jogos[indexAlvo].preco = 0;
                if(scanf("%f", &(jogos[indexAlvo].preco)) != 1 || jogos[indexAlvo].preco <= 0){
                    printf("\nValor invalido, insira um numero maior que zero.\n\n");
                    while (getchar() != '\n');
                } else {
                    getchar();
                    }
                } while (jogos[indexAlvo].preco <= 0);
            } else {
                jogos[indexAlvo].preco = 0;
            }
            fflush(stdin);
            pausar("\nPreco atualizado. Pressione ENTER para voltar ao menu de edicao.");

            break;
        case 3:
            do{
            printf("Qual sera o novo tamanho do jogo?(em GBs): ");
            jogos[indexAlvo].tamanho = 0;
            if(scanf("%f",&(jogos[indexAlvo].tamanho)) != 1 || jogos[indexAlvo].tamanho <= 0){
                printf("\nValor invalido, insira um numero maior que zero.\n\n");
                while(getchar() != '\n');
            } else {
                getchar();
                }
            } while (jogos[indexAlvo].tamanho <= 0);
                                         //atualiza os dados do jogo, conforme a vontade do usuario
            fflush(stdin);
            pausar("\nTamanho atualizado. Pressione ENTER para voltar ao menu de edicao.");

            break;
        case 4:
            printf("Novo genero?: ");
            scanf("%[^\n]", jogos[indexAlvo].genero);
            getchar();
            pausar("\nGenero atualizado. Pressione ENTER para voltar ao menu de edicao.");

            break;
        case 5:
            if(jogos[indexAlvo].disponivel)
                jogos[indexAlvo].disponivel = false;
            else
                jogos[indexAlvo].disponivel = true;

            pausar("\nDisponibilidade atualizada. Pressione ENTER para voltar ao menu de edicao.");
            break;
        }

    }
    editarJogosArquivo(jogos, indexAlvo); //chama a funçao para atualizar os jogos no arquivo
    pausar("\nEdicao finalizada. Pressione ENTER para voltar ao menu de edicao.");
}

void editarJogosArquivo(Jogo *jogos, int indexAlvo){
    FILE *arq, *temp;
    char dados[300];
    int count=0;

    if((arq = fopen("jogos.txt", "r"))==NULL){
        printf("Erro ao acessar dados dos produtos. Fim do programa.");
        exit(1);
    }

    if((temp=fopen("temp.txt", "w"))==NULL){
        printf("Erro ao editar dados dos produtos");
        exit(1);
    }

    while((fgets(dados, 300, arq))!=NULL)   //passa os dados editados para o temporário
    {
        if(count!=indexAlvo){
            fputs(dados, temp);
        }
        else{
            fprintf(temp, "%s&%s&%.2f&%.2f&%d&%d\n", jogos[indexAlvo].nome, jogos[indexAlvo].genero, jogos[indexAlvo].preco, jogos[indexAlvo].tamanho, jogos[indexAlvo].disponivel, jogos[indexAlvo].id);
        }
        count++;
    }

    fclose(temp);
    fclose(arq);

    if((arq = fopen("jogos.txt", "w"))==NULL){
        printf("Erro ao acessar dados dos produtos. Fim do programa.");
        exit(1);
    }

    if((temp=fopen("temp.txt", "r"))==NULL){
        printf("Erro ao editar dados dos produtos");
        exit(1);
    }

    while((fgets(dados, 300, temp))!=NULL)   //retorna ao arquivo original
    {
            fputs(dados, arq);
    }

    fclose(temp);
    remove("temp.txt");
    fclose(arq);
}

void TelaJogos(Jogo *jogos, int indexJogo, int idLogado){
   int resposta = 1;
    int numeroDeJogos = 0, *idLogadoP;
    idLogadoP = (int*)malloc(sizeof(int));
    *idLogadoP = idLogado;

    while (resposta != 0){
    system("cls || clear");
    printf("------------------------------------------------\n");
    printf("------------------------------------------------\n");
    printf("                 MENU DO USUARIO                 \n");        //menu para o usuario escolher suas ações enquanto usa o programa
    printf("------------------------------------------------\n");
    printf("----------------- USPGames v1.0 ----------------\n\n");
    printf("  Adicionar jogos ao catalogo   (1)\n");
    printf("  Ver jogos no catalogo         (2)\n");
    printf("  Ver jogos do usuario          (3)\n");
    printf("  Editar um jogo no catalogo    (4)\n");
    printf("  Sair                          (0)");
    printf("\n\nEscolha o que gostaria de fazer: ");

        if (scanf("%d", &resposta) != 1) {
            printf("\n--------------------------------------------------------");
            printf("\n\nPor favor, entre apenas com números inteiros: 0, 1, 2, 3 ou 4.");
            printf("\n\n--------------------------------------------------------");
            int verificador;
            while ((verificador = getchar()) != '\n');
            pausar("\n\nPressione ENTER para voltar");
            continue;
        }

        fflush(stdin);

        switch(resposta){
        case 1:

            system("clear || cls");
            printf("Quantos jogos voce gostaria de adicionar?\n");
            scanf("%d", &numeroDeJogos);

            for(int i = 0; i < numeroDeJogos; i++){
                adicionarJogo(jogos, &indexJogo, *idLogadoP);
                escreverJogosArquivo(jogos, indexJogo);
                indexJogo++;
            }
            break;
        case 2:

            system("clear || cls");
            if(indexJogo==0){
                pausar("Ainda nao ha nenhum jogo adicionado. \n\nPressione Enter para voltar ao menu.\n"); //caso queira ver os jogos e nao exista nenhum, essa mensagem é exibida

            }
            else{
                verJogos(jogos, &indexJogo);
            }
            break;
        case 3:

            verJogosUsuario(jogos, &indexJogo, *idLogadoP);
            break;
        case 4:

            editarJogo(jogos, indexJogo, *idLogadoP);
            break;
        case 0:

            printf("\nFim do programa.\n");
            exit(0);
            break;
        default:
            printf("\n--------------------------------------------------------");
            printf("\n\nPor favor, entre apenas com os inteiros: 0, 1, 2, 3 ou 4.");
            printf("\n\n--------------------------------------------------------");
            pausar("\n\nPressione ENTER para voltar");
            break;
    }
    }
}

void escreverJogosArquivo(Jogo *jogos, int indexJogo){
    FILE *arq;
    if((arq = fopen("jogos.txt", "a"))==NULL){
        printf("Erro ao acessar dados dos produtos. Fim do programa.");
        exit(1);
    }
    fprintf(arq, "%s&%s&%.2f&%.2f&%d&%d\n", jogos[indexJogo].nome, jogos[indexJogo].genero, jogos[indexJogo].preco, jogos[indexJogo].tamanho, jogos[indexJogo].disponivel, jogos[indexJogo].id);
    //escreve os jogos da seguinte maneira no arquivo: NOME&GENERO&PREÇO&TAMANHO&DISPONIBILIDADE&IDdoUsuarioQueAdicionou
    fclose(arq);
}

void pegarJogosArquivo(Jogo *jogos, int *indexJogo, FILE *arq){

    char dados[300];

    if((arq = fopen("jogos.txt", "a+"))==NULL){
        printf("Erro ao acessar dados dos produtos. Fim do programa.");
        exit(1);
    }

    while(fgets(dados, 300, arq) != NULL){
        char nome[100];
        float preco;
        float tamanho; //em GBs
        char genero[100]; //fantasia terror etc.
        int disponivel;
        int id;   //disponibilidade

        sscanf(dados, "%[^&]&%[^&]&%f&%f&%d&%d", nome, genero, &preco, &tamanho, &disponivel, &id); //pega os jogos do arquivo e passa para as tructs no programa
        printf("teste > %s", dados);

        strcpy(jogos[*indexJogo].nome, nome);
        strcpy(jogos[*indexJogo].genero, genero);
        jogos[*indexJogo].preco = preco;
        jogos[*indexJogo].tamanho = tamanho;
        jogos[*indexJogo].disponivel = disponivel;
        jogos[*indexJogo].id = id;

        (*indexJogo)++;

    }

    fclose(arq);
}

void verJogosUsuario(Jogo *jogos, int *index, int idLogado){ //função para ver apenas os jogos do usuario
    system("clear || cls");
    printf("------------------------------------------------\n");
    printf("------------------------------------------------\n");
    printf("                JOGOS DO USUARIO               \n");
    printf("------------------------------------------------\n");
    printf("----------------- USPGames v1.0 ----------------\n\n");

    int quantJogosUsuario = 0;

    for(int i = 0; i < *index; i++){
        if(jogos[i].id == idLogado){ //checa a partir do id quantos jogos o usuario tem
            quantJogosUsuario++;
        }
    }

    int indice = 0;
    for(int i = 0; i < *index; i++){
        if(jogos[i].id == idLogado){ //caso o id do jogo corresponda ao id do usuario, o programa mostra esse jogo
            indice++;
            printf("Jogo %d/%d:\n\n", indice, quantJogosUsuario);
            printf("Nome do Jogo: %s\n", jogos[i].nome);
            printf("Preco: %.2fR$\n", jogos[i].preco);
            printf("Armazenamento necessario: %.2fGBs\n", jogos[i].tamanho);
            printf("Genero do Jogo: %s\n", jogos[i].genero);

            if(jogos[i].disponivel)
                printf("O jogo esta disponivel para a compra!\n\n\n");

            else
                printf("O jogo ainda nao foi lancado e nao esta disponivel para a compra.\n\n\n");
        }
    }

    if(quantJogosUsuario == 0){
        pausar("Usuario ainda nao possui nenhum jogo cadastrado...Pressione ENTER para voltar ao menu");
    }else pausar("Pressione ENTER para voltar ao menu");

}

//MAIN*************************************************************************


int main(){
    FILE *arqUsers; //declara a variavel para manipulaçao de arquivo dos usuarios
    FILE *arqJogos; //abre o arquivo para manipulaçao dos jogos

    arqUsers = fopen("usuarios.txt", "a+");

    int numUsuariosTotal=0, idUsuario = 0, idLogado;
    Usuario *users, **ponteiroParaOVetorDeUsers; //cria um vetor de usuarios e um ponteiro para o vetor de usuarios
    users=(Usuario *)malloc(sizeof(Usuario)); //inicializa o vetor como um
    ponteiroParaOVetorDeUsers = &users; //passa o endereço do vetor para o ponteiro

    pegarUsuariosArquivo(ponteiroParaOVetorDeUsers, &numUsuariosTotal, arqUsers, &idUsuario); //pega os usuarios do arquivo
    if(numUsuariosTotal == 0){
        remove("jogos.txt");
    }

    idLogado = chamaTelaInicial(arqUsers, users, &numUsuariosTotal, &idUsuario); //a função tela inicial retorna o id do usuario que esta logado no momento

    Jogo *jogos = (Jogo *) calloc(100, sizeof(Jogo));//cria um vetor de structs com espaço para 100 jogos
    int indexJogo = 0;


    arqJogos = fopen("jogos.txt", "a+");
    pegarJogosArquivo(jogos, &indexJogo, arqJogos); //pega os jogos do arquivo

    TelaJogos(jogos, indexJogo, idLogado); //chama o menu do programa para manipulação dos jogos


return 0;

}
