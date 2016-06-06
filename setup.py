from distutils.core import setup, Extension

pycmath = Extension('pycmath',
                    sources = ['pycmath.c'])

setup(name = 'Pycmath',
      version = '1.0',
      description = 'C functions for math in project euler problems.',
      ext_modules = [pycmath])
