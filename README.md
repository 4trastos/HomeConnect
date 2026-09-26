# HomeConnect
Sistema de automatización doméstica escrito en C, con SQLite, API, scheduler y Modbus RTU como bus de campo.

### Arquitectura objetivo

```text
                    ┌───────────────────────────┐
                    │       PANEL TÁCTIL        │
                    │                           │
                    │  Linux                    │
                    │  HomeControl C             │
                    │  SQLite                    │
                    │  API                       │
                    │  Scheduler                 │
                    │  HMI / Kiosk               │
                    │                           │
                    │  Ethernet / WiFi           │
                    │  RS-485                    │
                    └─────────────┬─────────────┘
                                  │
                                  │ MODBUS RTU
                                  │ RS-485
                                  │
                 ┌────────────────▼────────────────┐
                 │      CAJA DE CONTROL           │
                 │                                │
                 │  Fuente 24 VDC                 │
                 │  Protección                    │
                 │  RS-485                        │
                 │                                │
                 │ ┌────────┐ ┌────────┐         │
                 │ │ NODE 01│ │ NODE 02│ ...     │
                 │ │ HVAC   │ │ EA     │         │
                 │ └────────┘ └────────┘         │
                 │                                │
                 │ Relés / contactores / E/S      │
                 └──────┬────────┬────────┬───────┘
                        │        │        │
                     CASA      EA       RIEGO
```

---

# 🗺️ HOJA DE RUTA

## FASE 0 — Ingeniería y levantamiento

**Objetivo: no tocar nada de la instalación hasta conocer exactamente qué tenemos.**

### 0.1 Inventario

Documentaremos:

* calefacción
* EA
* riego
* alarma
* cámaras
* alimentación disponible
* cuadro eléctrico
* recorridos de cables
* ubicación de la pantalla
* ubicación de la caja de control

### 0.2 Ingeniería eléctrica

Para cada elemento determinaremos:

* tensión
* corriente
* tipo de señal
* contacto seco / tensión
* NO / NC
* alimentación
* protecciones necesarias
* aislamiento galvánico

### 0.3 Documentación

Crearemos nuestros propios esquemas:

```text
electric/
├── cuadro_general.pdf
├── homecontrol.pdf
├── heating.pdf
├── evaporative.pdf
└── irrigation.pdf
```

---

# FASE 1 — Plataforma de control

Aquí empezamos el equivalente doméstico de SmartGreen.

## 1.1 Sistema Linux

El panel tendrá:

```text
Linux
  │
  ├── HomeControl
  ├── SQLite
  ├── Modbus
  └── Kiosk
```

El programa principal será **C**.

## 1.2 Arquitectura software

Inicialmente:

```text
homecontrol/
│
├── src/
│   ├── main.c
│   │
│   ├── modbus/
│   ├── heating/
│   ├── evaporative/
│   ├── irrigation/
│   ├── alarm/
│   ├── cameras/
│   ├── scheduler/
│   ├── sensors/
│   ├── database/
│   ├── api/
│   └── system/
│
├── include/
├── config/
├── database/
└── tests/
```

La idea es que **cada subsistema sea independiente**.

---

# FASE 2 — Bus Modbus

Aquí estableceremos nuestro protocolo interno.

Por ejemplo:

```text
RS-485
│
├── ID 01 → HVAC
├── ID 02 → EA
├── ID 03 → Riego
├── ID 04 → Sensores
└── ID 05 → Futuro
```

Definiremos desde el principio nuestro **mapa Modbus**.

Por ejemplo:

```text
Coil 00001 → Heating ON/OFF
Coil 00002 → EA ON/OFF
Coil 00003 → EA COOL
Coil 00004 → EA HIGH
Coil 00005 → Irrigation Zone 1
...
```

Y registros:

```text
40001 → temperatura salón
40002 → temperatura exterior
40003 → target calefacción
40004 → humedad
...
```

Esto será importante porque **el software no dependerá de cómo físicamente hayamos construido cada módulo**.

---

# FASE 3 — Primer controlador físico

Construiremos el primer nodo.

Algo del estilo:

```text
┌─────────────────────────────┐
│ HOME NODE 01                │
│                             │
│ MCU                         │
│                             │
│ RS-485                      │
│                             │
│ DI × 8                      │
│ DO × 8                      │
│ AI × 4                      │
│                             │
│ 24 VDC                      │
└─────────────────────────────┘
```

Pero **no compraremos todavía la placa definitiva**.

Primero determinaremos las necesidades reales después de estudiar calefacción + EA + riego.

---

# FASE 4 — Calefacción 🔥

Primer subsistema real.

Implementaremos:

### Manual

```text
ON
OFF
```

### Programador

```text
Lunes
07:00 ON
09:00 OFF
17:00 ON
23:00 OFF
```

### Termostato

```text
TARGET = 21.0 ºC
```

con histéresis configurable.

### Modos

```text
OFF
MANUAL
PROGRAMADO
TERMOSTATO
VACACIONES
ANTIHELADA
```

Y desde la pantalla:

```text
┌──────────────────────────────┐
│         CALEFACCIÓN          │
│                              │
│       20.3 ºC                │
│                              │
│      TARGET 21.0 ºC          │
│                              │
│      🔥 ENCENDIDA            │
│                              │
│ [ MANUAL ] [ PROGRAMADO ]    │
│ [ TERMOSTATO ]               │
└──────────────────────────────┘
```

---

# FASE 5 — Evaporativo EA ❄️

Aquí entra la información que me vas a pasar ahora.

Tenemos inicialmente:

