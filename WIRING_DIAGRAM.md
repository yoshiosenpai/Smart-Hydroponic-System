# Smart Plant Monitor — Full Wiring Diagram

## Schematic Overview

```
                              POWER SUPPLY
                    ┌─────────────────────────────┐
                    │  18650 Battery Pack (2S)    │
                    │  ~7.4V output               │
                    │  ┌──────┐ ┌──────┐         │
                    │  │ BAT1 │ │ BAT2 │         │
                    │  └──┬───┘ └──┬───┘         │
                    │     │ +      │ -           │
                    │     └───┬────┘             │
                    │         │                  │
                    │  (Use 5V buck converter    │
                    │   or USB power for dev)    │
                    └─────────┬──────────────────┘
                              │
                    +5V ──────┼───────────────────────────────────────┐
                    GND ──────┼───────────────────────────────────────┤
                              │                                       │
                              │                                       │
┌─────────────────────────────┴───────────────────────────────────────┴─────────────────────────┐
│                              ESP32 EXPANSION BOARD                                                │
│  ┌─────────────────────────────────────────────────────────────────────────────────────────┐   │
│  │                                                                                          │   │
│  │   3V3 ────────┬────────────────────────────────────────────────────────────────────┐    │   │
│  │               │                                                                    │    │   │
│  │   5V (Vin) ◄──┼── From USB or battery (5V regulator)                               │    │   │
│  │               │                                                                    │    │   │
│  │   GND ────────┼────────────────────── Common Ground ──────────────────────────────┤    │   │
│  │               │                                                                    │    │   │
│  │   GPIO 2 ─────┼──────────────────────────────► RELAY IN (Pump control)             │    │   │
│  │   GPIO 4 ◄────┼────────────────────────────── PIR OUT (Motion detect)              │    │   │
│  │   GPIO 15 ◄───┼────────────────────────────── DHT22 DATA                           │    │   │
│  │   GPIO 21 ────┼────────────────────────────── LCD SDA (I2C)                        │    │   │
│  │   GPIO 22 ────┼────────────────────────────── LCD SCL (I2C)                        │    │   │
│  │                                                                                     │    │   │
│  └────────────────────────────────────────────────────────────────────────────────────┘   │   │
└──────────────────────────────────────────────────────────────────────────────────────────────┘
```

---

## Component-by-Component Wiring

### 1. DHT22 Sensor

```
     DHT22 Module
    ┌─────────────┐
    │   [Sensor]  │
    │   ┌───┐     │
    │   │ ● │     │
    │   └─┬─┘     │
    │  ┌──┴──┐    │
    │  │ 1 2 3│   │   Pin 1: VCC  (3.3V or 5V)
    │  └──┬──┘    │   Pin 2: DATA (to GPIO 15)
    │     │       │   Pin 3: GND
    └─────┼───────┘
          │
    ┌─────┴─────┐
    │           │
    ▼           ▼
  GPIO15      3.3V
  (ESP32)     (ESP32)
    │           │
    │           └──► DHT22 Pin 1 (VCC)
    │
    └──► DHT22 Pin 2 (DATA)

  DHT22 Pin 3 (GND) ──► GND (ESP32)

  Optional: 4.7kΩ–10kΩ resistor between DATA and VCC
```

**Connection table — DHT22:**

| DHT22 Pin | Wire Color (Typical) | Connect To |
|-----------|----------------------|------------|
| 1 (VCC)   | Red                  | ESP32 3.3V |
| 2 (DATA)  | Yellow/White         | ESP32 GPIO 15 |
| 3 (GND)   | Black                | ESP32 GND |

---

### 2. PIR Motion Sensor (HC-SR501)

```
     PIR Sensor
    ┌─────────────────────┐
    │   [Fresnel Lens]    │
    │                     │
    │  ┌─────┬─────┬─────┐│
    │  │ VCC │ OUT │ GND ││   VCC  → 5V (or 3.3V)
    │  └──┬──┴──┬──┴──┬──┘│   OUT  → GPIO 4
    └─────┼─────┼─────┼───┘   GND  → GND
          │     │     │
          ▼     ▼     ▼
         5V   GPIO4  GND
        (ESP32)(ESP32)(ESP32)
```

**Connection table — PIR:**

| PIR Pin | Connect To |
|---------|------------|
| VCC     | 5V (ESP32 Vin or 5V rail) |
| OUT     | ESP32 GPIO 4 |
| GND     | GND |

---

### 3. 5V Relay Module

