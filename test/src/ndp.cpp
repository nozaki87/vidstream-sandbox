#include "ndp.h"

NDPFrame::NDPFrame()
{
	mIsCompleted = false;
	mFrameSize = 0;
}

void NDPFrame::add_packet(unsigned char * data, int recvsize)
{
	NDPPacket packet;
	packet.mData = data;
	packet.mSize = recvsize;
	mPackets.push_back(packet);
	mFrameSize += recvsize;
}

int NDPFrame::read_data(unsigned char * data, int size)
{
	int dcount = 0;
	for (std::vector<NDPPacket>::iterator itr = mPackets.begin();
		itr != mPackets.end(); ++itr) {
		if (dcount + itr->mSize > size) {
			memcpy(data + dcount, itr->mData, size - dcount);
			dcount += size - dcount;
			break;
		} else {
			memcpy(data + dcount, itr->mData, itr->mSize);			
			dcount += itr->mSize;
		}
	}
	return dcount;
}

int NDPFrame::get_size()
{
	return mFrameSize;
}

void NDPFrame::discard()
{
	for (std::vector<NDPPacket>::iterator itr = mPackets.begin();
		itr != mPackets.end(); ++itr) {
		delete itr->mData;
	}
	mPackets.clear();
	mIsCompleted = false;
	mFrameSize = 0;
}

NDPServer::NDPServer()
{
	mSocket = -1;
	mPort   = -1;
}

NDPServer::~NDPServer()
{
	if (mSocket > 0) {
		deinit_udpsock(mSocket);
	}
}

void NDPServer::setup()
{
	if (mPort > 0) {
		if (mSocket > 0) {
			deinit_udpsock(mSocket);
		}
		if (mAddr.size() > 0) {
			mSocket = udpsock_server(mPort, mAddr.c_str());
		} else {
			mSocket = udpsock_server(mPort, NULL);
		}
	}
}

void NDPServer::set_port(int port)
{
	mPort = port;
	setup();
}

void NDPServer::set_addr(std::string addr)
{
	mAddr = addr;
	setup();
}

NDPFrame NDPServer::recv_frame()
{
	NDPFrame frame;
	std::string from;
	int recvsize;
	do {
		unsigned char * buff = new unsigned char [NDP_MAX_PACKET];
	    recvsize = udpsock_recvdata(mSocket, from, NDP_MAX_PACKET, buff);
	    if (recvsize > 0) {
		    add_packet(from, buff, recvsize, NDP_MAX_PACKET);
	    } else {
	    	delete buff;    	
	    }
		frame = pop_frame();		
	} while (!frame.mIsCompleted);
	printf("entry %ld\n", mFrames.size());
	return frame;
}

void NDPServer::add_packet(std::string & from, unsigned char * data, int recvsize, int size)
{
	mFrames[from].add_packet(data, recvsize);
	printf("fs %d\n", mFrames[from].get_size());
	if (recvsize < size) mFrames[from].mIsCompleted = true;
}

NDPFrame NDPServer::pop_frame()
{
	for (std::map<std::string, NDPFrame>::iterator itr = mFrames.begin();
		itr != mFrames.end(); ++itr) {
		if (itr->second.mIsCompleted) {
			NDPFrame frame = itr->second;
			mFrames.erase(itr);
			return frame;
		}
	}
	NDPFrame frame;
	return frame;
}

NDPClient::NDPClient()
{
	mSocket = udpsock_client();
	mPort   = -1;
	mFrameCount = 0;
}

NDPClient::~NDPClient()
{
	if (mSocket > 0) {
		deinit_udpsock(mSocket);
	}
}

void NDPClient::set_port(int port)
{
	mPort = port;
}

void NDPClient::set_destination(std::string addr)
{
	mAddr = addr;
}

#include <time.h>

void NDPClient::send_frame(std::string & to, unsigned char * data, int size)
{
	unsigned char packet[NDP_MAX_PACKET];
	while (size > 0) {
		int sendsize = size;
		if (sendsize > NDP_MAX_PACKET) sendsize = NDP_MAX_PACKET;
		printf("%d,", sendsize);
		udpsock_senddata(mSocket, to.c_str(), mPort, data, sendsize);
		data += sendsize;
		size -= sendsize;
		usleep(1);
	}
	mFrameCount++;
	printf("\n");
}



