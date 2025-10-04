# 🏢 GS Attendance IoT Integration (PN532)

Complete IoT solution for GS (Grama Sevaka) attendance tracking using ESP32 microcontroller and PN532 NFC technology.

## 📋 Project Overview

This IoT integration enables automatic attendance tracking for Grama Sevaka officers using:
- **ESP32 microcontroller (30-pin)** for processing and WiFi connectivity
- **PN532 NFC Module** for reading RFID/NFC cards
- **Spring Boot API** integration for data synchronization
- **Minimal wiring** - only 4 connections needed
- **Optional visual and audio feedback** for user interaction

## 🔧 Hardware Components

| Component | Quantity | Purpose |
|-----------|----------|---------|
| ESP32 Development Board (30-pin) | 1 | Main microcontroller |
| PN532 NFC Module | 1 | RFID/NFC card reading |
| Breadboard | 1 | Prototyping connections |
| Jumper Wires | 4+ | Minimal connections |
| RFID/NFC Cards | Multiple | Officer identification |
| **Optional Components:** | | |
| Red LED | 1 | Error indicator |
| 220Ω Resistor | 1 | LED current limiting |
| Active Buzzer | 1 | Audio feedback |
| Old Display | 1 | Future status display |

## 🚀 Quick Start

### 1. Hardware Setup (Minimal - Only 4 wires!)
1. Connect PN532 NFC module to ESP32 following the [wiring diagram](diagrams/wiring_diagram.md)
2. Set PN532 switches: SEL0=ON, SEL1=OFF (I2C mode)
3. Optional: Add red LED and buzzer for feedback

### 2. Software Setup
1. Install Arduino IDE and ESP32 board package
2. Install required libraries (Adafruit PN532, ArduinoJson)
3. Configure your WiFi and API settings in the code
4. Upload `gs_attendance_pn532_minimal.ino` to ESP32 (recommended for beginners)

### 3. Configuration
```cpp
// Edit config.h with your settings:
const char* WIFI_SSID = "YourWiFiName";
const char* WIFI_PASSWORD = "YourWiFiPassword";
const char* API_SERVER_IP = "192.168.1.100";    // Your server IP
const String GS_OFFICE_ID = "office001";        // Office identifier
```

### 4. Testing
1. Power on ESP32 and check serial monitor
2. Verify WiFi connection (green LED blinks)
3. Test NFC card reading
4. Confirm API communication with server

## 🔌 Wiring Connections (Super Simple!)

### PN532 to ESP32 (Only 4 wires needed!)
| PN532 Pin | ESP32 Pin | Function |
|-----------|-----------|----------|
| VCC | 3.3V | Power (⚠️ NOT 5V!) |
| GND | GND | Ground |
| SDA | GPIO 21 | I2C Data |
| SCL | GPIO 22 | I2C Clock |

### Optional Components
| Component | ESP32 Pin | Notes |
|-----------|-----------|-------|
| Built-in LED | GPIO 2 | Success indicator (no resistor needed) |
| Red LED | GPIO 4 | Error indicator (+ 220Ω resistor) |
| Buzzer | GPIO 13 | Audio feedback (comment out if not used) |

### PN532 Switch Settings
- **SEL0: ON** (I2C mode)
- **SEL1: OFF**

## 🌐 API Integration

The ESP32 communicates with your Spring Boot server using HTTP POST requests:

```
POST http://your-server-ip:8080/api/gs-attendance/toggle?rfid=CARD_UID&gsOfficeId=office001
```

### Response Handling
- **200 OK**: Attendance marked successfully
- **400 Bad Request**: Invalid RFID or office ID
- **404 Not Found**: Officer or office not registered
- **500 Server Error**: Database or server issue

## 🔊 User Feedback

### Audio Signals
- **Single beep**: Card detected
- **Rising tone**: Check-in successful
- **Double beep**: Check-out successful
- **Descending tone**: Error occurred

### Visual Indicators
- **Green LED steady**: WiFi connected, system ready
- **Green LED blink**: Successful operation
- **Red LED steady**: WiFi disconnected
- **Red LED blink**: Error condition

## 📊 System Features

