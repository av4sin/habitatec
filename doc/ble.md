---
layout: manual
title: Bluetooth Low Energy
section: Funcionamiento
description: Habitatec anuncia un servicio ambiental con dos características notificables.
permalink: /ble.html
toc:
  - id: identidad
    label: Identidad del dispositivo
    icon: fa-bluetooth-b
  - id: conectar
    label: Conectarse
    icon: fa-link
previous:
  url: /firmware.html
  title: Firmware
next:
  url: /datos.html
  title: Formato de datos
---

## Identidad del dispositivo
{: #identidad }

El nombre anunciado es `Habitacion_Heltec`. Puedes inspeccionarlo con nRF Connect, LightBlue u otra aplicación BLE compatible.

| Recurso | UUID corto | Propiedades |
| --- | --- | --- |
| Servicio ambiental | `181A` | Contiene temperatura y humedad. |
| Temperatura | `2A6E` | Lectura y notificación. |
| Humedad | `2A6F` | Lectura y notificación. |

## Conectarse
{: #conectar }

1. Busca `Habitacion_Heltec`.
2. Conecta con el dispositivo.
3. Abre el servicio ambiental.
4. Activa las notificaciones de las dos características.
5. Espera el siguiente envío, que ocurre cada 30 segundos.
