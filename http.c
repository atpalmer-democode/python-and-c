#include <Python.h>


PyObject *requests_get(PyObject *self, PyObject *url) {
    PyObject *response_text = NULL;
    PyObject *get_args = NULL;
    PyObject *get_result = NULL;

    PyObject *requests_dict = PyModule_GetDict(self);
    PyObject *get_meth = PyMapping_GetItemString(requests_dict, "get");

    get_args = PyTuple_Pack(1, url);
    get_result = PyEval_CallObject(get_meth, get_args);
    if(get_result == NULL) {
        goto end;
    }

    response_text = PyObject_GetAttrString(get_result, "text");

end:
    Py_XDECREF(get_result);
    Py_XDECREF(get_args);
    Py_XDECREF(get_meth);
    return response_text;
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

    PyObject *arglist = pylist_from_carr(argc, argv);
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
    PyObject *requests = NULL;
    PyObject *text = NULL;
    PyObject *url = NULL;

    Py_Initialize();

    url = get_url_from_args(argc, argv);
    if(url == NULL) {
        goto end;
    }

    requests = PyImport_ImportModule("requests");
    if(requests == NULL) {
        goto end;
    }

    text = requests_get(requests, url);
    if(text == NULL) {
        goto end;
    }

    PyObject_Print(text, stdout, Py_PRINT_RAW);

end:
    Py_XDECREF(url);
    Py_XDECREF(text);
    Py_XDECREF(requests);
    int exit_code = EXIT_SUCCESS;
    if(PyErr_Occurred()) {
        PyErr_Print();
        exit_code = EXIT_FAILURE;
    }
    Py_Finalize();
    return exit_code;
}
