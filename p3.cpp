// TAG - 2021.1
// Universidade de Brasília
// Bruno Vargas de Souza - 202006564
// Referências - 
// https://afteracademy.com/blog/sudoku-solver
// https://www.geeksforgeeks.org/m-coloring-problem-backtracking-5/
// https://www.cmmprogressivo.net/2019/12/Como-Gerar-Numeros-Aleatorios-Randomicos-rand-srand.html
// https://cplusplus.com/reference/algorithm/random_shuffle/

#include <bits/stdc++.h>
#include <locale.h>
using namespace std;

// Cores para o output
#define NC "\e[0m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define CYN "\e[0;36m"
#define REDB "\e[41m"

//////////////////////////////////////////////////////////////////////////////////////////
//                            FUNCOES DO SUDOKU EM GERAL                                 /
//////////////////////////////////////////////////////////////////////////////////////////

// 81 vertices do sudoku e suas arestas
vector<int> sudoku[82];

// algumas variaveis uteis
bool obrigatorio[82];
int cor[82];

// transforma a matriz em um numero de vertice
int matriz_to_v(int i, int j){
    return (9*(j-1))+i;
}

// transforma o numero do vertice em matriz
pair<int,int> v_to_matriz(int v){
    return {((v+8)%9)+1, ((v+8)/9)};
}

// retorna a primeira coord do quadrante 3x3
pair<int,int> quadrante(int i, int j){
    int xquad = ((i+2)/3);
    int yquad = ((j+2)/3);
    int x = (xquad*3)-2;
    int y = (yquad*3)-2;
    return {x, y};
}

// le o sudoku de acordo com o arquivo passado
void leitura_sudoku(string s){
    ifstream in;
    in.open(s);
    // se o numero for diferente de zero
    // ele é obrigatorio
    // nao pode mudar esse numero
    for(int i=1; i<=81; i++){
        int color; in>>color;
        cor[i] = color;
        if(color != 0){
            obrigatorio[i] = true;
        }
    }     
    in.close();
}

