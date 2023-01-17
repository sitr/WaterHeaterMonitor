#include <HomeSeerClientESP8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define temperatureBottomPin D4
OneWire oneWire(temperatureBottomPin); 

DallasTemperature sensors(&oneWire);
HomeSeerClientESP8266 homeSeerClient;

const int heaterBottomTempProbeId = 2310;
const int heaterTopTempProbeId = 2280;
const int heaterMonitorStatusId = 2281;

unsigned long previousAliveMillis = 0;
const long aliveInterval = 3*60*1000L;

unsigned long previousSensorReadingsMillis = 0;
const long sensorReadingsInterval = 60*1000L;

void setup()
{
    Serial.begin(115200);
    while (!Serial) continue;
    Serial.println("Setting up! Please wait...");
    sensors.begin();
    homeSeerClient.init();
}

void loop()
{
    checkTemperatureSensors();
    sendAlive();
}

void checkTemperatureSensors()
{
    float topSensorReading;
    if(millis() - previousSensorReadingsMillis >= sensorReadingsInterval)
    {
        Serial.println("Reading sensor");
        previousSensorReadingsMillis += sensorReadingsInterval;
        sensors.requestTemperatures();
        float bottomSensorReading = sensors.getTempCByIndex(0);
        if(bottomSensorReading != DEVICE_DISCONNECTED_C) 
        {
            Serial.print("Temp bottom: ");
            Serial.println(bottomSensorReading);
            // topSensorReading = analogRead(heaterTopTempProbeId);
            homeSeerClient.updateHomeSeer(heaterBottomTempProbeId, bottomSensorReading);
            // homeSeerClient.updateHomeSeer(heaterTopTempProbeId, topSensorReading);
        }
        else
        {
            Serial.println("Error: Could not read temperature data");
        }
    }
}

void sendAlive() {
    if(millis() - previousAliveMillis >= aliveInterval)
    {
        previousAliveMillis += aliveInterval;
        Serial.println("Alive");
        homeSeerClient.updateHomeSeer(heaterMonitorStatusId, 100);
    }
}