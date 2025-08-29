build_test:
	gcc -DCAN_IF_NAME=\"vcan0\" main.c lib/can/can.c lib/gpio/gpio_mock.c hal/hal_break_pedal.c lib/uds/uds.c -pthread -o break_pedal

build:
	gcc main.c lib/can/can.c lib/gpio/gpio.c hal/hal_break_pedal.c lib/uds/uds.c -pthread -lgpiod -o break_pedal

run:
	./break_pedal

docker-build-test:
	docker build -t break-pedal-test:arm64 .

docker-run:
	docker run -d --net=host --cap-add=NET_ADMIN --cap-add=NET_RAW break-pedal-test:arm64
