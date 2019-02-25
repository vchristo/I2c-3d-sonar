/*
i2c_srf04 by Vitor Christo
vitor.christo@gmail.com
July 2014     V0.1
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version. see <http://www.gnu.org/licenses/>
 
 
 This read 5 sonar type SRF04, and sends it by I2C (left,right,front,back,alt)
 the idea is to navigate in restricted areas (indoors)
 and also help in the autolanding
 
*/

#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#include <sys/resource.h>
#include <sys/stat.h>
#include <math.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bcm2835.h>

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#define SONAR_ADD 0x71
#define ARDUINO_ADDRESS 0x12
#define SONAR1 60
#define SONAR2 61
#define SONAR3 62
#define SONAR4 63
#define SONAR5 64
#define DELAY_READ 70
int fdescriptor;
int fdescriptor1;
void initialize_sonar() {
    close(fdescriptor);
	if ((fdescriptor= open("/dev/i2c-0", O_RDWR)) < 0)   // check for that
       {
          // Open port for reading and writing
          fprintf(stderr, "Failed to open i2c bus\n");
         // exit(1);
       }
	if (ioctl(fdescriptor, I2C_SLAVE, SONAR_ADD) < 0)
       {
          fprintf(stderr, "Sonar Not Present\n");
        //  exit(1);
       }
 usleep(1000);
}
void initialize_ppm() {
    close(fdescriptor);
	if ((fdescriptor= open("/dev/i2c-0", O_RDWR)) < 0)   // check for that
       {
          // Open port for reading and writing
          fprintf(stderr, "Failed to open i2c bus\n");
         // exit(1);
       }
	if (ioctl(fdescriptor, I2C_SLAVE, ARDUINO_ADDRESS) < 0)
       {
          fprintf(stderr, "Arduino Not Present\n");
        //  exit(1);
       }
 usleep(1000);
}
uint8_t channel[]={1,128,128,128,128,128};
/*******************************************************************/
/*          Aduino i2c ppm radio                                   */
/*******************************************************************/

void writeRadio()
{ 

	initialize_ppm();

	/********************************************************/

	uint8_t dat[2];

	dat[0]=1;dat[1]=channel[0];
	write(fdescriptor,dat,2);
	usleep(10000);

	dat[0]=2;dat[1]=channel[1];
	write(fdescriptor,dat,2);
	usleep(10000);

	dat[0]=3;dat[1]=channel[2];
	write(fdescriptor,dat,2);
	usleep(10000);

	dat[0]=4;dat[1]=channel[3];
	write(fdescriptor,dat,2);
	usleep(10000);

	dat[0]=5;dat[1]=channel[4];
	write(fdescriptor,dat,2);
	usleep(10000);

	dat[0]=6;dat[1]=channel[5];
	write(fdescriptor,dat,2);
	usleep(10000);
	
	dat[0]=0x08;
	write(fdescriptor,dat,1);
	usleep(10000);
	close(fdescriptor);
    
	// printf("Servo1 %d, Servo2 %d, Servo3 %d, Servo4 %d \r",channel[0],channel[1],channel[2],channel[3]);
}

void initialize() {
    uint8_t dat[2];
    close(fdescriptor);
	if ((fdescriptor= open("/dev/i2c-0", O_RDWR)) < 0)   // check for that
       {
          // Open port for reading and writing
          fprintf(stderr, "Failed to open i2c bus\n");
         // exit(1);
       }
	if (ioctl(fdescriptor, I2C_SLAVE, SONAR_ADD) < 0)
       {
          fprintf(stderr, "Sonar Not Present\n");
        //  exit(1);
       }
     
     
	dat[0]=0; // sets register pointer to the command register (0x00)
	dat[1]=50; // command sensor to measure in "cm" (0x50)
	write(fdescriptor,dat,2);
	usleep(10000);
}


uint16_t s1=0,s2=0,s3=0,s4=0,s5=0;
int main(void){
	initialize();
	uint8_t tmp1,tmp2;
	uint8_t dat[2];
	uint8_t buf[2];
	uint16_t i=0;
	dat[0]=0;
	
	while(1){
		dat[1]=SONAR1;
		write(fdescriptor,dat,2);
		usleep(10000);		
		if (read(fdescriptor, buf, 2) == 2) {
			tmp1 = (uint8_t) buf[0];
			tmp2 = (uint8_t) buf[1];
			s1=tmp2 <<8;
			s1 |=tmp1;
		}
//		printf("Sonar1 = %d Sonar2 = %d Sonar3 = %d Sonar4 = %d Sonar5 = %d \r",s1,s2,s3,s4,s5);
//		usleep(10000);
		dat[1]=SONAR2;
		write(fdescriptor,dat,2);
		usleep(10000);		
		if (read(fdescriptor, buf, 2) == 2) {
			tmp1 = (uint8_t) buf[0];
			tmp2 = (uint8_t) buf[1];
			s2=tmp2 <<8;
			s2 |=tmp1;
		}
//		printf("Sonar1 = %d Sonar2 = %d Sonar3 = %d Sonar4 = %d Sonar5 = %d \r",s1,s2,s3,s4,s5);
//		usleep(10000);
		dat[1]=SONAR3;
		write(fdescriptor,dat,2);
		usleep(10000);		
		if (read(fdescriptor, buf, 2) == 2) {
			tmp1 = (uint8_t) buf[0];
			tmp2 = (uint8_t) buf[1];
			s3=tmp2 <<8;
			s3 |=tmp1;
		}
//		printf("Sonar1 = %d Sonar2 = %d Sonar3 = %d Sonar4 = %d Sonar5 = %d \r",s1,s2,s3,s4,s5);
//		usleep(10000);
		dat[1]=SONAR4;
		write(fdescriptor,dat,2);
		usleep(10000);		
		if (read(fdescriptor, buf, 2) == 2) {
			tmp1 = (uint8_t) buf[0];
			tmp2 = (uint8_t) buf[1];
			s4=tmp2 <<8;
			s4 |=tmp1;
		}
//		printf("Sonar1 = %d Sonar2 = %d Sonar3 = %d Sonar4 = %d Sonar5 = %d \r",s1,s2,s3,s4,s5);
//		usleep(10000);
		dat[1]=SONAR5;
		write(fdescriptor,dat,2);
		usleep(10000);		
		if (read(fdescriptor, buf, 2) == 2) {
			tmp1 = (uint8_t) buf[0];
			tmp2 = (uint8_t) buf[1];
			s5=tmp2 <<8;
			s5 |=tmp1;
		}
//	printf("                                                                       \r");
	printf("Altitude=%3.0d Left=%3.0d Back=%3.0d Right=%3.0d Front=%3.0d   indice=%d \r",s1,s2,s3,s4,s5,i);
	i++;
	if(i==1000)i=0;
//	Delay(500); 
}

return 1;
}
