build_test:
	gcc -DCAN_IF_NAME=\"vcan0\" main.c lib/can/can.c lib/gpio/gpio_mock.c hal/hal_break_pedal.c -o break_pedal

build:
	gcc main.c lib/can/can.c lib/gpio/gpio.c hal/hal_break_pedal.c -o break_pedal

run:
	./break_pedal
