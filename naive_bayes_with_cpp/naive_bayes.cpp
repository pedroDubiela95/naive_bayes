// Supressao de warnings
#pragma GCC diagnostic push												
#pragma GCC diagnostic ignored "-Wsign-compare"

// Libs
#include <iostream>  // Entrada/Saída de dados
#include <fstream>   // Manipulação de arquivos
#include <vector>    // Vetores
#include <string>    // trabalhar com strings
#include <numeric>   // trabalhar com numeros
#include <algorithm> // Preprocessamento de dados
#include <cmath>     // matemática
#include <math.h>    // matematica

// Namespace
using namespace std;

//----------------------------------------------Constantes e Estruturas---------------------------------------------------
// Ìndice de início dos dados de teste
const int starTest = 900;

// Struc
struct DataFrame {

    vector<string> cols;
    vector<vector<double>> data;

    DataFrame(int nrow, int ncols):
        cols(ncols, "NA"),
        data(nrow, vector<double> (ncols, 0)) {}
};

//----------------------------------------------Funções--------------------------------------------------------------------
// Leitura do arquivo .csv
ifstream read_file(string file_path);

// Instancia um objeto do tipo DataFrame
DataFrame create_DataFrame(vector<string> cols, vector<vector<double>> data);

// Printa um dataframe
void print_DataFrame(DataFrame df, int n, string where = "top");

// Reliza o slice em vetorores 1D
vector<double> slice(vector<double> v, int start, int end);

// Calcula uma tabela de frequências para a feature certificado_valido
vector<vector<double>> get_distribution_certificado_valido(DataFrame df, vector<vector<double>> tab);

// Calcula uma tabela de frequências para a feature tipo_doc
vector<vector<double>> get_distribution_tipo_doc(DataFrame df, vector<vector<double>> tab);

// Calcula a média para a váriável target classe quando y = 0 e quando y = 1
vector<double>  calc_mean(DataFrame df);

// Calcula o desvio padrão para a váriável target classe quando y = 0 e quando y = 1
vector<double> calc_std(DataFrame df, vector<double> mean);

// Encapsula a execução das funções 
// --> get_distribution_certificado_valido
// --> get_distribution_tipo_doc
// --> calc_mean
// --> calc_std
// Gerando um vetor de vetores do tipo double que representa o modelo treinado
vector<vector<vector<double>>> fit(DataFrame df);

// Calcula as verossimilhanças P(certificado_valido = x | classe = y)
double likelihood_certificado_valido(vector<vector<vector<double>>> probs, int y, double val_x);

// Calcula as verossimilhanças P(tipo_doc = x | classe = y)
double likelihood_tipo_doc(vector<vector<vector<double>>> probs, int y, double val_x);

// Calcula as verossimilhanças P(uso_dias = x | classe = y)
double likelihood_uso_dias(vector<vector<vector<double>>> probs, int y, double val_x);

// Calcula as probabilidades à priori P(classe = 0) e P(classe = 1) 
vector<double> priori(DataFrame df);

// Aplica o modelo na base de testes
vector<double> apply_model(DataFrame df_test, DataFrame df_train, vector<vector<vector<double>>> probs);

// Avalia a performance do modelo obtido
void evaluate(vector<double> y_true, vector<double> y_pred);