### ✅ Implemented Features
- [x] NFC card reading with PN532 module (I2C communication)
- [x] WiFi connectivity with auto-reconnection
- [x] HTTP API integration
- [x] Minimal wiring setup (only 4 connections)
- [x] Built-in LED feedback (no external components needed)
- [x] Optional visual and audio feedback
- [x] Error handling and retry mechanisms
- [x] Easy configuration (edit directly in code)
- [x] Debug mode with detailed logging
- [x] Card validation and duplicate prevention
- [x] Multiple office support
- [x] Network timeout handling

### 🔄 Advanced Features
- [x] Automatic WiFi reconnection
- [x] HTTP request retry logic
- [x] Card reading debouncing
- [x] System health monitoring
- [x] Production/development modes
- [x] Comprehensive error reporting

## 🔧 Troubleshooting

### Common Issues

#### ESP32 Not Detected
- Install USB drivers (CP2102 or CH340)
- Try different USB cable
- Check COM port in Device Manager

#### RC522 Not Working
- Verify 3.3V power supply (NOT 5V!)
- Check all SPI connections
- Test with multimeter for continuity

#### WiFi Connection Issues
- Ensure 2.4GHz network (not 5GHz only)
- Check SSID and password
- Move closer to router

#### API Communication Errors
- Verify server IP and port
- Test API with curl command
- Check firewall settings

## 📈 Performance Specifications

- **Reading Range**: 1-4cm (PN532 typical range)
- **Reading Speed**: ~200ms per card
- **WiFi Range**: 50-100m (depending on environment)
- **Power Consumption**: ~390mA during active scanning
- **Operating Voltage**: 3.3V
- **Operating Temperature**: 0°C to 70°C
- **Communication**: I2C (only 2 data wires needed)

## 🔒 Security Considerations

1. **Network Security**
   - Use WPA2/WPA3 WiFi encryption
   - Consider VPN for remote deployments
   - Change default passwords

2. **Data Security**
   - Server-side RFID validation
   - Encrypted communication (HTTPS)
   - Access logging and monitoring

3. **Physical Security**
   - Secure device enclosure
   - Tamper detection
   - Hidden configuration interface

## 🚀 Deployment Guide

### Development Environment
1. Use `gs_attendance_nfc.ino` for basic testing
2. Enable debug mode for detailed logging
3. Test with local server setup

### Production Environment
1. Use `gs_attendance_nfc_v2.ino` for deployment
2. Disable debug mode for performance
3. Configure production server settings
4. Implement monitoring and logging

## 📚 Documentation

- [📋 Complete Setup Guide](documentation/setup_guide.md) - Detailed installation instructions
- [🔌 Wiring Diagram](diagrams/wiring_diagram.md) - Hardware connection guide
- [📖 Library Requirements](esp32-code/libraries_required.md) - Software dependencies
- [⚙️ Configuration Reference](esp32-code/config.h) - Settings and options

## 🤝 Contributing

1. Fork the repository
2. Create feature branch (`git checkout -b feature/improvement`)
3. Commit changes (`git commit -am 'Add feature'`)
4. Push to branch (`git push origin feature/improvement`)
5. Create Pull Request

## 📝 License

This project is part of the GNMS (Grama Niladhari Management System) and follows the same licensing terms.

## 📞 Support

For technical support:
1. Check troubleshooting section
2. Review documentation
3. Create GitHub issue
4. Contact development team

---

## 🎯 Next Steps

1. **Hardware Assembly**: Follow the [wiring diagram](diagrams/wiring_diagram.md)
2. **Software Setup**: Install Arduino IDE and libraries
3. **Configuration**: Update `config.h` with your settings
4. **Testing**: Verify each component works correctly
5. **Deployment**: Install in production environment

## 🎯 Quick Setup for Your Components

Since you have:
- ESP32 (30-pin) ✓
- PN532 NFC Module ✓  
- Old Display (for future use) ✓
- Breadboard ✓
- Jumper Wires ✓

### Immediate Setup Steps:
1. **Wire PN532 to ESP32** (only 4 wires needed!)
2. **Set PN532 switches**: SEL0=ON, SEL1=OFF
3. **Install libraries**: Adafruit PN532, ArduinoJson
4. **Edit code**: Change WiFi credentials and server IP in `gs_attendance_pn532_minimal.ino`
5. **Upload and test!**

### Recommended Starting File:
Use `gs_attendance_pn532_minimal.ino` - it's designed specifically for minimal components like yours!

**Ready to start? Begin with the [Complete Setup Guide](documentation/setup_guide.md)! 🚀**
