const int minSpeed = 100;
const int maxSpeed = 255;

void motors_setModeStop();

void motors_setModeBkw();

void motors_setModeFwd();

void motors_setModeTurnRight();

void motors_setModeTurnLeft();

void motors_setSpeed(int speed);

void motors_stop();

void motors_correctMotorSpeed(byte motorIndex, int value);

void motors_initialCorrections();

void motors_flushCorrections();

void motors_resetCorrections();
