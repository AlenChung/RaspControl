／／newtesting0328-2
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <syslog.h>
#include <sys/wait.h>
#include <signal.h>
#include "rfid.h"
#include "bcm2835.h"
#include "config.h"
#include <iostream>
#include <linux/i2c-dev.h>  //read(address,start,how_long,save_space) //write(address,start,how_long,save_space)
#include <fcntl.h>
#include <mysql/mysql.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wiringPiI2C.h> 

int err;
const char *usercode[3]={"b2911f2b","008fb4d5","c43ae820"};
const char primary[2]={user1,user2};
const int flag[2]={0,1};//0 for in, 1 for out

#define DATABASE_NAME  "jeffDB" //資料庫名稱
#define DATABASE_USERNAME "jeff"  //mysql帳號
#define DATABASE_PASSWORD "1234"//mysql密碼

#define IPaddress "120.126.8.201"//mysql_ip

using namespace std;
MYSQL *mysql1;

#define address 0x71

void mysql_connect (void)   // 資料庫起始程式碼
{
  //initialize MYSQL object for connections
  mysql1 = mysql_init(NULL);
  
  if(mysql1 == NULL)
  {
    fprintf(stderr, "ABB : %s\n", mysql_error(mysql1));
    return;
  }
  //Connect to the database
  if(mysql_real_connect(mysql1, IPaddress, DATABASE_USERNAME, DATABASE_PASSWORD, DATABASE_NAME, 0, NULL, 0) == NULL)
  {
    fprintf(stderr, "%s\n", mysql_error(mysql1));
  }
  else
  {
    printf("Database connection successful.\r\n");
  }
}

void mysql_disconnect (void)
{
  mysql_close(mysql1);
  printf( "Disconnected from database.\r\n");
}


string s_mysql_command;

string s_mysql_Back = "' )";

string s_mysql_light="INSERT INTO userdata2 (light01,light02,light03) VALUES ( '";

string s_mysql_fan="INSERT INTO userdata2 (fan02,fan03) VALUES ( '";

string s_mysql_sensor="INSERT INTO userdata2 (sensor01,sensor02,sensor03) VALUES ( '";

string s_mysql_motor="INSERT INTO userdata2 (motor01) VALUES ( '";

string s_mysql_exittime="INSERT INTO userdata1 (account,flag) VALUES ( '";

string s_mysql_entertime = "INSERT INTO userdata1 (account,flag) VALUES ( '";


char c_str1[256]={0};//entertime for user1
char c_str2[256]={0};//entertime for user2
char c_str3[256]={0};
char c_str4[256]={0};
char c_str5[256]={0};
char c_str6[256]={0};//exittime for user1
char c_str7[256]={0};//exittime for user2

char c_str8[256]={0};//flag1 for in
char c_str9[256]={0};//flag2 for out
char c_str10[256]={0};


void mysql_write_SN_entertime(void)
{   
    
	if(sn_str=usercode[0]){
	sprintf( c_str1,"%i",primary[0]);
	sprintf( c_str8,"%i",flag[0]);
	s_mysql_command =s_mysql_entertime + c_str1 +"','"+ c_str8 +s_mysql_Back;
	if(mysql1 != NULL)
	{
	//Retrieve all data from alarm_times
		if (mysql_query(mysql1,s_mysql_command.c_str()))
		{ 
			fprintf(stderr, "%s\n", mysql_error(mysql1));
			return;
		}
	}
	}
	else if(sn_str=usercode[1]){
	sprintf( c_str2,"%i",primary[1]);
	sprintf( c_str8,"%i",flag[0]);
	s_mysql_command =s_mysql_entertime + c_str2 + "','"+ c_str8 +s_mysql_Back;
	if(mysql1 != NULL)
	{
	//Retrieve all data from alarm_times
		if (mysql_query(mysql1,s_mysql_command.c_str()))
		{ 
			fprintf(stderr, "%s\n", mysql_error(mysql1));
			return;
		}
	}
    }
}

