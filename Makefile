all:
	+$(MAKE) -C SnakeClient
	+$(MAKE) -C SnakeServer

clean:
	+$(MAKE) -C SnakeClient clean
	+$(MAKE) -C SnakeServer clean
