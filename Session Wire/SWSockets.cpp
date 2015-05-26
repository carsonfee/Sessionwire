//
//  SWSockets.cpp
//  Session Wire
//
//  Created by Carson Fee on 5/12/15.
//  Copyright (c) 2015 Carson Fee. All rights reserved.
//
#include "SWSockets.h"

ReaderWriterQueue<SWPacket> GOutPacketQueue(128);
ReaderWriterQueue<SWPacket> GInPacketQueue(128);

SWSocket::SWSocket()
: m_pHost(nullptr)
, m_stopRequested(false)
{
    
}

SWSocket::~SWSocket()
{
    close(m_attribs.m_socket);
}

void SWSocket::InitSWSocket()
{
    pthread_create(&m_attribs.m_sendThread, NULL, (void * (*) (void *))StarNetworkInbound, this);
    pthread_create(&m_attribs.m_receiveThread, NULL, (void * (*) (void *))StarNetworkOutbound, this);
    
}

void SWSocket::StarNetworkInbound(void *attr)
{
    SWSocket *sockManager = reinterpret_cast<SWSocket*>(attr);
    
    //open UDP socket
    sockManager->m_attribs.m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if ( sockManager->m_attribs.m_socket == -1) {
        std::cerr << "socket failed " << strerror(errno) << std::endl;
    }
    
    //initialize the server address
    sockManager->m_attribs.m_inAddr4.sin_family = AF_INET;
    sockManager->m_attribs.m_inAddr4.sin_port = htons(7000);
    sockManager->m_pHost = gethostbyname("10.0.1.14");
    sockManager->m_attribs.m_inAddr4.sin_addr = *((struct in_addr *)sockManager->m_pHost->h_addr);
    
    sockManager->ProcessInboundPackets(&sockManager->m_attribs);
}

void SWSocket::StarNetworkOutbound(void *attr)
{
    //SW_CFEE: Since our socket attribs were already set up for inbound there's no need to
    //set them again for outbound just pass along
    reinterpret_cast<SWSocket *>(attr)->ProcessOutboundPackets(attr);
}

void SWSocket::Join()
{
    pthread_join(m_attribs.m_receiveThread, NULL);
    pthread_join(m_attribs.m_sendThread, NULL);
}

void SWSocket::Exit()
{
    m_stopRequested = true;
    Join();
}

void SWSocket::ProcessOutboundPackets(void *attr)
{
    Attribs *attribs = reinterpret_cast<Attribs*>(attr);
    SWPacket data;
    while (!m_stopRequested)
    {
        if (GOutPacketQueue.try_dequeue(data))
        {
            std::cout << "WIN Sauce" << std::endl;
            sendto( attribs->m_socket, data.buffer, sizeof(data.buffer), 0,
                   (struct sockaddr *)&attribs->m_inAddr4, sizeof(struct sockaddr_in));
        }
    }

}

void SWSocket::ProcessInboundPackets(void *attr)
{
    Attribs *attribs = reinterpret_cast<Attribs*>(attr);
    SWPacket data;
    while(!m_stopRequested)
    {
        if(recv(attribs->m_socket, data.buffer, sizeof(data.buffer), 0))
        {
            GInPacketQueue.try_enqueue(data);
        }
        else
        {
            std::cerr << "Server not responding" << std::endl;
            break;
        }
    }
    
    close(attribs->m_socket);
    pthread_cancel(attribs->m_sendThread);
}