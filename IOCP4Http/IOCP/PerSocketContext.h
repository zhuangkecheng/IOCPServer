#ifndef __CLIENT_CONTEXT_H__
#define __CLIENT_CONTEXT_H__
#include "Addr.h"
#include "Buffer.h"
#include "PerIoContext.h"
#include <algorithm>
#include <string>
#include <queue>
#include <map>

struct ListenContext
{
	SOCKET m_socket; //监听socket
	SOCKADDR_IN m_addr; //监听地址

	ListenContext(short port, const std::string& ip = "0.0.0.0");
};

//============================================================================
//	单句柄数据结构体定义(用于每一个完成端口，也就是每一个Socket的参数)
//============================================================================
//每个SOCKET对应的数据结构(调用GetQueuedCompletionStatus传入)：-
//SOCKET，该SOCKET对应的客户端地址，作用在该SOCKET操作集合(对应结构IoContext)；
struct ClientContext
{
	SOCKET m_socket; //客户端socket
	Addr m_addr; //客户端地址
	RecvIoContext* m_recvIoCtx;
	SendIoContext* m_sendIoCtx;	
	std::queue<Buffer> m_outBufQueue;
	Buffer m_inBuf;
	Buffer m_outBuf;
	CRITICAL_SECTION m_csLock; //保护ClientContext
	//Avoids Access Violation，该值为0时才能释放ClientContext
	ULONG m_nPendingIoCnt;
	
	ClientContext(const SOCKET& socket = INVALID_SOCKET);	
	~ClientContext(); //socket由IocpServer释放
	void reset();

	void appendToBuffer(PBYTE pInBuf, size_t len);
	void appendToBuffer(const std::string& inBuf);
};

#endif // !__CLIENT_CONTEXT_H__

