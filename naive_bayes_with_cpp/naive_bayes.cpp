// Supressao de warnings
#pragma GCC diagnostic push												
#pragma GCC diagnostic ignored "-Wsign-compare"


// Libs
#include <iostream>  // Entrada/Saída de dados
#include <chrono>    // Medir tempo de execução do script
#include <fstream>   // Manipulação de arquivos
#include <vector>    // Vetores
#include <string>    // trabalhar com strings
#include <numeric>   // trabalhar com numeros
#include <algorithm> // Preprocessamento de dados
#include <cmath>     // matemática
#include <math.h>    // matematica

// Namespace
using namespace std;
using namespace std::chrono;

// Ìndice de início dos dados de teste
const int starTest = 900;

// Número de previsões que serão mostradas
const int numOfIterations = 5;

// Struc
struct DataFrame {

    vector<string> cols;
    vector<vector<double>> data;

    DataFrame(int nrow, int ncols):
        cols(ncols, "NA"),
        data(nrow, vector<double> (ncols, 0)) {}
};


// Funções
ifstream readFile(string file_path);
DataFrame createDataFrame(vector<string> cols, vector<vector<double>> data);
void printDataFrame(DataFrame df, int n);
vector<double> slice(vector<double> v, int start, int end);

int main() {

    //------------------- Leitura da base de dados-------
    ifstream inputFile;
    inputFile = readFile("src/dataset.csv");


    //------------------- Declaração das Vars ---------------

    // Variáveis escalares do tipo double para tratar os valores de cada coluna
	double idVal;
	double tipo_docVal;
	double classeVal;
	double certificado_validoVal;
	double uso_diasVal;

    // Variáveis do tipo vetor para todos os elementos de cada coluna do dataset
	vector<double> id;
	vector<double> tipo_doc;			
	vector<double> classe;		
	vector<double> certificado_valido;		
	vector<double> uso_dias;

    // Variável para armazenar o cabeçalho do arquivo
    string header;

    // Variável para armazenar cada célula do arquivo csv
    string cell;

    // Pegar a primeira linha do aqruivo csv
    getline(inputFile, header);

    //------------------- Loop de Carga e Limpeza dos Dados ---------------

    // Loop de carga e limpeza inicial dos dados
    while(inputFile.good()) {
				
        // Leitura do primeiro elemento da linha (coluna id)
		getline(inputFile, cell, ','); 			

        // Remove aspas		
		cell.erase(remove(cell.begin(), cell.end(), '\"' ),cell.end());	
	
        // Agora seguimos fazendo a leitura somente das células com valores
		if(!cell.empty()) {								

			idVal = stod(cell);   // str to double	
			id.push_back(idVal); // Append do valor de x no vetor					
							
            // Leitura da coluna tipo_doc
			getline(inputFile, cell, ',');
            tipo_docVal = stod(cell); 				
			tipo_doc.push_back(tipo_docVal);		
				
            // Leitura da coluna classe	
			getline(inputFile, cell, ',');		
            classeVal = stod(cell); 	
			classe.push_back(classeVal);	
			
            // Leitura da coluna certificado_valido 
			getline(inputFile, cell, ',');		
            certificado_validoVal = stod(cell); 			
			certificado_valido.push_back(certificado_validoVal);				
				
            // Leitura da coluna uso_dias
			getline(inputFile, cell);						
            uso_diasVal = stod(cell); 				
			uso_dias.push_back(uso_diasVal);					
		}
		else {

            // Se linha vazia, finaliza o loop											
			break;
		}	
	}

    //------------------- Criação do DataFrame ---------------
    vector<string> cols = {"id","certificado_valido", "classe", "tipo_doc", "uso_dias"};
    vector<vector<double>> data = {id, certificado_valido, classe, tipo_doc, uso_dias};
    DataFrame df(id.size(), cols.size());

    df = createDataFrame(cols, data);
    printDataFrame(df, 5);
    cout << endl;

    //------------------- Divisao em treino e teste ---------------
    vector<vector<double>> data_train;
    DataFrame df_train(starTest, cols.size());
    data_train = {
        slice(id, 0, starTest), 
        slice(certificado_valido, 0, starTest), 
        slice(classe, 0, starTest), 
        slice(tipo_doc, 0, starTest), 
        slice(uso_dias, 0, starTest)
    };

    df_train = createDataFrame(cols, data_train);
    printDataFrame(df_train, 5);

    // falta colocar o teste
};

ifstream readFile(string file_path) {
    /*
    Leitura do arquivos de dados
    */

    ifstream inputFile;
    inputFile.open(file_path);

    // Verifica se há algum erro
    if(!inputFile.is_open()) {
        cout << "Falha ao abrir o arquivo" << endl;
    }

    return inputFile;
};

DataFrame createDataFrame(vector<string> cols, vector<vector<double>> data) {

    int nrows, ncols;
    nrows = data.at(0).size();
    ncols = cols.size();
    DataFrame df(nrows, ncols);

    for (int i = 0; i < ncols; i++) {
        df.cols.at(i) = cols.at(i);  
    }

    for (int i = 0; i < ncols; i++) {
        for (int j = 0; j < nrows; j++) {
            df.data.at(j).at(i) = data.at(i).at(j);
        }
        
    }

    return df;
};

void printDataFrame(DataFrame df, int n) {

    // header
    for(auto var: df.cols) {
        cout << var << "   ";
    }
    cout << endl;

    // row
    for (int i = 0; i < n; i++) {
        cout << 
            df.data.at(i).at(0) << "             " <<
            df.data.at(i).at(1) << "            " <<
            df.data.at(i).at(2) << "        " <<
            df.data.at(i).at(3) << "         " <<
            df.data.at(i).at(4) << endl;
        
    }
};

vector<double> slice(vector<double> v, int start, int end) {
    // Verifica se os índices são válidos
    if (start < 0 || end > v.size() || start > end) {
        throw out_of_range("Invalid slice indices");
    }
    return vector<double>(v.begin() + start, v.begin() + end);
}