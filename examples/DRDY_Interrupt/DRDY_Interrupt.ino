/*
  DRDY Interrupt Example for ADS126X
  
  This example demonstrates how to use the DRDY (Data Ready) pin with interrupt mode.
  The DRDY pin goes LOW when a new conversion result is ready, triggering an interrupt.
  
  Hardware Setup:
  - Connect CS pin to Arduino pin 5
  - Connect DRDY pin to Arduino pin 2 (interrupt-capable pin)
  - Connect AIN0 and AIN1 to your analog inputs
  
  This example:
  1. Sets up the DRDY pin
  2. Enables interrupt mode
  3. Reads ADC1 with DRDY wait (using interrupt)
  4. Outputs results via Serial
*/

#include <ADS126X.h>

ADS126X adc; // create the ADS126X instance

const int chip_select = 5;  // Arduino pin connected to CS on ADS126X
const int drdy_pin = 2;     // Arduino pin connected to DRDY on ADS126X (must be interrupt-capable)

const int pos_pin = 0;      // ADS126X pin AIN0, for positive input
const int neg_pin = 1;      // ADS126X pin AIN1, for negative input

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect (needed for native USB)
  }
  
  Serial.println("ADS126X DRDY Interrupt Example");
  Serial.println("==============================");
  
  // Initialize the ADC with chip select pin
  adc.begin(chip_select);
  
  // Setup the DRDY pin
  adc.setDRDYPin(drdy_pin);
  
  // Set DRDY timeout to 2000ms (optional, default is 1000ms)
  adc.setDRDYTimeout(2000);
  
  // Enable interrupt mode - this attaches an interrupt on the DRDY pin
  // When DRDY goes LOW (data ready), an interrupt flag is set
  adc.enableInterruptMode();
  
  // Start continuous conversion on ADC1
  adc.startADC1();
  
  Serial.println("Setup complete. Starting readings...");
  Serial.println();
}

void loop() {
  // Read ADC1 with DRDY wait
  // This function will:
  // - Wait for the interrupt flag (set when DRDY goes LOW)
  // - Read the conversion result
  // - Clear the interrupt flag
  // - Return 0 if timeout occurs
  int32_t voltage = adc.readADC1_DRDY(pos_pin, neg_pin);
  
  // Check if timeout occurred (more reliable than checking for 0)
  if (adc.drdyTimedOut()) {
    Serial.println("Warning: DRDY timeout occurred!");
  } else {
    Serial.print("ADC1 Reading: ");
    Serial.println(voltage);
  }
  
  delay(1000); // wait 1 second between readings
}