```
     5V Relay Module
    ┌──────────────────────────────┐
    │  [Relay]      [LED] [Opto]   │
    │                              │
    │  VCC  GND  IN    COM  NO  NC │
    │   │    │   │      │   │   │  │
    └───┼────┼───┼──────┼───┼───┼──┘
        │    │   │      │   │   │
        ▼    ▼   ▼      │   │   │
       5V   GND GPIO2   │   └───┴──► Pump circuit (see below)
              (ESP32)   │
                        │
                        └──► To pump power (see Water Pump section)
```

**Connection table — Relay:**

| Relay Terminal | Connect To |
|----------------|------------|
| VCC            | 5V |
| GND            | GND |
| IN             | ESP32 GPIO 2 |
| COM            | Battery (+) — common contact |
| NO             | Pump (+) — normally open (pump ON when relay energized) |
| NC             | Not used (leave disconnected) |

---

### 4. 16×2 I2C LCD

```
     16×2 LCD (I2C Backpack)
    ┌─────────────────────────────┐
    │  ┌─────────────────────┐    │
    │  │     LCD Display     │    │
    │  │  (16 cols × 2 rows) │    │
    │  └─────────────────────┘    │
    │  [I2C Backpack - 4 pins]    │
    │   GND  VCC  SDA  SCL        │
    │    │    │    │    │         │
    └────┼────┼────┼────┼─────────┘
         │    │    │    │
         ▼    ▼    ▼    ▼
        GND  5V  GPIO21 GPIO22
              (ESP32)
```

**Connection table — LCD:**

| LCD Pin | Connect To |
|---------|------------|
| GND     | GND |
| VCC     | 5V |
| SDA     | ESP32 GPIO 21 |
| SCL     | ESP32 GPIO 22 |

**Note:** If your LCD has 16 pins (parallel), you need a different wiring and code. This diagram is for I2C version.

---

### 5. DC Water Pump (via Relay)

```
    Battery + (5–12V)  ────┬────────────┐
                           │            │
                           │   RELAY    │
                           │  ┌───────┐ │
                           └──┤  COM  │ │
                              │   NO  ├─┼──────► Pump (+) 
                              └───────┘ │
                                        │
    Battery - (GND)  ───────────────────┼──────► Pump (-)
                                        │
                                        └──────► GND (common)
```

**Pump wiring:**

| Pump Wire | Connect To |
|-----------|------------|
| Pump (+)  | Relay NO (Normally Open) |
| Pump (-)  | Battery negative (GND) |

| Relay COM | Connect To |
|-----------|------------|
| Relay COM | Battery positive (+) |

**Important:** The relay switches the positive supply to the pump. When GPIO2 is HIGH, the relay closes COM–NO and the pump runs. Use a power supply (battery/buck converter) that matches your pump voltage (e.g., 5V or 12V).

---

## Complete Connection Summary

| From | To |
|------|-----|
| **ESP32 3.3V** | DHT22 VCC |
| **ESP32 GND** | DHT22 GND, PIR GND, Relay GND, LCD GND, Battery (−) |
| **ESP32 5V** | PIR VCC, Relay VCC, LCD VCC (or use external 5V) |
| **ESP32 GPIO 2** | Relay IN |
| **ESP32 GPIO 4** | PIR OUT |
| **ESP32 GPIO 15** | DHT22 DATA |
| **ESP32 GPIO 21** | LCD SDA |
| **ESP32 GPIO 22** | LCD SCL |
| **Battery (+)** | Relay COM |
| **Relay NO** | Pump (+) |
| **Battery (−)** | Pump (−) |

---

## Breadboard Layout (Suggested)

```
    +5V Rail  ═══════════════════════════════════
    GND Rail  ═══════════════════════════════════

    [ESP32]     [DHT22]    [PIR]     [Relay]    [LCD]
    
    Place ESP32 on one side, sensors on the other.
    Use jumper wires to connect per tables above.
    Keep pump and battery connections on relay
    terminals (screw terminals if available).
```

---

## Pin Reference Card (ESP32)

```
    ESP32 Expansion Board Pinout (relevant pins)
    
        3V3  │  │  GND
        15   │  │  GND      ← DHT22 DATA
         2   │  │  GPIO 4   ← Relay IN, PIR OUT
        21   │  │  GPIO 22  ← LCD SDA, LCD SCL
        5V   │  │  GND
```

---

## Power Considerations

- **ESP32:** 3.3V logic, typically powered via USB (5V) or Vin (5V).
- **18650 (2S):** ~7.4V — use a 5V buck converter for ESP32 and 5V modules.
- **Pump:** Match voltage to pump rating; power via relay from battery or separate supply.
- **Ground:** Tie all GND points together (ESP32, battery, relay, sensors, LCD).
