#include <HomeSeerClient.h>

HomeSeerClient homeSeerClient;

const short temperatureBottomPin = A0;
const short temperatureTopPin = A1;

const int heaterBottomTempProbeId = 2444;
const int heaterTopTempProbeId = 2445;
const int heaterMonitorStatusId = 2446;

unsigned long previousAliveMillis;
const long aliveInterval = 3*60*1000L;

unsigned long previousSensorReadingsMillis;
const long sensorReadingsInterval = 3*60*1000L;

void setup()
{
    Serial.begin(9600);
    while (!Serial) continue;

    homeSeerClient.init();
}

void loop()
{
    sendAlive();
}

void checkTemperatureSensors()
{
    float bottomSensorReading;
    float topSensorReading;
    if(millis() - previousSensorReadingsMillis >= sensorReadingsInterval)
    {
        previousSensorReadingsMillis += sensorReadingsInterval;
        bottomSensorReading = analogRead(heaterBottomTempProbeId);
        topSensorReading = analogRead(heaterTopTempProbeId);
        homeSeerClient.updateHomeSeer(heaterBottomTempProbeId, bottomSensorReading);
        homeSeerClient.updateHomeSeer(heaterTopTempProbeId, topSensorReading);
    }
}

void sendAlive() {
    if(millis() - previousAliveMillis >= aliveInterval)
    {
        previousAliveMillis += aliveInterval;
        homeSeerClient.updateHomeSeer(heaterMonitorStatusId, 100);
    }
}