```text
POWER
 ├── OFF
 └── ON

MODE
 ├── VENTILACIÓN
 └── FRÍO

SPEED
 ├── LOW
 └── HIGH
```

Pero **antes de diseñar la electrónica necesitamos estudiar el circuito real del EA**.

Esto es importante porque quiero reproducir exactamente el funcionamiento del mando original, no hacer una aproximación.

Después tendremos desde pantalla:

```text
┌──────────────────────────────┐
│        EVAPORATIVO EA        │
│                              │
│       ● ENCENDIDO            │
│                              │
│       FRÍO                   │
│                              │
│       VELOCIDAD: HIGH        │
│                              │
│ [ ON/OFF ]                   │
│ [ FRÍO / VENTILACIÓN ]       │
│ [ LOW / HIGH ]               │
└──────────────────────────────┘
```

---

# FASE 6 — Riego 💧

Digitalizaremos el controlador analógico.

Primero:

```text
ZONE 1
ZONE 2
ZONE 3
...
```

Después:

```text
Programador
```

y finalmente podremos añadir:

```text
humedad suelo
lluvia
temperatura
caudal
```

La programación será independiente de la interfaz.

---

# FASE 7 — Sensores 🌡️

Aquí empezaremos a convertir el sistema en una verdadera instalación domótica.

Por ejemplo:

```text
SALÓN
├── temperatura
├── humedad
└── CO₂

EXTERIOR
├── temperatura
├── humedad
└── lluvia

JARDÍN
└── humedad suelo
```

Y los sensores estarán disponibles para **todos los subsistemas**.

Por ejemplo:

```text
Temperatura salón
       ↓
Termostato
       ↓
Calefacción
```

---

# FASE 8 — Alarma 🚨

Cuando me pases el modelo exacto estudiaremos qué posibilidades tenemos.

Intentaremos, por orden:

1. protocolo/documentación oficial;
2. interfaz digital existente;
3. RS-485/RS-232/Ethernet;
4. entradas/salidas;
5. contactos secos;
6. interfaz electrónica propia, si fuese necesario.

Y aquí tendremos que ser especialmente cuidadosos con la seguridad: **el sistema domótico no debe convertirse en el único elemento responsable de una función crítica de seguridad de la alarma.**

---

# FASE 9 — Cámaras 📷

Las cámaras permanecerán en la red IP.

El HomeControl actuará como interfaz:

```text
HOME CONTROL
     │
     ├── Cámara salón
     ├── Cámara jardín
     ├── Cámara entrada
     └── ...
```

Podremos integrar visualización, estado y eventualmente grabación, dependiendo de los modelos.

---

# FASE 10 — Automatizaciones

Aquí es donde el proyecto empieza a ponerse realmente interesante.

Podremos crear reglas:

```text
SI
temperatura < 19 ºC
Y
hora > 18:00
ENTONCES
calefacción = ON
```

O:

```text
SI
temperatura exterior > 28 ºC
Y
hora > 12:00
ENTONCES
EA = FRÍO + HIGH
```

O:

```text
SI
humedad_suelo > 70%
ENTONCES
NO REGAR
```

---

# FASE 11 — HMI definitiva

Cuando todo funcione, diseñaremos la interfaz final.

La pantalla principal podría ser:

```text
┌─────────────────────────────────────────────┐
│  CASA                           10:42  ☀️    │
├─────────────────────────────────────────────┤
│                                             │
│  🌡 21.3 ºC       🔥 CALEFACCIÓN            │
│                     21.0 ºC                 │
│                                             │
│  ❄ EA               OFF                     │
│                                             │
│  💧 RIEGO            PROGRAMADO             │
│                                             │
│  🚨 ALARMA           ARMADA                 │
│                                             │
│  📷 CÁMARAS                                  │
│                                             │
├─────────────────────────────────────────────┤
│  CASA   CLIMA   RIEGO   SEGURIDAD   AJUSTES │
└─────────────────────────────────────────────┘
```

Y quiero que el resultado final tenga aspecto de **producto terminado**, no de proyecto maker.

---

# FASE 12 — Caja eléctrica definitiva

Todo lo que esté relacionado con:

* relés
* contactores
* fuentes
* protecciones
* módulos Modbus
* bornas
* fusibles
* alimentación 24 V
* RS-485

irá en **una caja independiente junto al cuadro eléctrico**, como has indicado.

La pantalla solamente tendrá que recibir:

```text
230 VAC → fuente
o
24 VDC
```

según el panel que finalmente elijamos, y:

```text
Ethernet
RS-485
```

si son necesarios.

---

## 🎯 Nuestro objetivo final

Quiero que acabemos teniendo algo así:

```text
                    ┌──────────────────┐
                    │   HOME CONTROL   │
                    │                  │
                    │  LINUX + C       │
                    │  SQLite          │
                    │  MODBUS          │
                    │  HMI             │
                    └────────┬─────────┘
                             │
                         RS-485
                             │
                ┌────────────▼────────────┐
                │     CUADRO CONTROL      │
                │                          │
                │  PSU 24V                │
                │  MODBUS                 │
                │  I/O                    │
                │  RELÉS                  │
                │  PROTECCIONES            │
                └────┬─────┬─────┬────────┘
                     │     │     │
                     🔥    ❄️    💧
                   HVAC    EA   RIEGO
                     
                     │
                     ├── 🚨 ALARMA
                     │
                     ├── 📷 CÁMARAS
                     │
                     └── 🌡️ SENSORES
```
**Primero entender perfectamente el hardware, después diseñar la electrónica y finalmente programar.**
