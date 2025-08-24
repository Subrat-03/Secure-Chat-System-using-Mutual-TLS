CXX = g++
CXXFLAGS = -Wall -std=c++17
LIBS = -lssl -lcrypto -lpthread

SRC_DIR = src

all: server client

server:
	$(CXX) $(CXXFLAGS) -o server $(SRC_DIR)/secure_chat_server.cpp $(SRC_DIR)/tls_wrapper.cpp $(LIBS)

client:
	$(CXX) $(CXXFLAGS) -o client $(SRC_DIR)/secure_chat_client.cpp $(SRC_DIR)/tls_wrapper.cpp $(LIBS)

clean:
	rm -f server client
