#ifndef _NDP_H_
#define _NDP_H_

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include "udp_wrapper.h"

#define NDP_MAX_PACKET 32
#define NDP_HEADERSIZE 16

class NDPPacket {
public:
	unsigned char * mHead;
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
	int mFrameID;
private:
	int mPacketCount;
	int mFrameSize;
	std::vector<NDPPacket> mPackets;
};

class NDPStream {
public:
	void add_packet(unsigned char * data, int size);
	NDPFrame pop_frame();
	void discard_past_frame(int latest_id);
private:
	std::map<std::string, NDPFrame> mFrames;
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
	void add_packet(std::string & from, unsigned char * data, int recvsize);
	NDPFrame pop_frame();
	SOCKET 		mSocket;
	std::string mAddr;
	int 		mPort;
	std::map<std::string, NDPStream> mStreams;
};

class NDPClient {
public:
	NDPClient();
	~NDPClient();
	void set_port(int port);
	void set_destination(std::string addr);
	void send_frame(std::string & to, unsigned char * data, int size);

private:
	int make_packet(unsigned char * packet, int cframe, int npacket, int cpacket, unsigned char * data, int size);
	void setup();
	SOCKET 		mSocket;
	std::string mAddr;
	int 		mPort;
	int 		mFrameCount;
};

#endif // _NDP_H_
