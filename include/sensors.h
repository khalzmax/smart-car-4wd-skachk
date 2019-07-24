#include <Arduino.h>

const uint8_t NUMBER_OF_SENSORS = 4;
const uint8_t MAX_SENSOR_VALUE = 190;
const uint8_t MIN_SENSOR_DIFF = 4;

void sensors_init();

void initSensorsHistory();
void saveCountersToHistory();

void calculateAvgSensorsFromHistory();
int getTotalSensorsAvg();

void readSensors();
void printSensors();
void resetSensorsCounter();
void handleSensors();

int *getAvgSensors();
bool isSensorsHistoryFull();
