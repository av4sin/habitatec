# HABITATEC

> **Conexión obligatoria del DHT11:** coloca una resistencia de **10 kOhm** entre `VCC` y `DATA` (pull-up). Conecta `VCC` a 3.3 V, `GND` a GND y `DATA` al GPIO 4 (`DHT_PIN`). No conectes el sensor sin esta resistencia si tu módulo no la incorpora.

**Autor:** Gonzalo Mondragón Báscones (av4sin)
**Versión:** `2.3.2`

Consulta la [wiki del proyecto](https://av4sin.github.io/habitatec/) para ver el montaje, el funcionamiento, el protocolo BLE, la configuración y la resolución de problemas.

## Publicar la web

La web se genera desde los archivos Markdown mediante Jekyll y se publica con GitHub Pages. Para ponerla en línea:

1. Sube estos cambios a la rama `main` del repositorio `av4sin/habitatec`.
2. En GitHub abre `Settings > Pages` y selecciona `GitHub Actions` como fuente.
3. Espera a que termine `Publicar wiki en GitHub Pages`.
4. Abre `https://av4sin.github.io/habitatec/`.

El contenido de la web está en `doc/*.md` y la estructura común en `_layouts/manual.html`.

## Resumen

Habitatec es un hub de sensores ambientales para ESP32. Lee temperatura y humedad con un DHT11, conserva diez muestras de temperatura, calcula un valor filtrado cada 30 segundos y lo publica mediante Bluetooth Low Energy (BLE).

## Material necesario

- Placa ESP32 compatible con Arduino IDE.
- Sensor DHT11.
- Resistencia de 10 kOhm para el pull-up de datos.
- Cables Dupont y protoboard.
- Un teléfono u ordenador con una aplicación BLE, por ejemplo nRF Connect.

## Cableado rápido

| DHT11 | ESP32 |
| --- | --- |
| `VCC` | `3V3` |
| `GND` | `GND` |
| `DATA` | GPIO `4` |
| Resistencia 10 kOhm | Entre `VCC` y `DATA` |

## Puesta en marcha

1. Instala Arduino IDE y el soporte de placas ESP32.
2. Instala las bibliotecas `DHT11` y `BLE` que correspondan a tu ESP32. Para el DHT11, la librería empleada se encuentra en /lib
3. Abre `src/habitatec/habitatec.ino`.
4. Selecciona la placa y el puerto serie correctos.
5. Compila y carga el sketch.
6. Abre el monitor serie a `115200 baudios`.
7. Busca el dispositivo BLE `Habitacion_Heltec` y suscríbete a las notificaciones.

## Funcionamiento

- El DHT11 se lee de forma interna cada 3 segundos.
- Se guardan las últimas 10 temperaturas.
- Si las muestras están entre dos enteros consecutivos, se transmite el punto medio; en los demás casos se transmite la media.
- La humedad transmitida es siempre la última lectura directa, sin promediar.
- Cada 30 segundos se notifican temperatura y humedad si hay un cliente BLE conectado.
- Si el DHT11 falla, se emplean valores de demostración para mantener visible el flujo de prueba.

## BLE

- Nombre anunciado: `Habitacion_Heltec`
- Servicio ambiental: `0000181A-0000-1000-8000-00805F9B34FB`
- Temperatura: `00002A6E-0000-1000-8000-00805F9B34FB`, entero con signo en centésimas de grado, little-endian.
- Humedad: `00002A6F-0000-1000-8000-00805F9B34FB`, entero sin signo multiplicado por 10, big-endian.

## Licencia

Consulta [LICENSE](LICENSE).