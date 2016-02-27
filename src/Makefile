CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
export CC
export AR

all:
	$(CC) -g -o run_ROVER -I./ \
		main.c \
		Functional_Test/Functional_Test.c \
		Functions_WatchDog/Functions_WatchDog.c \
		Lib_Gpio/beh_BBB_gpio.c \
		Lib_Gpio/drv_BBB_gpio.c \
		Lib_Motor/Lib_Motor.c \
		Lib_PWM/Lib_PWM.c \
		Lib_Servo/Lib_Servo.c \
		Lib_Sonar/Lib_Sonar.c \
		-lpthread -lprussdrv
		 -Wall

clean:
	rm -f run_ROVER
	
install: all
	cp run_ROVER /usr/local/bin