from setuptools import setup, Extension
import time


setup(
    name='first',
    description='First C extension module',
    version=int(time.time()),
    ext_modules=[
        Extension('first', sources=['src/first.c']),
    ],
)
