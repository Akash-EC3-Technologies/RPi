build_test:
	gcc -g -fsanitize=address -DCAN_IF_NAME=\"vcan0\" main.c lib/can/can.c lib/gpio/gpio_mock.c hal/hal_tail_lamp.c lib/uds/uds.c -pthread -o tail_lamp

build:
	gcc -g -fsanitize=address main.c lib/can/can.c lib/gpio/gpio.c hal/hal_tail_lamp.c lib/uds/uds.c -pthread -lgpiod -o tail_lamp

run:
	./tail_lamp
