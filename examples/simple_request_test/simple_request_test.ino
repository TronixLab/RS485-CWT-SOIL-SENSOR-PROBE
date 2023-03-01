/*
  Note: 
  *Serial Port 0 is not used to connect the RS485 Converter (MAX485) because its used for debugging. 
  *The Serial Port 1 (TX1, RX1) is used for ModBus communication interface.
  *Wiring of Sensor, Arduino, and MAX485 TTL to RS485 Converter:
  ___________________________________________________________________________________________
  | Sensor (SHT20)   |   MAX485 TTL to RS485 Converter
  |  A (Yellow)      |        A (Terminal block)
  |  B (White)       |        B (Terminal block)
  |  GND (Black)     |       GND (External Supply)
  |  Vs (Red)        |      5-30V (External Supply)
  ___________________________________________________________________________________________
  | MAX485 TTL to RS485 Converter  |  Arduino (Hardware Serial)  |  Arduino (Software Serial)
  |     RO (Reciever Output)       |        D19 (RX1)            |          D9 (RX)
  |     RE (Reciever Enable)       |        D2                   |          D2
  |     DE (Driver Enable)         |        D3                   |          D3
  |     DI (Driver Input)          |        D18 (TX1)            |          D10 (TX)
  ___________________________________________________________________________________________
*/

// Define the pins used by the RS-485 module
#define MAX485_RE_NEG 2
#define MAX485_DE 3

void setup() {
  // Initialize program Serial port at 115200 baud rate
  Serial.begin(115200);

  // Initialize modbus Serial port at 4800 baud rate
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  // Start the Modbus serial Port, for Soil_sensor
  Serial1.begin(4800);
  delay(1000);
}

void loop() {
  // Read Humidity, temperature, conductivity, PH, N, P, K together
  preTransmission();
  byte Soil_sensor_request[] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x07, 0x04, 0x08 };
  Serial1.write(Soil_sensor_request, sizeof(Soil_sensor_request));
  Serial1.flush();
  postTransmission();
  byte Soil_sensor_buf[24] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  Serial1.readBytes(Soil_sensor_buf, 24);

  // Parse sensor parameters
  float humidity = word(Soil_sensor_buf[3], Soil_sensor_buf[4]);
  humidity = humidity / 10.0;

  float temperature = word(Soil_sensor_buf[5], Soil_sensor_buf[6]);
  temperature = temperature / 10.0;

  unsigned long int conductivity = word(Soil_sensor_buf[7], Soil_sensor_buf[8]);

  float pH = word(Soil_sensor_buf[9], Soil_sensor_buf[10]);
  pH = pH / 10.0;

  int Nitrogen = word(Soil_sensor_buf[11], Soil_sensor_buf[12]);
  int Phosphorus = word(Soil_sensor_buf[13], Soil_sensor_buf[14]);
  int Potassium = word(Soil_sensor_buf[15], Soil_sensor_buf[16]);
  unsigned long int Salinity = word(Soil_sensor_buf[17], Soil_sensor_buf[18]);
  unsigned long int TDS = word(Soil_sensor_buf[19], Soil_sensor_buf[20]);

  // Print sensor parameters
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  ");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C  ");
  Serial.print("Conductivity: ");
  Serial.print(conductivity);
  Serial.print("us/cm  ");
  Serial.print("pH: ");
  Serial.print(pH);
  Serial.print("  ");
  Serial.print("Nitrogen: ");
  Serial.print(Nitrogen);
  Serial.print("mg/L  ");
  Serial.print("Phosphorus: ");
  Serial.print(Phosphorus);
  Serial.print("mg/L   ");
  Serial.print("Potassium: ");
  Serial.print(Potassium);
  Serial.print("mg/L   ");
  Serial.print("Salinity: ");
  Serial.print(Salinity);
  Serial.print("dS/m   ");
  Serial.print("TDS: ");
  Serial.print(TDS);
  Serial.print("ppm  ");
  Serial.println();
  delay(3000);
}

void preTransmission() {
  digitalWrite(MAX485_RE_NEG, HIGH);
  digitalWrite(MAX485_DE, HIGH);
}

void postTransmission() {
  digitalWrite(MAX485_RE_NEG, LOW);
  digitalWrite(MAX485_DE, LOW);
}