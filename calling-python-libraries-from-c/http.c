#include <Python.h>


int raise_for_status(PyObject *response) {
    PyObject *meth = PyObject_GetAttrString(response, "raise_for_status");

    PyObject *args = PyTuple_Pack(0);
    PyObject *returned = PyEval_CallObject(meth, args);
    Py_DECREF(args);

    if(!returned)
        return 0;

    Py_DECREF(returned);
    return 1;
}


PyObject *requests_get(PyObject *self, PyObject *url) {
    PyObject *resp_text = NULL;
    PyObject *resp_obj = NULL;
    PyObject *get_meth = NULL;

    PyObject *req_dict = PyModule_GetDict(self);
    get_meth = PyMapping_GetItemString(req_dict, "get");

    PyObject *get_args = PyTuple_Pack(1, url);
    resp_obj = PyEval_CallObject(get_meth, get_args);
    Py_DECREF(get_args);

    if(!resp_obj)
        goto end;
    if(!raise_for_status(resp_obj))
        goto end;

    resp_text = PyObject_GetAttrString(resp_obj, "text");

end:
    Py_XDECREF(resp_obj);
    Py_XDECREF(get_meth);
    return resp_text;
}


PyObject *pylist_from_carr(int argc, const char *argv[]) {
    PyObject *list = PyList_New(argc);

    for(int i = 0; i < argc; ++i) {
        PyObject *item = PyUnicode_FromString(argv[i]);
        PyList_SetItem(list, i, item);
    }

    return list;
}


PyObject *get_url_from_args(int argc, const char *argv[]) {
    PyObject *url = NULL;
    PyObject *arglist = NULL;

    arglist = pylist_from_carr(argc, argv);
    Py_ssize_t size = PySequence_Size(arglist);
    if(size < 2) {
        PyErr_SetString(PyExc_ValueError, "Error: Must supply a URL");
        goto end;
    }
    url = PySequence_GetItem(arglist, 1);
    Py_INCREF(url);

end:
    Py_DECREF(arglist);
    return url;
}


int main(int argc, const char *argv[]) {
    PyObject *reqmod = NULL;
    PyObject *resp = NULL;
    PyObject *url = NULL;

    Py_Initialize();

    url = get_url_from_args(argc, argv);
    if(!url)
        goto end;
    reqmod = PyImport_ImportModule("requests");
    if(!reqmod)
        goto end;
    resp = requests_get(reqmod, url);
    if(!resp)
        goto end;

    PyObject_Print(resp, stdout, Py_PRINT_RAW);

end:
    Py_XDECREF(url);
    Py_XDECREF(resp);
    Py_XDECREF(reqmod);
    int exit_code = EXIT_SUCCESS;
    if(PyErr_Occurred()) {
        PyErr_Print();
        exit_code = EXIT_FAILURE;
    }
    Py_Finalize();
    return exit_code;
}
