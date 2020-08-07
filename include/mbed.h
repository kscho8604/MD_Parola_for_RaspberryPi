#ifndef MBED_H
#define MBED_H


// Useful C libraries
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <sys/time.h>
#include <iostream>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

typedef uint8_t PinName;

#define SPI_MOSI 0
#define NC 0
#define SPI_SCK 0

#define nullptr 0

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define MAX(a,b) (a>b ? a:b)

#define EXCEPTION printf

#define SET_IOCTL_VALUE(FD, CTRL, VALUE)                                       \
  retval = ioctl(FD, CTRL, &(VALUE));                                          \
  if (retval == -1) {                                                          \
    EXCEPTION("Unable to set " #CTRL);                                         \
    return;                                                                    \
  }


inline uint32_t millis()
{
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

class Print {
};
#define F(s) s
class SERIAL {
	public:
	SERIAL() {};
	~SERIAL() {};
	void begin(int baudrate) {};
	void println(const char* str) { printf("%s\n", str ); };
	void print(const char* str) { printf("%s", str); };
	void print(int value) { printf("%d", value); };
	void println(int8_t value) { printf("%d\n", value); };
	char read() { char c; std::cin.get(c); return c; };
	bool available() { return true; };
};

static SERIAL Serial;

class SPI
{
private:
  int m_fd;
  uint32_t m_mode;
  uint32_t m_max_speed;
  uint16_t m_delay;
  uint8_t m_bits_per_word;
public:
  uint8_t dataPin;
  SPI() : m_fd(-1),
    m_mode(0),
    m_max_speed(2000000),  // default speed in Hz () 1MHz
    m_delay(0),            // expose delay to options
    m_bits_per_word(8) { } // default bits per word

  ~SPI() {
    if(m_fd)
    {
    //  printf("close m_fd = 0x%x\n", m_fd);
      close(m_fd);
    }
  }

  SPI(uint8_t mosi, int nc, uint8_t clk) {
    int retval = 0;

    m_mode = SPI_MODE_0|SPI_CS_HIGH;
    m_mode &= ~SPI_NO_CS;
    m_max_speed = 2000000;
    m_delay = 0;
    m_bits_per_word = 8;
/*
    printf("m_mode %d\n", m_mode);
    printf("m_max_speed %d\n", m_max_speed);
    printf("m_delay %d\n", m_delay);
    printf("m_bits_per_word %d\n", m_bits_per_word);*/

    m_fd = open("/dev/spidev0.0", O_RDWR);
    if(m_fd < 0) {
      EXCEPTION("Unable to open device");
      return;
    }

    SET_IOCTL_VALUE(m_fd, SPI_IOC_WR_MODE, m_mode);
    SET_IOCTL_VALUE(m_fd, SPI_IOC_WR_BITS_PER_WORD, m_bits_per_word);
    SET_IOCTL_VALUE(m_fd, SPI_IOC_WR_MAX_SPEED_HZ, m_max_speed);
  }
  int write(const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length)
  {
    struct spi_ioc_transfer data = {
      (unsigned long)tx_buffer,
      (unsigned long)rx_buffer,
      (uint32_t)(MAX(tx_length, rx_length)),
      m_max_speed,
      m_delay, // Still unsure ... just expose to options.
      m_bits_per_word
    };

    int ret = ioctl(m_fd, SPI_IOC_MESSAGE(1), &data);

    if (ret == -1) {
      EXCEPTION("Unable to send SPI message");
      return -1;
    }
/*  printf("write \n ");
    printf("tx_length 0x%x \n", tx_length);
    for(int i = 0; i< tx_length; i++)
    printf("0x%x ", tx_buffer[i]);*/
    return 0;
  }
};


class DigitalOut
{
public:
    DigitalOut() {}
    ~DigitalOut() {}

    DigitalOut(uint8_t cs) { }
};

#define HIGH 1
#define LOW 0
#define INPUT_PULLUP 0

static int8_t digitalRead(int8_t pin) { return 0;  }
static void pinMode(int8_t pin, int8_t mode) { }
static long random(int min, int max) { static bool first = true; if(first) { srand(time(NULL)); first = false; } return min+rand()%((max+1) - min); }
static long random(int range) { random(0, range); }
#endif
