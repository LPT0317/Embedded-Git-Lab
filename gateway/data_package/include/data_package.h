#define HEARTBEAT 1
#define DATA 2
#define RESPONSE 3

void printData(char *data);
void heartbeatPackage(uint8_t ID, int status, int signal, char* data);
void dataPackage(uint8_t ID, int capacity, int floor, int trash, char* data);
void responsePackage(uint8_t ID, int type, char* data);
void getData(const char* logName, char *data, char* json);


