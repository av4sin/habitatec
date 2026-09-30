---
layout: manual
title: Firmware
section: Funcionamiento
description: El sketch coordina las lecturas locales y el envío periódico de los valores por BLE.
permalink: /firmware.html
toc:
  - id: lecturas
    label: Lecturas
    icon: fa-chart-line
  - id: temperatura
    label: Temperatura enviada
    icon: fa-temperature-half
  - id: intervalos
    label: Intervalos
    icon: fa-clock
previous:
  url: /puesta-en-marcha.html
  title: Puesta en marcha
next:
  url: /ble.html
  title: Bluetooth Low Energy
---

## Lecturas
{: #lecturas }

El DHT11 se lee cada 3 segundos. Las últimas 10 temperaturas se guardan en un buffer circular. La humedad no se promedia: se conserva únicamente la última lectura recibida.

### Cuando falla el sensor

El error se muestra por el monitor serie. Para que el enlace siga siendo comprobable, el sketch usa una secuencia de valores de demostración mientras el sensor no responde.

## Temperatura enviada
{: #temperatura }

Si las muestras solo contienen dos temperaturas enteras consecutivas, como 25 y 26, se envía el punto medio: 25.5 °C. En cualquier otro caso se calcula la media aritmética.

## Intervalos
{: #intervalos }

| Acción | Intervalo |
| --- | --- |
| Lectura DHT11 | 3 segundos |
| Notificación BLE | 30 segundos |
| Muestras de temperatura | 10 valores |
