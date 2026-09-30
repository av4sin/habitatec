---
layout: manual
title: Montaje eléctrico
section: Primeros pasos
description: Conecta el DHT11 al ESP32 con una resistencia de 10 kOhm en la línea de datos.
permalink: /montaje.html
toc:
  - id: conexiones
    label: Conexiones
    icon: fa-plug
  - id: comprobacion
    label: Comprobación rápida
    icon: fa-circle-check
previous:
  url: /
  title: Descripción general
next:
  url: /puesta-en-marcha.html
  title: Puesta en marcha
---

<div class="notice" id="conexiones"><div class="notice-title">Haz las conexiones con el ESP32 apagado</div>La resistencia funciona como pull-up: mantiene DATA en un nivel estable cuando el sensor no está transmitiendo.</div>

## Conexiones

| Elemento | Conectar a | Notas |
| --- | --- | --- |
| DHT11 VCC | ESP32 3V3 | Alimentación recomendada. |
| DHT11 GND | ESP32 GND | La masa debe ser común. |
| DHT11 DATA | GPIO 4 | Es el pin definido por `DHT_PIN`. |
| Resistencia | Entre VCC y DATA | 10 kOhm, pull-up. |

## Comprobación rápida
{: #comprobacion }

1. Confirma que VCC no está conectado a 5 V.
2. Comprueba que la resistencia une VCC y DATA, no DATA y GND.
3. Verifica la masa común entre sensor y placa.
4. Deja el sensor con cables cortos mientras haces la primera prueba.