void mysql_write_SN_exittime(void)
{   

	if(sn_str=usercode[0]){
	sprintf( c_str6,"%i",primary[0]);
	sprintf( c_str9,"%i",flag[1]);
	s_mysql_command =s_mysql_entertime + c_str6 +"','"+ c_str9 +s_mysql_Back;
	if(mysql1 != NULL)
	{
	//Retrieve all data from alarm_times
		if (mysql_query(mysql1,s_mysql_command.c_str()))
		{ 
			fprintf(stderr, "%s\n", mysql_error(mysql1));
			return;
		}
	}
	}
	else if(sn_str=usercode[1]){
	sprintf( c_str7,"%i",primary[1]);
	sprintf( c_str9,"%i",flag[1]);
	s_mysql_command =s_mysql_entertime + c_str7 + "','"+ c_str9 +s_mysql_Back;
	if(mysql1 != NULL)
	{
	//Retrieve all data from alarm_times
		if (mysql_query(mysql1,s_mysql_command.c_str()))
		{ 
			fprintf(stderr, "%s\n", mysql_error(mysql1));
			return;
		}
	}
    }
}

void mysql_write_something_1 (void)//light 函式
{
  
  sprintf( c_str3,"%i",readbuff[0]);//light01
  sprintf( c_str4,"%i",readbuff[1]);//light02
  sprintf( c_str5,"%i",readbuff[2]);//light03
  sprintf( c_str10,"%i",readbuff[3]);//light04

  s_mysql_command =s_mysql_light + c_str3 +"','"+ c_str4 +"','"+ c_str5+ "','"+ c_str10+ s_mysql_Back;
  if(mysql1 != NULL)
  {
    //Retrieve all data from alarm_times
    if (mysql_query(mysql1,s_mysql_command.c_str()))
    { 
      fprintf(stderr, "%s\n", mysql_error(mysql1));
      return;
    }
  }
}

void mysql_write_something_2 (void)//fan 函式
{

  sprintf( c_str3,"%i",readbuff[3]);//fan02
  sprintf( c_str4,"%i",readbuff[4]);//fan03
  
  s_mysql_command =s_mysql_fan + c_str3 +"','"+ c_str4 + s_mysql_Back;
  if(mysql1 != NULL)
  {
    //Retrieve all data from alarm_times
    if (mysql_query(mysql1,s_mysql_command.c_str()))
    { 
      fprintf(stderr, "%s\n", mysql_error(mysql1));
      return;
    }
  }
}

/*void mysql_write_something_4 (void)//sensor 函式
{
  
  sprintf( c_str3,"%i",readbuff[5]);//sensor02
  sprintf( c_str4,"%i",readbuff[6]);//sensor03
  sprintf( c_str5,"%i",z);//sensor01

  s_mysql_command =s_mysql_sensor + c_str3 +"','"+ c_str4 +"','"+ c_str5+ s_mysql_Back;
  if(mysql1 != NULL)
  {
    //Retrieve all data from alarm_times
    if (mysql_query(mysql1,s_mysql_command.c_str()))
    { 
      fprintf(stderr, "%s\n", mysql_error(mysql1));
      return;
    }
  }
}
*/

void mysql_write_something_3 (void)//motor 函式
{
  
  sprintf( c_str3,"%i",readbuff[7]);
  
  s_mysql_command =s_mysql_Front + c_str3 + s_mysql_Back;
  if(mysql1 != NULL)
  {
    //Retrieve all data from alarm_times
    if (mysql_query(mysql1,s_mysql_command.c_str()))
    { 
      fprintf(stderr, "%s\n", mysql_error(mysql1));
      return;
    }
  }
}


//程式開始



uint8_t HW_init(uint32_t spi_speed, uint8_t gpio);
void usage(char *);
char *KeyA="Test01";
char *KeyB="Test10";
uint8_t debug=0;



