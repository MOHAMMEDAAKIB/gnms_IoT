# ESP32 + PN532 NFC Module Wiring Diagram

## Components Required
- ESP32 Development Board (30 pins)
- PN532 NFC/RFID Module
- Old Display (Optional - for future use)
- Breadboard and Jumper Wires
- RFID Cards (MIFARE 1K)
- Optional: LED (Red) and Buzzer

## Wiring Connections

### PN532 NFC Module to ESP32 (I2C Mode)
```
PN532 Pin    →    ESP32 Pin    →    Function
-----------------------------------------
VCC (3.3V)   →    3.3V         →    Power Supply
GND          →    GND          →    Ground
SDA          →    GPIO 21      →    I2C Data Line
SCL          →    GPIO 22      →    I2C Clock Line
```

**Note**: PN532 module switches must be set for I2C mode:
- Switch 1: ON (I2C mode)
- Switch 2: OFF
- Some modules have SEL0=ON, SEL1=OFF for I2C

### Additional Components (Optional)
```
Component    →    ESP32 Pin    →    Function
-----------------------------------------
Built-in LED →    GPIO 2       →    Success Indicator (Green)
Red LED      →    GPIO 4       →    Error Indicator (Optional with 220Ω resistor)
Buzzer       →    GPIO 13      →    Audio Feedback (Optional)
Button       →    GPIO 0       →    Manual Reset/Config (Optional)
Old Display  →    TBD          →    Future Status Display (Not wired yet)
```

## Visual Wiring Diagram (ASCII)

```
                    ESP32 Development Board (30-pin)
                   ┌─────────────────────────┐
                   │  ┌─┐               ┌─┐  │
              3.3V │  │ │    ESP32      │ │  │ GND
                ┌──┼──┤ │               │ │──┼──┐
                │  │  │ │               │ │  │  │
                │  │  └─┘               └─┘  │  │
                │  │                         │  │
                │  │ GPIO21  GPIO22  GPIO2   │  │
                │  ├──┬────────┬────────┬───┼──┤
                │  │  │        │        │   │  │
                │  │  │        │        │   │  │
                │  │ GPIO4   GPIO13      │   │  │
                │  ├──┬────────┬────────────┼──┤
                │  │  │        │            │  │
                │  └──┼────────┼────────────┼──┘
                │     │        │            │
                │     │        │            │
         ┌──────┴─────┼────────┼────────────┼─────┐
         │            │        │            │     │
         │            │        │            │     │
         │       ┌────┴─┐   ┌──┴──┐    ┌────┴─┐   │
         │       │ RED  │   │BUZZ │    │BUILT │   │
         │       │ LED  │   │(OPT)│    │ IN   │   │
         │       │(OPT) │   │     │    │ LED  │   │
         │       └──────┘   └─────┘    └──────┘   │
         │                                        │
         │          PN532 NFC Module              │
         │     ┌─────────────────────────┐        │
         │ VCC │ ○ ○ ○ ○                 │        │
         └─────┤                         │        │
               │     [ANTENNA COIL]      │        │
          ┌────┤                         │        │
          │GND │ ○ ○ ○ ○                 │        │
          │    └─┬─┬─┬─┬─────────────────┘        │
          │      │ │ │ │                          │
          │     SDA│ │ │                          │
          │       │ │ │                          │
          │      SCL│ │                          │
          │        │ │                           │
          │       IRQ│ (Not Connected)           │
          │         │                            │
          │        RST (Not Connected)           │
          │                                      │
          └──────────────────────────────────────┘

Switch Settings on PN532:
SEL0: ON (I2C Mode)
SEL1: OFF

Pin Connections Summary:
┌─────────────┬─────────────┬──────────────────┐
│ PN532 Pin   │ ESP32 Pin   │ Wire Color Rec.  │
├─────────────┼─────────────┼──────────────────┤
│ VCC         │ 3.3V        │ Red              │
│ GND         │ GND         │ Black            │
│ SDA         │ GPIO 21     │ Blue             │
│ SCL         │ GPIO 22     │ Yellow           │
└─────────────┴─────────────┴──────────────────┘

Additional Components (Optional):
┌─────────────┬─────────────┬──────────────────┐
│ Component   │ ESP32 Pin   │ Notes            │
├─────────────┼─────────────┼──────────────────┤
│ Built-in LED│ GPIO 2      │ No resistor req. │
│ Red LED     │ GPIO 4      │ + 220Ω resistor  │
│ Buzzer      │ GPIO 13     │ Active buzzer    │
│ Button      │ GPIO 0      │ Pull-up (opt.)   │
│ Old Display │ TBD         │ Future use       │
└─────────────┴─────────────┴──────────────────┘
```

## Breadboard Layout

```
     ESP32                    Breadboard                    PN532
   ┌─────────┐              ┌─────────────┐              ┌─────────┐
   │         │              │  + Power    │              │         │
   │   3.3V──┼──────────────┼──[Red Wire] ├──────────────┼──VCC    │
   │    GND──┼──────────────┼──[Blk Wire] ├──────────────┼──GND    │
   │ GPIO21──┼──────────────┼──[Blu Wire] ├──────────────┼──SDA    │  
   │ GPIO22──┼──────────────┼──[Yel Wire] ├──────────────┼──SCL    │
   │         │              │             │              │         │
   │  GPIO2──┼──────────────┼──[Built-in] │              │         │
   │  GPIO4──┼──────[220Ω]──┼──[Red LED]  │(Optional)    │         │
   │ GPIO13──┼──────────────┼──[Buzzer]   │(Optional)    │         │
   │         │              │             │              │         │
   │         │              │             │              │ SEL0=ON │
   │         │              │             │              │ SEL1=OFF│
   └─────────┘              └─────────────┘              └─────────┘
```

## Power Requirements

- **ESP32**: 3.3V @ 240mA (during WiFi transmission)
- **PN532**: 3.3V @ 150mA (active mode scanning)
- **Built-in LED**: 3.3V @ ~20mA
- **Optional Red LED**: 3.3V @ ~20mA (with 220Ω resistor)
- **Optional Buzzer**: 3.3V @ ~30mA
- **Total**: ~460mA max (can be powered via USB 2.0 - 500mA)

## Important Notes

1. **Power Supply**: Use ESP32's 3.3V output for PN532 (NOT 5V)
2. **I2C Communication**: PN532 uses I2C protocol (SDA/SCL)
3. **Switch Settings**: Set PN532 switches for I2C mode (SEL0=ON, SEL1=OFF)
4. **Antenna**: Keep PN532 antenna away from ESP32 WiFi antenna
5. **Range**: NFC reading range is 1-4cm typically
6. **Frequency**: PN532 operates at 13.56MHz
7. **Cards**: Compatible with MIFARE Classic, NFC Type 1/2/3/4
8. **Minimal Setup**: Only 4 wires needed (VCC, GND, SDA, SCL)

## Testing Connections

Before programming, verify connections with multimeter:
- Continuity test between ESP32 pins and RC522 pins
- Voltage test: 3.3V on RC522 VCC pin
- No short circuits between power and ground

## Troubleshooting

### Common Issues:
1. **No card detected**: Check I2C connections (SDA, SCL) and switch settings
2. **Module not responding**: Check power connections and I2C address (0x24)
3. **WiFi issues**: Ensure stable power supply during transmission  
4. **Intermittent readings**: Check breadboard connections and wire quality
5. **Switch settings wrong**: Ensure SEL0=ON, SEL1=OFF for I2C mode