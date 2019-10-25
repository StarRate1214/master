import os
import email
import email.policy
import email.parser
import re
from sklearn.base import BaseEstimator, TransformerMixin
import numpy as np
from sklearn.model_selection import train_test_split
from scipy.sparse import csr_matrix
from sklearn.pipeline import Pipeline
import dpkt

class Load_data:
    def __init__(self):
        self.normal=[]
        self.sqli=[]
        
    def loadFromtxt(self, normal_path, sqli_path, encode='utf-8'):
        with open(normal_path, "r", encoding=encode) as f:
                file=f.readlines()
                for data in file:
                    self.normal.append(data)
        with open(sqli_path, "r", encoding=encode) as f:
                file=f.readlines()
                for data in file:
                    self.sqli.append(data)
        return self.normal, self.sqli

    def get_separate_datasets(self):
        X = np.array(self.normal + self.sqli)
        y = np.array([0] * len(self.normal) + [1] * len(self.sqli))
        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
        return X_train, X_test, y_train, y_test

class TransformData(BaseEstimator, TransformerMixin):
    def __init__(self, keyword=None):
        if(keyword==None):
            self.keyword=['--','/**/', 'AND', 'OR', 'UNION', 'WHERE', 'LIMIT', 'GROUP BY', 'SELECT', 'HEX', r'\'', 'SUBSTR', '%20' ]
        else:
            self.keyword=keyword
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        X_transformed = []
        for p in X:
            p=p.upper()
            count=0
            length=0
            for key in self.keyword:
                count+=p.count(key)
                #p=p.replace(key, '&')
            length=len(p)
            X_transformed.append([count, length])
        return np.array(X_transformed)

    
if __name__=='__main__':
    DataSet=Load_data()
    DataSet.loadFromtxt('sqli_normal.txt', 'sqli.txt')
    X_train, X_test, y_train, y_test=DataSet.get_separate_datasets()
    transformer=TransformData()
    a=transformer.transform(X_train)
    print(a)


        

