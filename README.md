# **Programación de microcontroladores**

By: Ramiro Sanes
IDE and MCU: STM32 Cube IDE - STM32F429ZI

## Table of contents
- [SemanaDos](#SemanaDos)
- [SemanaTres](#SemanaTres)
- [SemanaCuatro](#SemanaCuatro)

## SemanaDos
Se implementa un programa que:

Utiliza retardos no bloqueantes para parpadear los LEDs de la placa de desarrollo:

- 100 ms encendido
- 100 ms apagado 

o (cuando el voton de usuario esté pulsado)

- 5 veces con período 1 segundo y ciclo de trabajo 50%.
- 5 veces con período 200 ms y ciclo de trabajo 50%.
- 5 veces con período 100 ms y ciclo de trabajo 50%. 

## SemanaTres

Se implementa un programa que tiene tres tareas a pollear constantemente:

- Chequear State, y actuar según este estado
- Updatear delay On, cambiar State segun este update
- Updatear delay Cycle, cambiar delays e indexes según este update

Se utilizan las funciones construidas en las semanas anteriores, y se comienzan a modularizar e importar mediante #include

## SemanaCuatro

Se implementa en nuestro programa una Maquina de Estado Finita (FSM), para realizar anti-rebotes del pulsador de usuario mediante software. además se utiliza otra FSM para el manejo de los LEDs. Se combinan ambas funcionalidades para obtener un programa que realiza las mismas tareas que la semana tres pero exclusivamente mediante FSMs.
