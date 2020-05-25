# Incidence Matrices

Este trabajo se basó en la la teoría de *Matrices de incidencia* para la recuperación de datos mediante consultas booleanas. En este caso se trabajó con seis libros sobre los cuales se realizaron las consultas de palabras.

## Formato de consultas

El formato para las consultas es mediante carácteres representativos equivalentes a los simbolos booleanos.
A continuación se listan dichas representaciones.

- AND: Es representado por la letra `A`
- OR: Es representado por la letra `O`
- NOT: Es representado por la letra `N`

### Ejemplo de consulta

Para realizar una consulta como:

    Word1 And Word2 And Word3

El formato es el siguiente:

    Word1 A Word2 A Word3

Cabe recalcar que el programa soporta parentesis para indicar las prioridades en una consulta.
De ese modo puedon existir consultas como:

    Word1 Or (Word2 And Word3)

Expresados de la siguiente forma:

    Word1 O ( Word2 A Word3 )

**Ojo:** siempre debe de haber espaios entre cada elemento de la expresioón.

# Pruebas y resultados

Para hacer las pruebas se realizaron tres consultas. El desarrollo de cada una se presenta a continuación:

## Consulta 1

Conocer los libros donde aparecen las palabras Gandalf y Acebeda.

**Query:**

    Gandalf A Acebeda

**Resultado:**

```basg
Matriz de incidencia :
            libro1  libro2  libro3  libro4  libro5  libro6
Gandalf     1       1       1       0       1       1
Acebeda     0       1       0       0       0       0

Arbol de la expresion :
    Acebeda
A
    Gandalf

Resultado :
Los libros son
        Libro 2
```

## Consulta 2

Conocer los libros donde aparecen las palabras Tirith y Frodo pero no Pelennor.

**Query:**

     Tirith A Frodo A N Pelennor

**Resultado:**

```bash
Matriz de incidencia :
                libro1  libro2  libro3  libro4  libro5  libro6
Tirith          0       1       1       1       1       1
Frodo           1       1       1       1       1       1
Pelennor        0       0       0       0       1       0

Arbol de la expresion :
        Pelennor
    N
        ~
A
        Frodo
    A
        Tirith

Resultado :
Los libros son
        Libro 2
        Libro 3
        Libro 4
        Libro 6

```

## Consulta 3

Conocer los libros donde aparecen las palabras Tirith y Frodo pero que no esten ni Saruman ni Grima.

**Query:**

    ( Tirith A Frodo ) A N ( Saruman O Grima )

**Resultado:**

```bash
Matriz de incidencia :
                libro1  libro2  libro3  libro4  libro5  libro6
Tirith          0       1       1       1       1       1
Frodo           1       1       1       1       1       1
Saruman         0       1       1       0       0       1
Grima           0       0       0       0       0       1

Arbol de la expresion :
            Grima
        O
            Saruman
    N
        ~
A
        Frodo
    A
        Tirith

Resultado :
Los libros son
        Libro 4
        Libro 5
```
