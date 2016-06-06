#include <Python.h>

/* A simple vector datatype.
   Essentially a bounded array, with the length information attached.
*/
struct vector {
    long length;
    long* data;
};

void free_vector(struct vector* v) {
    free(v->data);
    free(v);
}

struct vector* new_vector(long n) {
    struct vector* v = malloc(sizeof(struct vector));
    v->length = n;
    v->data = malloc(sizeof(long) * n);
    return v;
}



/* Calculate the vector of primes <= n using the sieve of eristothonies */
struct vector* make_primes_vector(long n) {

    /* maybe_prime is a vector of booleans, tracking whether it is possible for
       a given index to be a prime, update as we sieve.
    */
    struct vector* maybe_prime = new_vector(n);
    for(long i = 0; i < maybe_prime->length; i++) {
        maybe_prime->data[i] = 1;
    }
    maybe_prime->data[0] = 0;
    maybe_prime->data[1] = 0;
    
    // We are overallocating here, but we will fix it soon.
    struct vector* primes = new_vector(n);
    long primes_idx = 0;

    /* Sieve. */
    for(long i = 0; i < maybe_prime->length; i++) {
        if(maybe_prime->data[i] != 0) {
            primes->data[primes_idx] = i;
            primes_idx++;
            for(long j = i*i; j < maybe_prime->length; j = j + i) {
                maybe_prime->data[j] = 0;
            }
        }
    }

    // Fix overallocation.
    long n_primes_found = 0;
    for(long i = 0; i < maybe_prime->length; i++) {
        n_primes_found += maybe_prime->data[i];
    }
    primes->length = n_primes_found;

    free_vector(maybe_prime);
    return primes;
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
