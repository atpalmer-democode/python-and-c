#include <Python.h>

PyDoc_STRVAR(print_str__doc__, "Prints a string");
static PyObject *print_str(PyObject *self, PyObject *args) {
    const char *message;
    if(!PyArg_ParseTuple(args, "s", &message)) {
        return NULL;
    }
    printf("Message: %s\n", message);
    Py_RETURN_NONE;
}


PyDoc_STRVAR(print_int__doc__, "Prints an int");
static PyObject *print_int(PyObject *self, PyObject *args) {
    int value;
    if(!PyArg_ParseTuple(args, "i", &value)) {
        return NULL;
    }
    printf("int value: %d\n", value);
    Py_RETURN_NONE;
}


static PyMethodDef first_methods[] = {
    { "print_str", print_str, METH_VARARGS, print_str__doc__ },
    { "print_int", print_int, METH_VARARGS, print_int__doc__ },
    { NULL, NULL, 0, NULL },
};


static struct PyModuleDef first_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "first_module",
    .m_doc = "",
    .m_size = 0,
    .m_methods = first_methods,
};


PyMODINIT_FUNC PyInit_first(void) {
    return PyModule_Create(&first_module);
}
