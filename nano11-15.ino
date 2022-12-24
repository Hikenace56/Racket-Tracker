#include <MPU6050.h>
#include <Wire.h>

MPU6050 mpu;
// Serial data variables ------------------------------------------------------
//Incoming Serial Data Array
const byte kNumberOfChannelsFromExcel = 7; 
float t,x,y,z,a,b,c;

// Comma delimiter to separate consecutive data if using more than 1 sensor
const char kDelimiter = ',';    
// Interval between serial writes
const int kSerialInterval = 50;   
// Timestamp to track serial interval
unsigned long serialPreviousTime; 

char* arr[kNumberOfChannelsFromExcel];

// SETUP ----------------------------------------------------------------------
void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);  
  mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G);
  mpu.calibrateGyro();


  }
  
void checkSettings()
{
  Serial.println();
  
  Serial.print(" * Sleep Mode:            ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  
  Serial.print(" * Clock Source:          ");
  switch(mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
  }
}

// START OF MAIN LOOP --------------------------------------------------------- 
void loop()
{
  // Gather and process sensor data
  processSensors();

  // Read Excel variables from serial port (Data Streamer)
  processIncomingSerial();

  // Process and send data to Excel via serial port (Data Streamer)
  processOutgoingSerial();

// SENSOR INPUT CODE-----------------------------------------------------------
void processSensors() 
{
  t+=0.02;
  Vector rawAccel, rawGyro;
  // Read sensor  and store to a variable
  rawGyro = mpu.readRawGyro();

  a=rawGyro.XAxis;
  b=rawGyro.YAxis;
  c=rawGyro.ZAxis;
  
  rawAccel = mpu.readRawAccel();
  x=rawAccel.XAxis;
  y=rawAccel.YAxis;
  z=rawAccel.ZAxis;
  
  // Add any additional raw data analysis below (e.g. unit conversions)
  
}

// Add any specialized methods and processing code below


// OUTGOING SERIAL DATA PROCESSING CODE----------------------------------------
void sendDataToSerial()
{
  // Send data out separated by a comma (kDelimiter)
  // Repeat next 2 lines of code for each variable sent:

  Serial.print(t);
  Serial.print(kDelimiter);
  Serial.print(a);
  Serial.print(kDelimiter);
  Serial.print(b);
  Serial.print(kDelimiter);
  Serial.print(c);
  Serial.print(kDelimiter);
  Serial.print(x);
  Serial.print(kDelimiter);
  Serial.print(y);
  Serial.print(kDelimiter);
  Serial.print(z);
  
  
  Serial.println(); // Add final line ending character only once
}

//-----------------------------------------------------------------------------
// DO NOT EDIT ANYTHING BELOW THIS LINE
//-----------------------------------------------------------------------------

// OUTGOING SERIAL DATA PROCESSING CODE----------------------------------------
void processOutgoingSerial()
{
   // Enter into this only when serial interval has elapsed
  if((millis() - serialPreviousTime) > kSerialInterval) 
  {
    // Reset serial interval timestamp
    serialPreviousTime = millis(); 
    sendDataToSerial(); 
  }
}

// INCOMING SERIAL DATA PROCESSING CODE----------------------------------------
void processIncomingSerial()
{
  if(Serial.available()){
    parseData(GetSerialData());
  }
}

// Gathers bytes from serial port to build inputString
char* GetSerialData()
{
  static char inputString[64]; // Create a char array to store incoming data
  memset(inputString, 0, sizeof(inputString)); // Clear the memory from a pervious reading
  while (Serial.available()){
    Serial.readBytesUntil('\n', inputString, 64); //Read every byte in Serial buffer until line end or 64 bytes
  }
  return inputString;
}

// Seperate the data at each delimeter
void parseData(char data[])
{
    char *token = strtok(data, ","); // Find the first delimeter and return the token before it
    int index = 0; // Index to track storage in the array
    while (token != NULL){ // Char* strings terminate w/ a Null character. We'll keep running the command until we hit it
      arr[index] = token; // Assign the token to an array
      token = strtok(NULL, ","); // Conintue to the next delimeter
      index++; // incremenet index to store next value
    }
}