int main() {

    cout << endl <<"#################### Naive Bayes #########################" << endl << endl;

    //----------------------------------------------Leitura da base de dados---------------------------------------------------
    ifstream inputFile;
    inputFile = read_file("src/dataset.csv");

    //----------------------------------------------Declaração das Vars -------------------------------------------------------
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

    //---------------------------------------------Loop de Carga e Limpeza dos Dados-------------------------------------------
    // Loop de carga e limpeza inicial dos dados
    while(inputFile.good()) {
				
        // Leitura do primeiro elemento da linha (coluna id)
		getline(inputFile, cell, ','); 			

        // Remove aspas		
		cell.erase(remove(cell.begin(), cell.end(), '\"' ),cell.end());	
	
        // Agora seguimos fazendo a leitura somente das células com valores
		if(!cell.empty()) {								

			idVal = stod(cell);   // str to double	
			id.push_back(idVal);  // Append do valor de x no vetor					
							
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

    //---------------------------------------------Criação dos DataFrames------------------------------------------------------
    vector<string> cols = {"id","certificado_valido", "classe", "tipo_doc", "uso_dias"};
    vector<vector<double>> data = {id, certificado_valido, classe, tipo_doc, uso_dias};
    DataFrame df(id.size(), cols.size());

    df = create_DataFrame(cols, data);
    
    cout << "#------------Dataframe - primeiras 5 linhas------------#"  << endl;
    print_DataFrame(df, 5);
    cout << endl;
    //cout << "#-------------Dataframe - últimas 5 linhas-------------#"  << endl;
    //print_DataFrame(df, 5, "bottom");
    cout << endl <<"=======================================================" << endl;

    //Treino
    vector<vector<double>> data_train;
    DataFrame df_train(starTest, cols.size());
    data_train = {
        slice(id,                 0, starTest), 
        slice(certificado_valido, 0, starTest), 
        slice(classe,             0, starTest), 
        slice(tipo_doc,           0, starTest), 
        slice(uso_dias,           0, starTest)
    };

    df_train = create_DataFrame(cols, data_train);
    cout << "#---------Dataframe Treino - primeiras 5 linhas---------#" << endl;
    print_DataFrame(df_train, 5);
    cout << endl;
    //cout << "#----------Dataframe Treino - últimas 5 linhas----------#"<< endl;
    //print_DataFrame(df_train, 5, "bottom");
    cout << endl <<"=======================================================" << endl;

    //Teste
    vector<vector<double>> data_test;
    DataFrame df_test(id.size() - starTest, cols.size());
    data_test = {
        slice(id,                 starTest, id.size()), 
        slice(certificado_valido, starTest, id.size()), 
        slice(classe,             starTest, id.size()), 
        slice(tipo_doc,           starTest, id.size()), 
        slice(uso_dias,           starTest, id.size())
    };

    df_test = create_DataFrame(cols, data_test);                    
    cout << "#---------Dataframe Teste - primeiras 5 linhas---------#" << endl;
    print_DataFrame(df_test, 5);
    cout << endl;
    //cout << "#---------Dataframe Teste - últimas 5 linhas----------#"  << endl;
    //print_DataFrame(df_test, 5, "bottom");
    cout << endl <<"=======================================================" << endl;

    //-----------------------------------------------Treina Modelo-------------------------------------------------------------
    vector<vector<vector<double>>> probs;
    probs = fit(df_train);

    //-----------------------------------------------Aplica Modelo-------------------------------------------------------------
    vector<double> y_pred = apply_model(df_test, df_train, probs);

    //-----------------------------------------------Avaliação-----------------------------------------------------------------
    vector<double> y_true;
    for(auto row: df_test.data) {y_true.push_back(row.at(2));};
    evaluate(y_true, y_pred);

    cout << endl <<"##########################################################" << endl;
};


ifstream read_file(string file_path) {

    ifstream inputFile;
    inputFile.open(file_path);

    // Verifica se há algum erro
    if(!inputFile.is_open()) {
        cout << "Falha ao abrir o arquivo" << endl;
    }

    return inputFile;
};


DataFrame create_DataFrame(vector<string> cols, vector<vector<double>> data) {

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


void print_DataFrame(DataFrame df, int n, string where) {

    // header
    for(auto var: df.cols) {
        cout << var << "   ";
    }
    cout << endl;


    if(where == "top") {
        // row
        for (int i = 0; i < n; i++) {
            cout << 
                df.data.at(i).at(0) << "             " <<
                df.data.at(i).at(1) << "            " <<
                df.data.at(i).at(2) << "        " <<
                df.data.at(i).at(3) << "         " <<
                df.data.at(i).at(4) << endl;
            
        }

    }

    if(where == "bottom") {
        int start = df.data.size() - n;
        // row
        for (int i = start; i < df.data.size(); i++) {
            cout << 
                df.data.at(i).at(0) << "             " <<
                df.data.at(i).at(1) << "            " <<
                df.data.at(i).at(2) << "        " <<
                df.data.at(i).at(3) << "         " <<
                df.data.at(i).at(4) << endl;
            
        }
    }

};


vector<double> slice(vector<double> v, int start, int end) {
    // Verifica se os índices são válidos
    if (start < 0 || end > v.size() || start > end) {
        throw out_of_range("Invalid slice indices");
    }
    return vector<double>(v.begin() + start, v.begin() + end);
}


vector<vector<double>> get_distribution_certificado_valido(DataFrame df, vector<vector<double>> tab) {

    for (auto row: df.data) {

        if(row.at(1) == 0 && row.at(2) == 0) {
            tab.at(0).at(0)++;
        }

        if(row.at(1) == 1 && row.at(2) == 0) {
            tab.at(0).at(1)++;
        }

        if(row.at(1) == 0 && row.at(2) == 1) {
            tab.at(1).at(0)++;
        }

        if(row.at(1) == 1 && row.at(2) == 1) {
            tab.at(1).at(1)++;
        }

    }

    cout << "Distribuição da feature certificado_valido" << endl;
    tab.at(0).at(0) = tab.at(0).at(0) / (tab.at(0).at(0) + tab.at(0).at(1)); 
    tab.at(0).at(1) = 1 - tab.at(0).at(0);
    
    tab.at(1).at(0) = tab.at(1).at(0) / (tab.at(1).at(1) + tab.at(1).at(0)); 
    tab.at(1).at(1) = 1 - tab.at(1).at(0);

    cout << "        x = 0     x = 1" << endl;
    cout <<"y = 0 "<<tab.at(0).at(0) << " ";
    cout << tab.at(0).at(1) << endl;
    cout <<"y = 1 "<<tab.at(1).at(0) << " ";
    cout << tab.at(1).at(1) << endl;
    cout << endl <<"=======================================================" << endl;

    return tab;
}


vector<vector<double>> get_distribution_tipo_doc(DataFrame df, vector<vector<double>> tab) {

    for (auto row: df.data) {

        if(row.at(3) == 1 && row.at(2) == 0) {
            tab.at(0).at(0)++;
        }

        if(row.at(3) == 2 && row.at(2) == 0) {
            tab.at(0).at(1)++;
        }

        if(row.at(3) == 3 && row.at(2) == 0) {
            tab.at(0).at(2)++;
        }

        if(row.at(3) == 1 && row.at(2) == 1) {
            tab.at(1).at(0)++;
        }

        if(row.at(3) == 2 && row.at(2) == 1) {
            tab.at(1).at(1)++;
        }

        if(row.at(3) == 3 && row.at(2) == 1) {
            tab.at(1).at(2)++;
        }

    }

    cout << "Distribuição da feature tipo_doc" << endl;
    double sum;

    sum = (tab.at(0).at(0) + tab.at(0).at(1) + tab.at(0).at(2));
    tab.at(0).at(0) = tab.at(0).at(0) / sum;
    tab.at(0).at(1) = tab.at(0).at(1) / sum;
    tab.at(0).at(2) = tab.at(0).at(2) / sum; 
    
    sum = (tab.at(1).at(0) + tab.at(1).at(1) + tab.at(1).at(2)); 
    tab.at(1).at(0) = tab.at(1).at(0) / sum;
    tab.at(1).at(1) = tab.at(1).at(1) / sum; 
    tab.at(1).at(2) = tab.at(1).at(2) / sum; 

    cout << "        x = 1     x = 2    x = 3" << endl;
    cout <<"y = 0 "<<tab.at(0).at(0) << " ";
    cout << tab.at(0).at(1) << " ";
    cout << tab.at(0).at(2) << endl;
    cout <<"y = 1 "<<tab.at(1).at(0) << " ";
    cout << tab.at(1).at(1) << " ";
    cout << tab.at(1).at(2) << endl;
    cout << endl <<"=======================================================" << endl;

    return tab;
};


vector<double> calc_mean(DataFrame df) {

    vector<double> mean_uso_dias(2,0);
    double sum_0, sum_1;
    int cnt_0, cnt_1;

    sum_0 = 0;
    sum_1 = 0;
    cnt_0 = 0;
    cnt_1 = 0;

    for(auto row: df.data) {
        if(row.at(2) == 0) {
            sum_0 += row.at(4);
            cnt_0++;
        }
        if(row.at(2) == 1) {
            sum_1 += row.at(4);
            cnt_1++;
        }
    }

    mean_uso_dias.at(0) = sum_0/cnt_0;
    mean_uso_dias.at(1) = sum_1/cnt_1;

    cout << "Médias da feature uso_dias" << endl;
    cout << "y = 0     y = 1" << endl;
    cout << mean_uso_dias.at(0) <<" ";
    cout << mean_uso_dias.at(1) << endl;
    cout << endl <<"=======================================================" << endl;

    return mean_uso_dias;
};


vector<double> calc_std(DataFrame df, vector<double> mean) {

    vector<double> std_uso_dias(2,0);
    double sum_0, sum_1;
    int cnt_0, cnt_1;

    sum_0 = 0;
    sum_1 = 0;
    cnt_0 = 0;
    cnt_1 = 0;

    for(auto row: df.data) {
        if(row.at(2) == 0) {
            sum_0 += pow((row.at(4) - mean.at(0)), 2);
            cnt_0++;
        }
        if(row.at(2) == 1) {
            sum_1 += pow((row.at(4) - mean.at(1)), 2);
            cnt_1++;
        }
    }

    std_uso_dias.at(0) = sqrt(sum_0/(cnt_0 - 1));
    std_uso_dias.at(1) = sqrt(sum_1/(cnt_1 - 1));

    cout << "Desvio padrão da feature uso_dias" << endl;
    cout << "y = 0     y = 1" << endl;
    cout << std_uso_dias.at(0) <<" ";
    cout << std_uso_dias.at(1) << endl;
    cout << endl <<"=======================================================" << endl;

    return std_uso_dias;
};


vector<vector<vector<double>>> fit(DataFrame df) {

    // ------------------------- Discretas ------------------------------------
    vector<vector<double>>  probability_distribution_certificado_valido(2, vector<double>(2,0)); // 2 x 2
    vector<vector<double>>  probability_distribution_tipo_doc(2, vector<double>(3,0));           // 2 x 3

    // --> certificado_valido
    probability_distribution_certificado_valido = get_distribution_certificado_valido(
        df, 
        probability_distribution_certificado_valido
    );

    // --> tipo_doc
    probability_distribution_tipo_doc = get_distribution_tipo_doc(
        df, 
        probability_distribution_tipo_doc
    );

    // ------------------------- Contínuas ------------------------------------
    vector<double> mean_uso_dias(2,0);                                                 // 1x2 
    vector<double> std_uso_dias(2,0);                                                  // 1x2
    vector<vector<double>>  probability_distribution_uso_dias(2, vector<double>(2,0)); // 2 x 2

    // --> uso_dias
    mean_uso_dias                     = calc_mean(df);
    std_uso_dias                      = calc_std(df, mean_uso_dias);
    probability_distribution_uso_dias = {
        mean_uso_dias,
        std_uso_dias
    };

    // ------------------------- Resultado ------------------------------------
        
    // Result
    vector<vector<vector<double>>> probs;
    probs = {
        probability_distribution_certificado_valido,
        probability_distribution_tipo_doc,
        probability_distribution_uso_dias
    };

    return probs;

};

double likelihood_certificado_valido(vector<vector<vector<double>>> probs, int y, double val_x) {
    return probs.at(0).at(y).at(val_x);
};

double likelihood_tipo_doc(vector<vector<vector<double>>> probs, int y, double val_x) {
    return probs.at(1).at(y).at(val_x-1);
}
    
double likelihood_uso_dias(vector<vector<vector<double>>> probs, int y, double val_x) {

    double mean, std, var, den, num, pdf;

    mean = probs.at(2).at(0).at(y);
    std  = probs.at(2).at(1).at(y);
    var  = pow(std, 2);

    // Gaussian
    den = sqrt((2 * M_PI * var));
    num = exp(-pow((val_x- mean), 2)  /(2*var));
    pdf = num/den;

    //cout <<"y = "<<y<< "| valor =  " << val_x << "| mean = " << mean << "| std = " << std << "| pdf = " << pdf << endl;
    return pdf;
}


vector<double> priori(DataFrame df) {

    vector<double> priori(2,0);
    double cnt_0, cnt_1;

    cnt_0 = 0;
    cnt_1 = 0;

    for(auto row: df.data) {
        if(row.at(2) == 0) {
            cnt_0++;
        }
        if(row.at(2) == 1) {
            cnt_1++;
        }
    }

    priori = {
        cnt_0/df.data.size(),
        cnt_1/df.data.size()
    };

    return priori;
};


vector<double> apply_model(DataFrame df_test, DataFrame df_train, vector<vector<vector<double>>> probs) {

    //posteriori   
    double posterior_0, posterior_1;

    //piori
    vector<double> priori_all = priori(df_train);
    double priori_0, priori_1;

    priori_0 = priori_all.at(0);
    priori_1 = priori_all.at(1);

    //Verossimilhança
    double p_tipo_doc_y_0, p_tipo_doc_y_1;
    double p_certificado_valido_y_0, p_certificado_valido_y_1;
    double p_uso_dias_y_0, p_uso_dias_y_1;
    double likelihood_0,likelihood_1;
    double val_x, y;

    // pred
    vector<double> y_pred;

    // Calcula
    for (auto row: df_test.data) {

        // certificado_valido
        val_x = row.at(1);
        p_certificado_valido_y_0 = likelihood_certificado_valido(probs, 0, val_x);
        p_certificado_valido_y_1 = likelihood_certificado_valido(probs, 1, val_x);

        // tipo_doc
        val_x = row.at(3);
        p_tipo_doc_y_0 = likelihood_tipo_doc(probs, 0, val_x);
        p_tipo_doc_y_1 = likelihood_tipo_doc(probs, 1, val_x);

        // uso_dias
        val_x = row.at(4);
        p_uso_dias_y_0 = likelihood_uso_dias(probs, 0, val_x);
        p_uso_dias_y_1 = likelihood_uso_dias(probs, 1, val_x);

        likelihood_0 = p_certificado_valido_y_0 * p_tipo_doc_y_0 * p_uso_dias_y_0;
        likelihood_1 = p_certificado_valido_y_1 * p_tipo_doc_y_1 * p_uso_dias_y_1;

        posterior_0 = likelihood_0 * priori_0;
        posterior_1 = likelihood_1 * priori_1;

        // normaliza
        if(posterior_0 > posterior_1){
            y_pred.push_back(0);
        } else {
            y_pred.push_back(1);
        }
    }
    return y_pred;
};


void evaluate(vector<double> y_true, vector<double> y_pred){

    double tp = 0, tn = 0, fp = 0, fn = 0;

    for (int i = 0; i < y_pred.size(); i++) {

        if(y_true.at(i) == 1 && y_pred.at(i) == 1){
            tp++;
        }
        if(y_true.at(i) == 1 && y_pred.at(i) == 0){
            fn++;
        }

        if(y_true.at(i) == 0 && y_pred.at(i) == 1){
            fp++;
        }

        if(y_true.at(i) == 0 && y_pred.at(i) == 0){
            tn++;
        }
    }

    double acc  = (tp + tn)/(tp + tn + fp + fn);
    double sens = tp/(tp + fn);
    double spec = tn/(tn + fp);

    cout << "Avaliação do Modelo" << endl;
    cout << "Acurácia: " << acc   << endl 
    << "Sensibilidade: " << sens  << endl 
    <<"Especificidade: " << spec  << endl; 
}