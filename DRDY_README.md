# DRDY (Data Ready) Pin Support

The ADS126X library now supports the DRDY (Data Ready) pin of the ADS1262/ADS1263, which allows you to know exactly when new conversion data is available.

## Features

- **Pin Configuration**: Set up a DRDY pin similar to the START pin
- **Polling Mode**: Wait for DRDY to go LOW before reading data
- **Interrupt Mode**: Use hardware interrupts for efficient data-ready detection
- **Configurable Timeout**: Set a timeout for DRDY wait operations
- **Backward Compatible**: Existing `readADC1()` and `readADC2()` functions continue to work

## Basic Usage

### Setup the DRDY Pin

```cpp
#include <ADS126X.h>

ADS126X adc;

void setup() {
  adc.begin(5);  // CS pin
  adc.setDRDYPin(2);  // DRDY pin (must be digital input capable)
}
```

### Polling Mode (Default)

Use `readADC1_DRDY()` or `readADC2_DRDY()` to wait for data ready before reading:

```cpp
void loop() {
  // Read ADC1 with DRDY wait (polling mode)
  // This will poll the DRDY pin until it goes LOW (data ready)
  int32_t value = adc.readADC1_DRDY(0, 1);  // pos_pin=0, neg_pin=1
  
  // Check if timeout occurred (more reliable than checking for 0)
  if (adc.drdyTimedOut()) {
    Serial.println("Timeout!");
  } else {
    Serial.println(value);
  }
  
  delay(1000);
}
```

### Interrupt Mode

For more efficient operation, enable interrupt mode:

```cpp
void setup() {
  adc.begin(5);
  adc.setDRDYPin(2);  // Must be interrupt-capable pin
  adc.enableInterruptMode();  // Attach interrupt on falling edge
  adc.startADC1();
}

void loop() {
  // In interrupt mode, readADC1_DRDY() waits for the interrupt flag
  int32_t value = adc.readADC1_DRDY(0, 1);
  
  if (adc.drdyTimedOut()) {
    Serial.println("Timeout!");
  } else {
    Serial.println(value);
  }
  
  delay(1000);
}
```

### Manual DRDY Check

You can also manually check if data is ready:

```cpp
if (adc.isDataReady()) {
  int32_t value = adc.readADC1(0, 1);
  Serial.println(value);
}
```

## API Reference

### Configuration Functions

#### `void setDRDYPin(uint8_t pin)`
Configures a digital pin to read the DRDY signal from the ADS126X.
- **pin**: Arduino pin number connected to DRDY

#### `void setDRDYTimeout(unsigned long timeout_ms)`
Sets the timeout for DRDY wait operations.
- **timeout_ms**: Timeout in milliseconds (default: 1000ms)

### Interrupt Functions

#### `void enableInterruptMode(void)`
Enables interrupt mode by attaching an interrupt handler to the DRDY pin.
- DRDY pin must be interrupt-capable (e.g., pin 2 or 3 on most Arduino boards)
- Interrupt triggers on falling edge (DRDY goes LOW when data ready)

#### `void disableInterruptMode(void)`
Disables interrupt mode and detaches the interrupt handler.

### Reading Functions

#### `int32_t readADC1_DRDY(uint8_t pos_pin, uint8_t neg_pin)`
Reads ADC1 with DRDY wait (polling or interrupt mode).
- Waits for DRDY to go LOW before reading
- Returns 0 if timeout occurs (use `drdyTimedOut()` to distinguish from actual zero reading)
- Falls back to regular `readADC1()` if DRDY pin not configured

#### `int32_t readADC2_DRDY(uint8_t pos_pin, uint8_t neg_pin)`
Reads ADC2 with DRDY wait (polling or interrupt mode).
- Same behavior as `readADC1_DRDY()` but for ADC2

#### `bool isDataReady(void)`
Checks if data is ready by reading the DRDY pin.
- Returns `true` if DRDY is LOW (data ready)
- Returns `false` if DRDY is HIGH (data not ready) or DRDY pin not configured

#### `bool drdyTimedOut(void)`
Checks if the last DRDY read operation timed out.
- Returns `true` if the last `readADC1_DRDY()` or `readADC2_DRDY()` timed out
- Returns `false` if the last read completed successfully
- Use this to distinguish between timeout (returns 0) and actual zero reading

## Hardware Connections

Make sure to connect the DRDY pin of the ADS126X to a digital input pin on your Arduino:

```
ADS126X          Arduino
--------         --------
DRDY     ----->  Pin 2 (or other interrupt-capable pin for interrupt mode)
CS       <-----  Pin 5 (or your chosen CS pin)
START    <-----  Pin 3 (if using START pin)
```

## Examples

See the example sketches for complete working examples:

- **DRDY_Interrupt**: Demonstrates interrupt mode usage
- **DRDY_Polling**: Demonstrates polling mode usage

## Notes

- The DRDY pin goes LOW when a new conversion result is ready
- In interrupt mode, use interrupt-capable pins (typically pins 2 and 3 on Arduino Uno/Nano)
- The timeout feature prevents your program from hanging if DRDY never goes LOW
- Existing `readADC1()` and `readADC2()` functions work without any DRDY checking for backward compatibility
