build:
	g++ -Wall -o client client.cpp buffer.cpp helpers.cpp requests.cpp
	
run: 
	./client
	
clean:
	rm client