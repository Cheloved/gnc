all:
	gcc server.c -o server
	sudo ./server 80
new:
	gcc server_new.c -o server_new
	sudo ./server_new 80
