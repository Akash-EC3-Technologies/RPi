build_test:
	gcc -DCAN_IF_NAME=\"vcan0\" main.c lib/can/can.c lib/gpio/gpio_mock.c hal/hal_tail_lamp.c -o tail_lamp

build:
	gcc main.c lib/can/can.c lib/gpio/gpio.c hal/hal_tail_lamp.c -o tail_lamp

run:
	./tail_lamp
