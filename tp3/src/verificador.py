# -*- coding: utf-8 -*-
import sys, os, glob

def leer_archivo(filename):
    print "Probando: " + filename
    lines = [line.rstrip('\n') for line in open(filename)]
    i = 0
    for line in lines:
        sys.stdout.write(".")
        if not (all(c == line[0] for c in line)):
            print u"\nFalla en la línea " + str(i) + u" \u2717"
            return False
        i += 1

    print u"\nTest pasado con éxito \u2713"
    return True

def main():
    archivos = glob.glob("tests/*.tst")
    tests = True
    print "Empezando a testear..."
    for archivo in archivos:
        tests = tests and leer_archivo(archivo)

    if tests:
        print "Todos los tests fueron pasados con éxito! :)"
    else:
        print "Algún test falló :("

if __name__ == '__main__':
	main()
