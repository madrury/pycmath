#include <Python.h>


void make_primes_array(long n, long* primes_array, long* n_primes) {
    *n_primes = 2;
    primes_array = malloc(sizeof(long) * *n_primes);
    primes_array[0] = 1;
    primes_array[0] = 2;
}

static PyObject* pycmath_primes(PyObject* self, PyObject *args) {
    const long n;              // Argument: we are calculating the primes <= n
    //long* primes_array = NULL; // A C array to hold the computed primes.
    //long n_primes;             // How many primes did we find?
    PyObject* primes;          // A python list to hold the primes we found.

    if (!PyArg_ParseTuple(args, "l", &n))
        return NULL;
    
    //make_primes_array(n, primes_array, &n_primes);

    primes = PyList_New(0);
    Py_INCREF(primes);
    for(long i = 0; i < n; i++) {
        PyList_Append(primes, PyLong_FromLong(i));
    }

    //free(primes_array);
    return primes;
}

static PyMethodDef PycmathMethods[] = {
    {"primes", pycmath_primes, METH_VARARGS,
     "Compute a list of all the primes up to an integer n."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef pycmath_module = {
   PyModuleDef_HEAD_INIT,
   "pycmath",   /* name of module */
   NULL, /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   PycmathMethods
};

PyMODINIT_FUNC PyInit_pycmath(void) {
    PyObject *m;

    m = PyModule_Create(&pycmath_module);
    if (m == NULL)
        return NULL;

    return m;
}
