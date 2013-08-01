#ifndef _NDP_H_
#define _NDP_H_

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include "udp_wrapper.h"

#define NDP_MAX_PACKET 8193

class NDPPacket {
public:
	unsigned char * mData;
	int 			mSize;
};

class NDPFrame {
public:
	NDPFrame();
	void add_packet(unsigned char * data, int recvsize);
	int read_data(unsigned char * data, int size);
	int get_size();
	void discard();
	bool mIsCompleted;
private:
	int mFrameSize;
	std::vector<NDPPacket> mPackets;
};

class NDPServer {
public:
	NDPServer();
	~NDPServer();
	void set_port(int port);
	void set_addr(std::string addr);
	NDPFrame recv_frame();

private:
	void setup();
	void add_packet(std::string & from, unsigned char * data, int recvsize, int size);
	NDPFrame pop_frame();
	SOCKET 		mSocket;
	std::string mAddr;
	int 		mPort;
	std::map<std::string, NDPFrame> mFrames;
};

class NDPClient {
public:
	NDPClient();
	~NDPClient();
	void set_port(int port);
	void set_destination(std::string addr);
	void send_frame(std::string & to, unsigned char * data, int size);

private:
	void setup();
	SOCKET 		mSocket;
	std::string mAddr;
	int 		mPort;
	int 		mFrameCount;
};

#endif // _NDP_H_
