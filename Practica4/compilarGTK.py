#!/usr/local/bin/python3.7

from sys import argv
from subprocess import call

if len(argv) > 1:
    name = argv[1]
    nameNoExt = name[:-2]
    extraParams = argv[2:]

    comando = (
        f'gcc -o {nameNoExt} {name} {" ".join(extraParams)} '
        '`pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`')

    print(f'Ejecutando: {comando}')
    call(['/bin/bash', '-c', comando])
else:
    print('No se proporcionó un nombre de archivo para compilar')
