# Sistemas Operativos 2024

## Laboratorio 1: MyBash

EL proyecto se basa en codificar un shell al estilo de bash (Bourne Again SHell) al que llamaremos mybash. El programa
debe tener las siguientes funcionalidades generales: 

## Integrantes

- **Madero Ismael**
- **Castillo Agustin**: Pruebas de command.c
- **Gonzalez Juan Pablo**: Pruebas de parsing.c
- **Pellegrino Milena**: Pruebas para todos los módulos juntos

## Requisitos

- **sudo apt-get install check**: Se da una test-suite implementada con check a fin de comprobar que la implementación dada tiene alguna parte de la funcionalidad esperada.
- **make test-command**: Pruebas de command.c
- **make test-parsing**: Pruebas de parsing.c
- **make test**: Pruebas para todos los módulos juntos
- **make memtest**:Pruebas de manejo de memoria en los módulos:

- **make**: Compilar el proyecto 
- **./mybash**: Ejecutar el proyecto


## Puntos Estrella

1. **Obfuscated**

   Pudimos solucionar el problema del obfuscated. Del enunciado extra. 
  
2. **Prompt relevante**

   Imprimimos un prompt relevante con nombre de usuario, ruta, utilizando diferentes colores, entre otras cosas. 
   
3. **&& a medias**

   No terminamos de implementar el && (para una cantidad arbitraria) sin embargo, funciona como si fueran ; 
   