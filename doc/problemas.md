---
layout: manual
title: Solución de problemas
section: Ayuda
description: Una lista corta de comprobaciones para encontrar el fallo sin desmontar todo el proyecto.
permalink: /problemas.html
toc:
  - id: ble
    label: No aparece el dispositivo BLE
    icon: fa-bluetooth-b
  - id: dht
    label: El DHT11 da error
    icon: fa-triangle-exclamation
  - id: valores
    label: Los valores parecen antiguos
    icon: fa-clock
  - id: numeros
    label: La aplicación muestra números raros
    icon: fa-calculator
previous:
  url: /datos.html
  title: Formato de datos
next:
  url: /
  title: Descripción general
---

## No aparece el dispositivo BLE
{: #ble }

Comprueba la alimentación y que el sketch se haya cargado correctamente. Abre el monitor serie a `115200 baudios` y confirma que el dispositivo se llama `Habitacion_Heltec`.

## El DHT11 da error
{: #dht }

Revisa la resistencia de 10 kOhm entre VCC y DATA, el GPIO 4, la masa común y la longitud de los cables. Si el módulo ya incluye una resistencia, evita duplicarla sin comprobar el esquema.

## Los valores parecen antiguos
{: #valores }

La notificación está programada cada 30 segundos. La temperatura necesita reunir muestras; la humedad usa la última lectura disponible.

## La aplicación BLE muestra números raros
{: #numeros }

La temperatura usa `int16le_div100` y la humedad `uint16be_div10`. No interpretes ambos valores con el mismo orden de bytes.
