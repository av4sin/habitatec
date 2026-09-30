---
layout: manual
title: Formato de datos
section: Funcionamiento
description: Las características BLE entregan dos bytes por lectura. El orden y la escala cambian según el tipo de dato.
permalink: /datos.html
toc:
  - id: temperatura
    label: Temperatura
    icon: fa-temperature-half
  - id: humedad
    label: Humedad
    icon: fa-droplet
  - id: resumen
    label: Resumen
    icon: fa-list-check
previous:
  url: /ble.html
  title: Bluetooth Low Energy
next:
  url: /problemas.html
  title: Solución de problemas
---

## Temperatura
{: #temperatura }

Es un entero con signo de 16 bits en formato little-endian. El valor está expresado en centésimas de grado.

| Bytes | Interpretación | Ejemplo |
| --- | --- | --- |
| 2 bytes | `int16le / 100` | `2550` = 25.50 °C |

## Humedad
{: #humedad }

Es un entero sin signo de 16 bits en formato big-endian, multiplicado por 10.

| Bytes | Interpretación | Ejemplo |
| --- | --- | --- |
| 2 bytes | `uint16be / 10` | `503` = 50.3 % |

## Resumen
{: #resumen }

Temperatura: lee primero el byte menos significativo. Humedad: lee primero el byte más significativo. Ambas características se notifican cada 30 segundos.
