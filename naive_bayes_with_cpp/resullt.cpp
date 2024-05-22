// Supressao de warnings
#pragma GCC diagnostic push												
#pragma GCC diagnostic ignored "-Wsign-compare"


// Libs
#include <iostream> // Entrada/Saída de dados
#include <chrono> // Medir tempo de execução do script
#include <fstream> // Manipulação de arquivos
#include <vector> // Vetores
#include <string>  // trabalhar com strings
#include <numeric> // trabalhar com numeros
#include <algorithm> // Preprocessamento de dados
#include <cmath> // matemática
#include <math.h> // matematica

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
    vector<string> cols_type;
    vector<vector<string>> data;

    DataFrame(int nrow, int ncols):
        cols(ncols, "NA"),
        cols_type(cols.size(), "NA"),
        data(nrow, vector<string> (ncols, "NA")) {}
};


// Funções
ifstream readFile(string file_path);
DataFrame createDataFrame(vector<string> cols, vector<string> cols_type, vector<vector<string>> data);
void printDataFrame(DataFrame df);



int main() {

    //------------------- Leitura da base de dados-------
    ifstream inputFile;
    inputFile = readFile("src/dataset.csv");


    //------------------- Declaração das Vars ---------------

    // Variáveis do tipo vetor para todos os elementos de cada coluna do dataset
    vector<string> id;
    vector<string> tipo_doc;
    vector<string> classe;
    vector<string> certificado_valido;
    vector<string> uso_dias;

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

			id.push_back(cell); // Append do valor de x no vetor					
							
            // Leitura da coluna tipo_doc
			getline(inputFile, cell, ','); 				
			tipo_doc.push_back(cell);		
				
            // Leitura da coluna classe	
			getline(inputFile, cell, ',');		
			classe.push_back(cell);	
			
            // Leitura da coluna certificado_valido 
			getline(inputFile, cell, ',');				
			certificado_valido.push_back(cell);				
				
            // Leitura da coluna uso_dias
			getline(inputFile, cell);							
			uso_dias.push_back(cell);					
		}
		else {

            // Se linha vazia, finaliza o loop											
			break;
		}	
	}

    //------------------- Criação do DataFrame ---------------
    vector<string> cols = {"id","certificado_valido", "classe", "tipo_doc", "uso_dias"};
    vector<string> cols_type = {"num","num", "num", "num", "num"};
    vector<vector<string>> data(id.size(), vector<string> (cols.size(), "NA"));

    for (int i = 0; i < id.size(); i++) {
        data.at(i).at(0) = id.at(i);
        data.at(i).at(1) = certificado_valido.at(i);
        data.at(i).at(2) = classe.at(i);
        data.at(i).at(3) = tipo_doc.at(i);
        data.at(i).at(4) = uso_dias.at(i);
    }

    DataFrame df(data.size(), cols.size());
    df = createDataFrame(cols, cols_type, data);

    printDataFrame(df);

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

DataFrame createDataFrame(vector<string> cols, vector<string> cols_type, vector<vector<string>> data) {

    int nrows, ncols;
    nrows = data.size();
    ncols = cols.size();
    DataFrame df(nrows, ncols);

    for (int i = 0; i < ncols; i++) {
        df.cols.at(i)      = cols.at(i);
        df.cols_type.at(i) = cols_type.at(i);    
    }

    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            df.data.at(i).at(j) = data.at(i).at(j);
        }
        
    }

    return df;
};

void printDataFrame(DataFrame df) {

    int nrows, ncols;
    nrows = df.data.size();
    ncols = df.cols.size();
    vector<int> max_length(ncols, 0);

    for (int i = 0; i < ncols; i++) {

        if (max_length.at(i) < df.cols.at(i).length()) {
            max_length.at(i) = df.cols.at(i).length();
        }

        for (int j = 0; j < nrows; j++) {

            if (max_length.at(i) < df.data.at(j).at(i).length()) {
                max_length.at(i) = df.data.at(j).at(i).length();
            }
        }
    }

    for (int i = 0; i < nrows; i++) {

        for (int j = 0; j < ncols; j++) {

            string space = "";
            int val = max_length.at(j)-4;

            for (int z = 0; z < val; z++) {
                space = space + " ";
            }

            cout << df.data.at(i).at(j) << "    ";
        }

        cout << endl;
        
    }
};