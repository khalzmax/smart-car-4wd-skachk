
const int minSpeed = 100;
const int maxSpeed = 255;

int motorsSpeedCorrection[] = {0, 0, 0, 0};
const uint8_t MOTORS_SPEED_CORRECTION_VALUE = 5;

void motors_setModeStop();

void motors_setModeBkw();

void motors_setModeFwd();

void motors_setModeTurnRight();

void motors_setModeTurnLeft();

void motors_setSpeed(int speed);

void motors_stop();

void motors_correctMotorSpeed(byte motorIndex, int value);

void motors_initialCorrections();