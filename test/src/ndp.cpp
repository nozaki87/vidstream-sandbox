#include "ndp.h"

NDPFrame::NDPFrame()
{
	mIsCompleted = false;
	mFrameSize = 0;
	mPacketCount = 0;
}

void NDPFrame::add_packet(unsigned char * data, int recvsize)
{
	int size = recvsize - NDP_HEADERSIZE;
	// Create packet
	NDPPacket packet;
	packet.mHead = data;
	packet.mData = data + NDP_HEADERSIZE;
	packet.mSize = size;
	// extract total packet number & current packet id.
	char tmp[64];
	memcpy(tmp, data+4, 4); tmp[4] = '\0';
	std::string npacket(tmp);
	memcpy(tmp, data+8, 4); tmp[4] = '\0';
	std::string cpacket(tmp);
	// total number of packet in this frame.
	mPackets.resize(atoi(npacket.c_str()));
	mPackets[atoi(cpacket.c_str())] = packet;
	mFrameSize += size;
	mPacketCount++;
	if (mPacketCount == atoi(npacket.c_str())) mIsCompleted = true;
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
		delete itr->mHead;
	}
	mPackets.clear();
	mIsCompleted = false;
	mFrameSize = 0;
}

void NDPStream::add_packet(unsigned char * data, int size)
{
	char tmp[64];
	memcpy(tmp, data, 4); tmp[4] = '\0';
	std::string cframe(tmp);
	mFrames[cframe].add_packet(data, size);
	mFrames[cframe].mFrameID = atoi(cframe.c_str());
}

NDPFrame NDPStream::pop_frame()
{
	NDPFrame frame;
	for (std::map<std::string, NDPFrame>::iterator itr = mFrames.begin();
		itr != mFrames.end(); ++itr) {
		if (itr->second.mIsCompleted) {
			frame = itr->second;
			mFrames.erase(itr);
			break;
		}
	}
	return frame;
}

void NDPStream::discard_past_frame(int latest_id)
{
	for (std::map<std::string, NDPFrame>::iterator itr = mFrames.begin();
		itr != mFrames.end(); ) {
		if (latest_id - itr->second.mFrameID > 0) {
			itr->second.discard();
			mFrames.erase(itr++);
		} else {
			++itr;
		}
	}
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
		unsigned char * buff = new unsigned char [NDP_HEADERSIZE+NDP_MAX_PACKET];
	    recvsize = udpsock_recvdata(mSocket, from, NDP_HEADERSIZE+NDP_MAX_PACKET, buff);
	    if (recvsize > 0) {
		    add_packet(from, buff, recvsize);
	    } else {
	    	delete buff;    	
	    }
		frame = pop_frame();		
	} while (!frame.mIsCompleted);
	return frame;
}

void NDPServer::add_packet(std::string & from, unsigned char * data, int recvsize)
{
	mStreams[from].add_packet(data, recvsize);
}

NDPFrame NDPServer::pop_frame()
{
	for (std::map<std::string, NDPStream>::iterator itr = mStreams.begin();
		itr != mStreams.end(); ++itr) {
		NDPFrame frame = itr->second.pop_frame();
		if (frame.mIsCompleted) {
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

int NDPClient::make_packet(unsigned char * packet, int cframe, int npacket, int cpacket, unsigned char * data, int size)
{
	sprintf((char *)packet, "%04d%04d%04d", cframe, npacket, cpacket);
	packet += NDP_HEADERSIZE;
	int payloadsize = size;
	if (payloadsize > NDP_MAX_PACKET) payloadsize = NDP_MAX_PACKET;
	memcpy(packet, data, payloadsize);
	return payloadsize;
}

void NDPClient::send_frame(std::string & to, unsigned char * data, int size)
{
	int packet_count = 0;
	int packet_num   = (size+NDP_MAX_PACKET-1) / NDP_MAX_PACKET;
	unsigned char packet[NDP_HEADERSIZE+NDP_MAX_PACKET];
	while (size > 0) {
		int packet_size = make_packet(packet, mFrameCount, packet_num, packet_count, data, size);
		udpsock_senddata(mSocket, to.c_str(), mPort, packet, NDP_HEADERSIZE+packet_size);
		data += packet_size;
		size -= packet_size;
#if defined(WIN32)
		Sleep(1);
#else
		usleep(1);
#endif
		packet_count++;
	}
	mFrameCount++;
}



