#include "include.hpp"

class I2cControl
{
    private:
        int file_i2c;
        unsigned char I2C_ADDR;
        void error(const char *msg);

    public:
        I2cControl(int i2cNumber);
        void readI2c();
        void writeByte(unsigned char address, unsigned char byte);
};

using namespace std;


I2cControl::I2cControl(int i2cBusNumber)
{
	char i2cBus[10];
    sprintf(i2cBus, "/dev/i2c-%i", i2cBusNumber);
	//----- OPEN THE I2C BUS -----
	char *filename = (char*)i2cBus;
	if ((this->file_i2c = open(filename, O_RDWR)) < 0)
	{
		error("Failed to open the i2c bus");
		exit(1);
	}
}

void I2cControl::writeByte(unsigned char address, unsigned char byte)
{
	if(address != this->I2C_ADDR)
	{
		this->I2C_ADDR = address;
		if (ioctl(this->file_i2c, I2C_SLAVE, this->I2C_ADDR) < 0)
		{
			error("Failed to acquire bus access and/or talk to slave.\n");
			exit(1);
		}
	}
	else
	{
		//----- WRITE BYTES -----
		unsigned char buffer[60] = {0};
		buffer[0] = byte;
		int length = 1;			//<<< Number of bytes to write
		if (write(this->file_i2c, buffer, length) != length)		//write() returns the number of bytes actually written
		{
			error("write_i2c Failed to write to the i2c bus.\n");
		}
	}
}

void I2cControl::readI2c()
{
	//----- READ BYTES -----
	unsigned char buffer[60] = {0};
	int length = 4;			//<<< Number of bytes to read
	if (read(this->file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read
	{
		error("readI2c Failed to read from the i2c bus.\n");
	}
	else
	{
		printf("Data read: %s\n", buffer);
	}
}

void I2cControl::error(const char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}
