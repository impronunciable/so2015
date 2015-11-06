#!/bin/bash

# Tests genéricos de 1 a 10 clientes
for i in {1..10}
do
    echo $(($i * 2))
    mpiexec -np $(($i * 2)) ./tp3 > /dev/null 2> "tests/test$i.tst"
done
