---
layout: manual
title: Puesta en marcha
section: Primeros pasos
description: Carga el sketch, mira el monitor serie y comprueba que el dispositivo anuncia su servicio BLE.
permalink: /puesta-en-marcha.html
toc:
  - id: preparar
    label: Preparar Arduino IDE
    icon: fa-screwdriver-wrench
  - id: comprobacion
    label: Primera comprobación
    icon: fa-terminal
previous:
  url: /montaje.html
  title: Montaje eléctrico
next:
  url: /firmware.html
  title: Firmware
---

## Preparar Arduino IDE
{: #preparar }

1. Instala Arduino IDE y el soporte de placas ESP32.
2. Instala las bibliotecas `DHT11` y `BLE` compatibles con tu núcleo ESP32.
3. Abre `src/habitatec/habitatec.ino`.
4. Selecciona la placa ESP32 y el puerto serie correctos.
5. Compila y carga el sketch.

## Primera comprobación
{: #comprobacion }

Abre el monitor serie a `115200 baudios`. Deberías ver el arranque del hub y, cuando toque, las lecturas locales del DHT11.

Busca `Habitacion_Heltec` con una aplicación BLE. Conecta con el servicio ambiental y activa las notificaciones de temperatura y humedad.
