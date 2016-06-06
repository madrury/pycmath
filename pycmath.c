#include <Python.h>

/* A simple vector datatype.
   Essentially a bounded array, with the length information attached.
*/
struct vector {
    long length;
    long* data;
};

struct vector* make_primes_vector(long n) {
    struct vector* v = malloc(sizeof(struct vector));
    v->length = n;
    v->data = malloc(sizeof(long) * n);
    for(long i = 0; i < n; i++) {
        v->data[i] = i;
    }
    return v;
}

void free_vector(struct vector* v) {
    free(v->data);
    free(v);
}


/* Python methods for module. */

static PyObject* pycmath_primes(PyObject* self, PyObject *args) {
    const long n;              // Argument: we are calculating the primes <= n
    PyObject* primes;          // A python list to hold the primes we found.

    if (!PyArg_ParseTuple(args, "l", &n))
        return NULL;
    
    struct vector* primes_vector = make_primes_vector(n);

    primes = PyList_New(0);
    Py_INCREF(primes);
    for(long i = 0; i < primes_vector->length; i++) {
        PyList_Append(primes, PyLong_FromLong(primes_vector->data[i]));
    }

    free_vector(primes_vector);
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
