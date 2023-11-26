#define UART1_TXD_PIN (GPIO_NUM_10)
#define UART1_RXD_PIN (GPIO_NUM_9)
#define UART2_TXD_PIN (GPIO_NUM_17)
#define UART2_RXD_PIN (GPIO_NUM_16)
#define UART1 UART_NUM_1
#define UART2 UART_NUM_2

#define RX_BUFFER 8

void uart_config(void);
int sendData(const char* logName, uart_dev_t uart, const char* data);
void receiveData(const char* logName, uart_dev_t uart, const char* data);
