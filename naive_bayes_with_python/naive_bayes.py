# -*- coding: utf-8 -*-
"""
Neste  projeto  nosso  trabalho  é  construir  um  modelo  de  Machine  Learning  capaz  de  
classificar   um   documento   como   tendo   ou   não   assinatura   digital  com  base  em  
algumas  características   do   documento.   

Usaremos   para   isso   um   dos   mais   poderosos   algoritmos   probabilísticos, o Naive Bayes, 
implementando-o em Python.
"""

#%% Setup
from sklearn.metrics import confusion_matrix
from typing          import Tuple

import pandas      as pd
import numpy       as np
import scipy.stats as st

#%% Load
df = pd.read_csv("src/dataset.csv")

#%% Preprocessing
df.rename(columns = {"Unnamed: 0" : "id"}, inplace = True)

starTest = 900
df_train = df.loc[0: starTest - 1,]
df_test  = df.loc[starTest:,]

#%% Split train and test
y_train = df_train.pop("classe")
X_train = df_train.drop("id", axis = 1)

y_test = df_test.pop("classe")
X_test = df_test.drop("id", axis = 1)

#%% Create Model
"""
P(y|X)   = P(x1|y) * P(x2|y) * ... P(xn |y) * P(y)   / [P(x1) * P(x2) *... P(xn)]
postior  = likelihood                       * priori / marginal

P(y|X) <-- prop --> P(x1|y) * P(x2|y) * ... P(xn |y) * P(y) 

P(y = 1 |X) = P(tipo_doc = v1 | y = 1) * P(certificado_valido = v2 | y = 1) * fdp(x = v3, med_y_1, std_y_1) * P(y = 1) 

P(y = 0 |X) = P(tipo_doc = v1 | y = 0) * P(certificado_valido = v2 | y = 0) * fdp(x = v3, med_y_0, std_y_0) * P(y = 0) 
"""

# Discrete
discrete_vars   = ["tipo_doc", "certificado_valido"]

discrete_probability_distribution = {}
[
 discrete_probability_distribution
 .update({
     var: pd.crosstab(y_train, X_train[var], normalize="index")
 })
 for var in discrete_vars
]


def discrete_likelihood(x : int, tab : pd.DataFrame) -> Tuple[float, float]:
    return tab.loc[0, x], tab.loc[1, x]


def continuous_likelihood(x : int, X : pd.DataFrame, y : pd.Series) -> Tuple[float, float]:
    
    mean_0 = X.loc[y == 0,].mean()
    std_0  = X.loc[y == 0,].std(ddof = 1)
    pdf_0  = st.norm.pdf(x = x, loc = mean_0, scale = std_0)
    
    mean_1 = X.loc[y == 1,].mean()
    std_1  = X.loc[y == 1,].std(ddof = 1)
    pdf_1  = st.norm.pdf(x = x, loc = mean_1, scale = std_1)

    return pdf_0, pdf_1


def apply(x:dict) -> int:
    
    p_tipo_doc_y_0, p_tipo_doc_y_1 = discrete_likelihood(
        x["tipo_doc"], 
        discrete_probability_distribution["tipo_doc"]
    )
    
    p_certificado_valido_y_0, p_certificado_valido_y_1 = discrete_likelihood(
        x["certificado_valido"], 
        discrete_probability_distribution["certificado_valido"]
    )
    
    p_uso_dias_y_0, p_uso_dias_y_1 = continuous_likelihood(
        x["uso_dias"],
        X_train["uso_dias"],
        y_train
    )
    
    priori_0 = (y_train == 0).sum()/y_train.shape[0]
    priori_1 = (y_train == 1).sum()/y_train.shape[0]
    
    likelihood_0 = p_tipo_doc_y_0 * p_certificado_valido_y_0 * p_uso_dias_y_0
    likelihood_1 = p_tipo_doc_y_1 * p_certificado_valido_y_1 * p_uso_dias_y_1
    
    posterior_0 = likelihood_0 * priori_0
    posterior_1 = likelihood_1 * priori_1
    
    return np.argmax(np.array([posterior_0, posterior_1]))    
    

#%% Apply
y_pred_train = []
for x in X_train.to_dict(orient = "records"):
    y_pred_train.append(apply(x))
    
y_pred_test = []
for x in X_test.to_dict(orient = "records"):
    y_pred_test.append(apply(x))

#%% Eval

cm  = confusion_matrix(y_test, y_pred_test)

def acc(cm : np.ndarray) -> float:
    
    TN = cm[0,0]
    FP = cm[0,1]
    FN = cm[1,0]
    TP = cm[1,1]
    
    acc_val = (TN + TP)/(TN + FP + FN + TP)
    
    return acc_val


def sens(cm : np.ndarray) -> float:
    
    FN = cm[1,0]
    TP = cm[1,1]
    
    sens_val = TP/(TP + FN)
    
    return sens_val


def spec(cm : np.ndarray) -> float:
    
    TN = cm[0,0]
    FP = cm[0,1]
    
    spec_val = TN/(TN + FP)
    
    return spec_val

    
print(f"Acurácia: {acc(cm)}")
print(f"Sensibilidade {sens(cm)}")
print(f"Especificidade {spec(cm)}")










