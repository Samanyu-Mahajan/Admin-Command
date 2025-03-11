CC = g++ -std=c++11
CFLAGS = -I/opt/homebrew/Cellar/boost/1.87.0/include -I/opt/homebrew/Cellar/openssl@3/3.4.1/include
LDFLAGS = -L/opt/homebrew/Cellar/boost/1.87.0/lib -lboost_system -pthread -L/opt/homebrew/Cellar/openssl@3/3.4.1/lib -lssl -lcrypto


admin: admin.cpp main.cpp
	$(CC) main.cpp -o server $(CFLAGS) $(LDFLAGS)
	./server


