build_test:
	gcc -g -fsanitize=address -DCAN_IF_NAME=\"vcan0\" main.c lib/can/can.c lib/gpio/gpio_mock.c hal/hal_break_pedal.c lib/uds/uds.c -pthread -o break_pedal

build:
	gcc -g -fsanitize=address main.c lib/can/can.c lib/gpio/gpio.c hal/hal_break_pedal.c lib/uds/uds.c -pthread -lgpiod -o break_pedal

run:
	./break_pedal