int main(int argc, char *argv[]) {

//主程式定義
	uid_t uid;
	uint8_t SN[10];
	uint16_t CType=0;
	uint8_t SN_len=0;
	char status;
	int tmp,i;

	char str[255];
	char *p;
	char sn_str[23];
	pid_t child;
	int max_page=0;
	uint8_t page_step=0;

	FILE * fmem_str;
	char save_mem=0;
	char fmem_path[255];
	uint8_t use_gpio=0;
	uint8_t gpio=255;
	uint32_t spi_speed=10000000L;


    //正文

	unsigned char readbuff[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
 	unsigned char writebuff[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 




    //RFID CODE
	if (argc>1) {
		if (strcmp(argv[1],"-d")==0) {debug=1;
		printf("Debug mode.\n");
		}else{
			usage(argv[0]);
			exit(1);
		}
	}

	if (open_config_file(config_file)!=0) {
		if (debug) {fprintf(stderr,"Can't open config file!");}
		else{syslog(LOG_DAEMON|LOG_ERR,"Can't open config file!");}
		return 1;
	}
	if (find_config_param("GPIO=",str,sizeof(str)-1,1)==1) {
		gpio=(uint8_t)strtol(str,NULL,10);
		if (gpio<28) {
			use_gpio=1;
		} else {
			gpio=255;
			use_gpio=0;
		}
	}
	if (find_config_param("SPI_SPEED=",str,sizeof(str)-1,1)==1) {
		spi_speed=(uint32_t)strtoul(str,NULL,10);
		if (spi_speed>125000L) spi_speed=125000L;
		if (spi_speed<4) spi_speed=4;
	}

	if (HW_init(spi_speed,gpio)) return 1; // ???? ?? ??????? ???????????????? RC522 ???????.
	if (read_conf_uid(&uid)!=0) return 1;
	setuid(uid);
	InitRc522();

	if (find_config_param("NEW_TAG_PATH=",fmem_path,sizeof(fmem_path)-1,0)) {
		save_mem=1;
		if (fmem_path[strlen(fmem_path)-1]!='/') {
			sprintf(&fmem_path[strlen(fmem_path)],"/");
			if (strlen(fmem_path)>=240) {
				if (debug) {fprintf(stderr,"Too long path for tag dump files!");}
				else{syslog(LOG_DAEMON|LOG_ERR,"Too long path for tag dump files!");}
				return 1;
			}
		}
	}

	for (;;) {
		status= find_tag(&CType);
		if (status==TAG_NOTAG) {
			usleep(200000);
			continue;
		}else if ((status!=TAG_OK)&&(status!=TAG_COLLISION)) {continue;}

		if (select_tag_sn(SN,&SN_len)!=TAG_OK) {continue;}

		p=sn_str;
		*(p++)='[';
		for (tmp=0;tmp<SN_len;tmp++) {
			sprintf(p,"%02x",SN[tmp]);
			p+=2;
		}
		//for debugging
		if (debug) {
		*p=0;
		fprintf(stderr,"Type: %04x, Serial: %s\n",CType,&sn_str[1]);
		}
		*(p++)=']';
		*(p++)=0;

		if (use_gpio) bcm2835_gpio_write(gpio, HIGH);
		//???? SN ? ???????
		if (find_config_param(sn_str,str,sizeof(str),1)>0) {
			child=fork();
			if (child==0) {
				fclose(stdin);
				freopen("","w",stdout);
				freopen("","w",stderr);
				execl("/bin/sh","sh","-c",str,NULL);
			} else if (child>0) {
				i=6000;
				do {
					usleep(10000);
					tmp=wait3(NULL,WNOHANG,NULL);
					i--;
				} while (i>0 && tmp!=child);

				if (tmp!=child) {
					kill(child,SIGKILL);
					wait3(NULL,0,NULL);
					if (debug) {fprintf(stderr,"Killed\n");}
				}else {
					if (debug) {fprintf(stderr,"Exit\n");}
				}
			}else{
				if (debug) {fprintf(stderr,"Can't run child process! (%s %s)\n",sn_str,str);}
				else{syslog(LOG_DAEMON|LOG_ERR,"Can't run child process! (%s %s)\n",sn_str,str);}
			}

		}else{    
             
			if (debug) {fprintf(stderr,"New tag: type=%04x SNlen=%d SN=%s\n",CType,SN_len,sn_str);}
				else{syslog(LOG_DAEMON|LOG_INFO,"New tag: type=%04x SNlen=%d SN=%s\n",CType,SN_len,sn_str);}

			if (save_mem) {
				switch (CType) {
				case 0x4400:
					max_page=0x0f;
					page_step=4;
					break;
				case 0x0400:
					PcdHalt();
					if (use_gpio) bcm2835_gpio_write(gpio, LOW);
					continue;
					max_page=0x3f;
					page_step=1;
					break;
				default:
					break;
				}
				p=str;
				sprintf(p,"%s",fmem_path);
				p+=strlen(p);
				for (tmp=0;tmp<SN_len;tmp++) {
					sprintf(p,"%02x",SN[tmp]);
					p+=2;
				}
				sprintf(p,".txt");
				if ((fmem_str=fopen(str,"r"))!=NULL) {
					fclose(fmem_str);
					PcdHalt();
					if (use_gpio) bcm2835_gpio_write(gpio, LOW);
					continue;
				}
				if ((fmem_str=fopen(str,"w"))==NULL) {
					syslog(LOG_DAEMON|LOG_ERR,"Cant open file for write: %s",str);
					PcdHalt();
					if (use_gpio) bcm2835_gpio_write(gpio, LOW);
					continue;
				}
				for (i=0;i<max_page;i+=page_step) {
					read_tag_str(i,str);
					fprintf(fmem_str,"%02x: %s\n",i,str);
				}
				fclose(fmem_str);
			}
			
              	res=i2c_smbus_read_i2c_block_data(fd, 11, 2, readbuff);
                int m;
                for(m=0; m<2;m++){
              	if(sn_str==usercode[m])
              	{
            	    if(readbuff[11]==3){         //readbuff[11]=3 表示 進門

            	 	  mysql_write_SN_entertime();
            	    }
            	    else if(readbuff[12]==4){      //readbuff[12]=4 表示 離開
            	 	  
                      mysql_write_SN_exittime();
            	    }

            	    writebuff[1]==1;
            	    res=i2c_smbus_write_i2c_block_data(fd, 1, 1, writebuff);//傳值到pic，啟動馬達來開門
                       
            	}

              }            
		}
		PcdHalt();
		if (use_gpio) bcm2835_gpio_write(gpio, LOW);

		      int fd,fd2;	
                        
		      int c, d, e, f, g, h, k, l, q, p,  err1, err2, err3, x, y, z;//err1 is for light, err2 is for fan, err2 is for sensor

		      const char *fileName = "/dev/i2c-1";				// Name of the port we will be using
	          int  address=0x70;	// Address of the SRF08 shifted right 1 bit
              int  address2=0x71; 

              unsigned char buf[6]={0,0,0,0,0,0}; 
 	          unsigned char buff[6]={0,0,0,0,0,0}; 
 	          unsigned char buf2[6]={0,0,0,0,0,0}; 



 	          __u8 reg; 								
              __s32 res;
  
              FILE *fptr1;


              float fdata1[1000];
 
	          if ((fd = open(fileName, O_RDWR)) < 0) {	// Open port for reading and writing
		          printf("Failed to open i2c port\n");
		          exit(1);
	          }
	
	          if (ioctl(fd, I2C_SLAVE, address) < 0) {		// Set the port options and set the address of the device we wish to speak to
		          printf("Unable to get bus access to talk to slave\n");
		          exit(1);
		      }
	                 
              if ((fd2 = open(fileName, O_RDWR)) < 0) {	// Open port for reading and writing
		          printf("Failed to open i2c port\n");
		          exit(1);
	          }
	
	          if (ioctl(fd2, I2C_SLAVE, address2) < 0) {		// Set the port options and set the address of the device we wish to speak to
		          printf("Unable to get bus access to talk to slave\n");
		          exit(1);
		      }

              printf("**** NOW POSITION ****\n");
	          buff[0]=0x54;
              res=i2c_smbus_write_i2c_block_data(fd, 0, 1, buff);
              usleep(1000000);
                   
              res=i2c_smbus_read_i2c_block_data(fd,2, 2, buf);
              res=i2c_smbus_read_i2c_block_data(fd2,2, 2, buf2);

              x=buf[0];
              y=buf[1];
              p=buf2[0];
              q=buf2[1];

              z=x+y;
              k=p+q;

              printf("x: %d \n",x);
              printf("y: %d \n",y);
              printf("z: %d \n",z);  

              printf("p: %d \n",p);
              printf("q: %d \n",q);
              printf("k: %d \n",k); 

              
              
              if(z<50 || k<50){                       //50為假設值
            	    writebuff[19]=1;
            	    res=i2c_smbus_write_i2c_block_data(fd, 19, 1, writebuff);//開燈
              }
              else{
             	    writebuff[18]=0;
            	    res=i2c_smbus_write_i2c_block_data(fd, 18, 1, writebuff);//關燈            	
              }
              
              
              
              res=i2c_smbus_read_i2c_block_data(fd, 0, 7, readbuff);   //0為關閉   1為開啟    2為壞掉

              c=readbuff[0];//light01
              d=readbuff[1];//light02
              e=readbuff[2];//light03(the front of the corridor)
              f=readbuff[3];//light04(the back of the corridor)

              g=readbuff[3];//fan01
              h=readbuff[4];//fan02

              l=readbuff[5];//motor00                                      
                                                              //=readbuff[6];//sensor02
                                                              //=readbuff[7];//sensor03              
                                                              //n=c+d+e+f+g+h+k+l;//n>0, something is out of work in LAB

                err1=c+d+e+f;
            	if(err1>=0){           		
                        mysql_write_something_1();//寫入燈的狀態	
                }

                err2=h+g;
            	if(err2>=0){
            			mysql_write_something_2();//寫入風扇狀況
            		}
                                                                               // err3=h+k+z;
                                                                               //if(err3>=0){mysql_write_something_3();}//寫入感測器狀況
            	if(l>=0){
            		    mysql_write_something_3();//寫入馬達狀況
            	}
	}

	bcm2835_spi_end();
	bcm2835_close();
	close_config_file();
	return 0;

}


uint8_t HW_init(uint32_t spi_speed, uint8_t gpio) {
	uint16_t sp;

	sp=(uint16_t)(250000L/spi_speed);
	if (!bcm2835_init()) {
		syslog(LOG_DAEMON|LOG_ERR,"Can't init bcm2835!\n");
		return 1;
	}
	if (gpio<28) {
		bcm2835_gpio_fsel(gpio, BCM2835_GPIO_FSEL_OUTP);
		bcm2835_gpio_write(gpio, LOW);
	}

	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(sp); // The default
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
	return 0;
}

void usage(char * str) {
	printf("Usage:\n%s [options]\n\nOptions:\n -d\t Debug mode\n\n",str);
}


//res=i2c_smbus_read_i2c_block_data(fd, 0, 20, readbuff);

//1.進入出去時間函式是否要使用迴圈  
//2.卡片讀取的小型for迴圈中是否要 else
//3.如果debug mode顯示之後,如果沒有卡片感應,會不會離開debug mode的else , 繼續做
//int c, d, e, f, g, h, k, l,  err1, err2, err3, x, y, z;//err1 is for light, err2 is for fan, err2 is for sensor
//4. address

              printf("**** NOW POSITION ****\n");
	          buff[0]=0x54;
              res=i2c_smbus_write_i2c_block_data(fd, 0, 1, buff);
              usleep(1000000);
		.
		.
		.
		.
		.
		.
              之後的程式
