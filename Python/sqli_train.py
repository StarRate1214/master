from sklearn.linear_model import LogisticRegression
from sklearn.svm import SVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import VotingClassifier
import os
import pickle
from sklearn.metrics import precision_score, recall_score
import sqli_dataSet

DataSet=sqli_dataSet.Load_data()
DataSet.loadFromtxt('sqli_normal.txt', 'sqli.txt')
X_train, X_test, y_train, y_test=DataSet.get_separate_datasets()
transformer=sqli_dataSet.TransformData()
X_test_transformed=transformer.transform(X_test)
X_train_transformed=transformer.transform(X_train)


log_clf = LogisticRegression(solver='liblinear', random_state=42)
rnd_clf = RandomForestClassifier(n_estimators=53, random_state=42)
svm_clf = SVC(C= 0.01, degree=1, gamma=5, kernel='poly', random_state=42, probability=True)
voting_clf = VotingClassifier(
    estimators=[('lr', log_clf), ('rf', rnd_clf), ('svc', svm_clf)],
    voting='soft')
voting_clf.fit(X_train_transformed, y_train)

y_pred = voting_clf.predict(X_test_transformed)
'''
log_clf = RandomForestClassifier(n_estimators=53, random_state=42)
log_clf.fit(X_train_transformed, y_train)
y_pred = log_clf.predict(X_test_transformed)
'''
print("정밀도: {:.2f}%".format(100 * precision_score(y_test, y_pred)))
print("재현율: {:.2f}%".format(100 * recall_score(y_test, y_pred)))


filename = 'sqli.ml'
pickle.dump(voting_clf, open(filename, 'wb'))
