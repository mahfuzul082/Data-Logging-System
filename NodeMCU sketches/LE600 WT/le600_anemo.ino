  #include <Arduino.h>
  #include <U8g2lib.h>
  #ifdef U8X8_HAVE_HW_SPI
  #include <SPI.h>
  #endif
  #ifdef U8X8_HAVE_HW_I2C
  #include <Wire.h>
  #endif
  #include <ESP8266WiFiMulti.h>
  ESP8266WiFiMulti wifiMulti;
  #define DEVICE "ESP8266"
  #include <InfluxDbClient.h>
  #include <InfluxDbCloud.h>
  
  // WiFi AP SSID
  #define WIFI_SSID ""
  // WiFi password
  #define WIFI_PASSWORD ""


  #define TRIGGER_PIN 0
  
  #define INFLUXDB_URL ""
  #define INFLUXDB_TOKEN "" //ESP token
  #define INFLUXDB_ORG "" //org ID
  #define INFLUXDB_BUCKET "" //bucket name
  #define TURBINE_NAME "" //measurement
  #define LOCATION "" //tag key

  float output_voltage, wind_speed = 0.0;
  
  // Time zone info
  #define TZ_INFO "UTC6"
  //INA226 INA(0x40);
  U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
  // Declare InfluxDB client instance with preconfigured InfluxCloud certificate
  InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
  
  // Declare Data point
  Point sensor(TURBINE_NAME);

  const int analogInPin = A0;

  void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    pinMode(A0, INPUT);
    u8g2.begin();
    Serial.println("Hello!");
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font
    u8g2.setCursor(5,20);
    u8g2.print("Restarted...");
    u8g2.sendBuffer();
    Serial.println();

    Serial.println("\n Starting");
    u8g2.begin();
    Serial.println("Hello!");
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font
    u8g2.setCursor(5,20);
    u8g2.print("Restarted...");
    u8g2.sendBuffer();
    Serial.println();

    // Setup wifi
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  
    Serial.println("Connecting to wifi");
    while (wifiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
    delay(3000);
    Serial.println("connected...");
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font
    u8g2.setCursor(5,20);
    u8g2.print("connecting to wifi...");
    u8g2.sendBuffer();  
    //delay(3000);
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font
    u8g2.setCursor(5,20);
    u8g2.print("connected");
    u8g2.sendBuffer();    
    
    pinMode(TRIGGER_PIN, INPUT);
  
    // Accurate time is necessary for certificate validation and writing in batches
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
    
    // Add tags to the data point
    sensor.addTag("location", LOCATION);
  
    // Check server connection
    if (client.validateConnection()) {
      Serial.print("Connected to InfluxDB: ");
      Serial.println(client.getServerUrl());

      delay(1000);

      String query = "from(bucket: \"";
      query += INFLUXDB_BUCKET;
      query += "\") \|>range(start: -30d)\|>filter(fn: (r) => r._measurement == \"";
      query += TURBINE_NAME;
      query += "\" and r._field == \"Total\ Energy\ (kWh)\" and r.location == \"";
      query += LOCATION;
      query += "\")\|>last()";

      // Print ouput header
      Serial.print("==========\n");
      // Send query to the server and get result
      FluxQueryResult result = client.query(query);

      if(result.getError() != "") {
        Serial.print("Query result error: ");
        Serial.println(result.getError());
      }

      // Close the result
      result.close();  
      //Serial.print(totalEnergyPrev,18);
      Serial.print("\n==========\n");
      } 
      else {
      Serial.print("InfluxDB connection failed: ");
      Serial.println(client.getLastErrorMessage());
      }
      }
  
  void loop(){
  int adc = analogRead(A0);
  output_voltage = adc * (5.0 / 1023.0);
  output_voltage = 1.23*output_voltage; //voltage calibrated
  if (output_voltage <= 0.1){
    output_voltage = 0.0;
  }
    
  wind_speed = 6*output_voltage;
 
  Serial.print("anemo-V: ");
  Serial.print(output_voltage);
  Serial.print(" V");
  Serial.print("\t");
  Serial.print("wind speed: ");
  Serial.print(wind_speed);
  Serial.println(" m/s");
  //delay(500);

  // Store measured value into point
  sensor.clearFields();
  
  sensor.addField("Wind speed (m/s)", wind_speed);
  Serial.print("Writing: ");
  Serial.println(client.pointToLineProtocol(sensor));
  // Serial.println(totalEnergy, 18);

  // Write point
  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }
  
  delay(1000);  
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font
  u8g2.setCursor(0,20);
  u8g2.print("Voltage: ");
  u8g2.setCursor(70,20);
  u8g2.print(output_voltage);
  u8g2.setCursor(100,20);
  u8g2.print(" V");
  u8g2.setCursor(0,40);
  u8g2.print("Speed: ");
  u8g2.setCursor(60,40);
  u8g2.print(wind_speed);
  u8g2.setCursor(95,40);
  u8g2.print(" m/s");
  u8g2.sendBuffer();          // transfer internal memory to the display
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(10);
}
