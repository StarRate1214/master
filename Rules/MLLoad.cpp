#include "RuleEngine.h"


bool CRuleEngine::ml(std::string module)
{
	const char *cmodule = module.c_str();
	PyObject* pName, * pModule, * pFunc,* pArgs;
	PyObject* pValue;
	long Pyresult;
	
	PyRun_SimpleString("import sys\n"
						"sys.path.append('/home/jwh/master/Python')\n");	
	pName = PyUnicode_DecodeFSDefault(cmodule);

	pModule = PyImport_Import(pName);
	Py_DECREF(pName);
	if (pModule != NULL) {
		pFunc = PyObject_GetAttrString(pModule, cmodule);
		/* pFunc is a new reference */
		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(1);
			pValue = PyByteArray_FromStringAndSize(getPayload(), (Py_ssize_t)(getPacket().data_payload_size));
			if (!pValue) {
				Py_DECREF(pArgs);
				Py_DECREF(pModule);
				perror("Python error");
				syslog(LOG_INFO | LOG_LOCAL0,"[Python Error]Cannot convert Python argument\n");
				return false;
			}
			/* pValue reference stolen here: */
			PyTuple_SetItem(pArgs, 0, pValue);
			pValue = PyObject_CallObject(pFunc, pArgs);
			//리턴값 저장됨
			Py_DECREF(pArgs);
			if (pValue != NULL) {
				Pyresult=PyLong_AsLong(pValue); //모듈 리턴값 저장
				Py_DECREF(pValue);
			}
			else {
				Py_DECREF(pFunc);
				Py_DECREF(pModule);
				perror("Python error");
				syslog(LOG_INFO | LOG_LOCAL0,"[Python Error]Call failed Python Return value\n");	
				return false;
			}
		}
		else {
			perror("Python error");
			syslog(LOG_INFO | LOG_LOCAL0,"[Python Error]Cannot find Python function\n");
			return false;	
		}
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
	}
	else {
		perror("Python error");
		syslog(LOG_INFO | LOG_LOCAL0,"[Python Error] Failed to load Python Module\n");	
		return false;	
	}
	
	return Pyresult; //모듈 결과값 리턴
}