// faz a ligacao entre os vertices adjascentes
// horizontal, vertical, e entre os quadrados
void ligacao_vertices(){
    for(int j=1; j<=9; j++){
        for(int i=1; i<=9; i++){
            // para cada i e j
            
            int v = matriz_to_v(i, j);
            for(int k=1; k<=9; k++){
                if(k != j){
                    // horizontalmente
                    int u = matriz_to_v(i, k);
                    sudoku[v].push_back(u);
                }
                if(k != i){
                    // verticalmente
                    int u = matriz_to_v(k, j);
                    sudoku[v].push_back(u);
                }
            }
            pair<int,int> coords = quadrante(i, j);
            // quadrados
            for(int jj=0; jj<3; jj++){
                for(int ii=0; ii<3; ii++){
                    int x = coords.first + ii, y = coords.second+ jj;  
                    if(x != i and y != j){
                        int u = matriz_to_v(x, y);
                        sudoku[v].push_back(u);
                    }
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//                              FUNCOES DO SUDOKU NORMAL                                 /
//////////////////////////////////////////////////////////////////////////////////////////

// verifica se é possivel colorir um vertice olhando seus adjascentes
bool possivel_solucao(int v, int color){
    for(auto u: sudoku[v]){
        if(color == cor[u]) return false;
    }
    return true;
}

// verifica se todo o grafo está certo
bool solucao_final(){
    for(int i=1; i<=81; i++){
        for(auto v: sudoku[i]){
            if(cor[i] and cor[i] == cor[v]) return false;
        }
    }
    return true;
}

// armazena os passos intermediarios da solucao
vector<string> passos;

// faz a funcao de percorrer e colorir o grafo
// usando backtracking
// para cada vertice, ele verifica se consegue colorir de uma certa cor
// verificando seus adjascentes
bool colorir_grafo(){
    for(int i=1; i<=81; i++){
        if(cor[i] == 0){
            for(int color=1; color<=9; color++){
                if(possivel_solucao(i, color)){
                    cor[i] = color;
                    
                    if(colorir_grafo()) {
                        // adiciona o passo intermediario em uma string para printar depois
                        string passo = "Vertice ";
                        passo += to_string(i);
                        passo += " pintado com a cor: ";
                        passo+= to_string(color);
                        passo += " -> Correto";

                        passos.push_back(passo);

                        return true;
                    }
                    // adiciona o passo intermediario em uma string para printar depois
                    string passo = "Vertice ";
                    passo += to_string(i);
                    passo += " pintado com a cor: ";
                    passo+= to_string(color);
                    passo += " -> Incorreto";
                    
                    passos.push_back(passo);
                    cor[i] = 0;
                }
            }
            return false;
        }
    }
    return true;
}

/////////////////////////////////////////////
//      FUNCOES DO SUDOKU ALEATORIO         /
/////////////////////////////////////////////

// variaveis do sudoku aleatorio
int cor_aleatorio[82];
bool num_ocupado[82][10];
bool visited_aleatorio[82];

bool possivel_solucao_aleatorio(int v, int color){
    for(auto u: sudoku[v]){
        if(color == cor_aleatorio[u]) return false;
    }
    return true;
}

// funcao que sera usada para randomizar um vetor
int myrandom (int i) { return std::rand()%i;}

// funcao que vai colorir o grafo aleatorio parecido com o colorir_grafo()
// porem usa algumas funcoes de randomizacao
bool colorir_grafo_aleatorio(){
    for(int i=1; i<=81; i++){
        if(cor_aleatorio[i] == 0){
            vector<int> colors(9);
            // inicializa o vetor de 1 a 9
            iota(colors.begin(), colors.end(), 1);
            // deixa o vetor aleatorio
            random_shuffle(colors.begin(), colors.end(), myrandom);

            for(int idx=0; idx<9; idx++){
                if(possivel_solucao_aleatorio(i, colors[idx])){
                    cor_aleatorio[i] = colors[idx];
                    if(colorir_grafo_aleatorio()) {
                        return true;
                    }
                    cor_aleatorio[i] = 0;
                }
            }
            return false;
        }
    }
    return true;
}

// gera numero aleatorio de 1 a x
int aleatorio(int x){
    return (rand()%x) + 1;
}

// funcao que vai gerar um sudoku aleatorio com n cores pintadas
// usando backtracking, em que ele gera o sudoku
// e depois remove alguns vertices para que seja solucionado

void gera_aleatorio(int n){
    int count_pintados = 81;
    bool removido[81];
    for(int i=1; i<=81; i++)removido[i]=false;
    colorir_grafo_aleatorio();
    while(count_pintados > (81-n)){
        int x = aleatorio(81);

        if(removido[x]) {continue;}
        else{
            removido[x] = true;
            cor_aleatorio[x] = 0;
            count_pintados--;
        }
    }
}

// funcao que ira resetar os valores das variaveis
void reseta_valores(){
    memset(obrigatorio, 0, sizeof(obrigatorio));
    memset(cor, 0, sizeof(cor));
    memset(cor_aleatorio, 0, sizeof(cor_aleatorio));
    vector<string> x;
    passos = x;
    memset(num_ocupado, 0, sizeof(num_ocupado));
    memset(visited_aleatorio, 0, sizeof(visited_aleatorio));
}

int main(){//sws;
    // define a linguagem
    setlocale(LC_ALL, "Portuguese");
    // gerar numeros aleatorios
    srand(time(NULL));
    ligacao_vertices();

    cout<< "Digite 'sair' para encerrar o programa"<<"\n";
    cout<< "1 - Solucionar um Sudoku"<<"\n";
    cout<< "2 - Checar o Sudoku do arquivo"<<"\n";
    cout<< "3 - Gerar um sudoku aleatorio"<<"\n";
    cout<< "======================================================="<<"\n";

    string in;
    cin>>in;
    while(in != "sair"){
        
        if(in == "1"){
            cout<<"\n";
            string in1;
            cout<< "Digite 'sair' para encerrar o programa"<<"\n";
            cout<< "1 - Usar o file 'sudoku.txt' "<<"\n";
            cout<< "2 - Usar o file 'sudoku_aleatorio.txt' "<<"\n";
            cout<< "======================================================="<<"\n";
            cin>>in1;

            if(in1 == "1" or in1 == "2"){
                if(in1 == "1") leitura_sudoku("sudoku.txt");
                else if(in1 == "2") leitura_sudoku("sudoku_aleatorio.txt");
                
                colorir_grafo();
                ofstream file_solve;
                file_solve.open("resposta_sudoku.txt");
                int aux=1;
                for(int i=1; i<=81; i++){
                    file_solve<<cor[i]<<" ";
                    if(aux == 9){
                        aux = 0;
                        file_solve<<"\n";
                    }
                    aux++;
                }
                file_solve.close();

                cout<<GRN "Gerado um arquivo 'resposta_sudoku.txt' "<<"\n\n"<<NC;

                cout<<CYN "Deseja visualizar os passos do algoritmo? (pode levar alguns segundos) (y/n)"<<NC;
                string opt;
                cin>>opt;

                if(opt == "y"){
                    for(int i=0; i<passos.size(); i++){
                        cout<<passos[i]<<endl;
                    }
                }
                cout<<"\n\n";
                cout<< "======================================================="<<"\n";
            }
            

        }else if(in == "2"){
            cout<<"\n";
            string in1;
            cout<< "Digite 'sair' para encerrar o programa"<<"\n";
            cout<< "1 - Checar o arquivo 'sudoku.txt' "<<"\n";
            cout<< "2 - Checar o arquivo 'sudoku_aleatorio.txt' "<<"\n";
            cout<< "======================================================="<<"\n";
            cin>>in1;
            if(in1 == "1" or in1 == "2"){
                if(in1 == "1") leitura_sudoku("sudoku.txt");
                else if(in1 == "2") leitura_sudoku("sudoku_aleatorio.txt");
                if(colorir_grafo()){
                    cout<<GRN "O Sudoku tem solucao !"<<"\n\n";
                    cout<<NC;
                }else{
                    cout<<RED "O Sudoku nao tem solucao !"<<"\n\n";
                    cout<<NC;
                }
                cout<< "======================================================="<<"\n";
            }

        }else if(in == "3"){
            cout<<"\n";
            gera_aleatorio(25);
            ofstream file_aleatorio;
            file_aleatorio.open("sudoku_aleatorio.txt");
            int aux_aleatorio=1;
            for(int i=1; i<=81; i++){
                file_aleatorio<<cor_aleatorio[i]<<" ";
                if(aux_aleatorio == 9){
                    aux_aleatorio = 0;
                    file_aleatorio<<"\n";
                }
                aux_aleatorio++;
            }
            file_aleatorio.close();
            cout<<GRN "Sudoku aleatorio gerado no arquivo 'sudoku_aleatorio.txt'\n\n"<< NC;
            cout<< "======================================================="<<"\n";
        }

        reseta_valores();
        cout<< "Digite 'sair' para encerrar o programa"<<"\n";
        cout<< "1 - Solucionar um Sudoku"<<"\n";
        cout<< "2 - Checar o Sudoku do arquivo"<<"\n";
        cout<< "3 - Gerar um sudoku aleatorio"<<"\n";
        cout<< "======================================================="<<"\n";

        cin>>in;
    }
    
}