//
//  SWSockets.h
//  Session Wire
//
//  Created by Carson Fee on 5/12/15.
//  Copyright (c) 2015 Carson Fee. All rights reserved.
//

#ifndef __Session_Wire__SWSockets__
#define __Session_Wire__SWSockets__

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include "readwriterqueue.h"

using namespace SessionWire;

class SWSocket;

struct SWPacket
{
    unsigned int    packetType;
    unsigned char   buffer[256];
};

struct Attribs
{
    int             m_socket;
    SWSocket*       m_pSocketManager;
    sockaddr_in     m_inAddr4;
    sockaddr_in6    m_inAddr6;
    pthread_t       m_sendThread, m_receiveThread;
};

class SWSocket
{
public:
    SWSocket();
    ~SWSocket();
    void InitSWSocket();
    void Join();
    void Exit();
    int getStopIssued(void);
    void setStopIssued(int val);
    static void StarNetworkInbound(void *attr);
    static void StarNetworkOutbound(void *attr);
    void ProcessOutboundPackets(void *attr);
    void ProcessInboundPackets(void *attr);
    
private:
    hostent*        m_pHost;
    Attribs         m_attribs;
    volatile bool   m_stopRequested;
    
};

extern ReaderWriterQueue<SWPacket> GOutPacketQueue;
extern ReaderWriterQueue<SWPacket> GInPacketQueue;

#endif /* defined(__Session_Wire__SWSockets__) */
