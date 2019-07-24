const int minSpeed = 100;
const int maxSpeed = 255;

void motors_init();

void motors_setModeStop();

void motors_setModeBkw();

void motors_setModeFwd();

void motors_setModeTurnRight();

void motors_setModeTurnLeft();

void motor_setSpeed(int motorIndex, int speed);

void motors_stop();

