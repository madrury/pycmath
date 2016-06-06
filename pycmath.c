#include <Python.h>
#include <math.h>

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
       a given index to be a prime, updated as we sieve.
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

/* Calcualte the continued fraction representation of a quadratic irrationality. 

   The return value is a two-tuple (Int, List[Int]), with the first coordinate
   the integer part of the result, and the list containing the coninued
   fraction decomposition of the number.
*/
static PyObject* pycmath_coninued_fraction(PyObject* self, PyObject *args) {
    long n;
    PyObject* continued_fraction;
    PyObject* decimal_part;

    if (!PyArg_ParseTuple(args, "l", &n))
        return NULL;

    double square_root = sqrt(n);
    long integer_part = (long)square_root;

    continued_fraction = Py_BuildValue("(l,[])", integer_part);
    decimal_part = PyTuple_GetItem(continued_fraction, 1); // Borrow.

    /* Iteravely compute the decimal part of the continued fraction expansion. */
    long m = 0;
    long d = 1;
    long a = integer_part;
    do {
        m = d*a - m;
        d = (n - m*m) / d;
        a = (long)( (square_root + m) / d );
        PyList_Append(decimal_part, PyLong_FromLong(a));
    } while(a != (2 * integer_part));

    return continued_fraction;
}

/* Module setup and tables. */

static PyMethodDef PycmathMethods[] = {
    {"primes", pycmath_primes, METH_VARARGS,
     "Compute a list of all the primes up to an integer n."},
    {"continued_fraction", pycmath_coninued_fraction, METH_VARARGS,
     "Compute the continued fraction expansion of a quadratic irrationality."},
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
