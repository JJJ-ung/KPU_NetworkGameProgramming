#include "framework.h"
#include "NetworkMgr.h"

IMPLEMENT_SINGLETON(NetworkMgr)

NetworkMgr::NetworkMgr()
{
}

NetworkMgr::~NetworkMgr()
{
	Free();
}

HRESULT NetworkMgr::Ready_WinSock()
{
	WSAStartup(MAKEWORD(2, 2), &m_wsa);
	m_socket = socket(AF_INET, SOCK_STREAM, 0);

	ZeroMemory(&m_addr, sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	m_addr.sin_port = htons(SERVER_PORT);

	retval = connect(m_socket, (SOCKADDR*)&m_addr, sizeof(m_addr));
	if (connect(m_socket, (SOCKADDR*)&m_addr, sizeof(m_addr)) == SOCKET_ERROR)
		return E_FAIL;

	cout << "Network Mgr Initailize Succeed!" << endl;

	return NOERROR;
}

HRESULT NetworkMgr::Setup_Networking()
{
	// 기존 activate

	return NOERROR;
}

HRESULT NetworkMgr::Send_ClientInfo(GameStatePlayer& tPlayerPacket)
{
	if (FAILED(send(m_socket, (char*)&tPlayerPacket, sizeof(GameStatePlayer), 0)))
	{
		Render_Error("Failed To Send Client Info");
		return E_FAIL;
	}
	return NOERROR;
}

HRESULT NetworkMgr::Send_CustomizeInfo(cs_packet_change_color& tColorPacket)
{
	tColorPacket.type = CS_PACKET_CHANGE_COLOR;
	if (FAILED(send(m_socket, (char*)&tColorPacket, sizeof(cs_packet_change_color), 0)))
	{
		Render_Error("Failed To Send Customizing Info");
		return E_FAIL;
	}
	return NOERROR;
}

HRESULT NetworkMgr::Recv_ServerInfo(void* tRecvInfo)
{
	char buf[BUF_SIZE];

	if(FAILED(recv(m_socket, buf, BUF_SIZE, 0)))
	{
		Render_Error("Failed To Receive Info");
		return E_FAIL;
	}

	recv(m_socket, buf, BUF_SIZE, 0);
	switch(buf[0])
	{
	case SC_PACKET_LOGIN_OK: // 서버에서 받아온 로그인 ok신호!!!
		sc_packet_login_ok rp;
		memcpy(&rp, &buf, sizeof(sc_packet_login_ok));
		break;

	case SC_PACKET_CHANGE_COLOR: // 서버에서 받아온 색깔!!!!
		sc_packet_change_color rp;
		memcpy(&rp, &buf, sizeof(sc_packet_change_color));
		break;

	default:
		break;
	}

	return NOERROR;
}

void NetworkMgr::Render_Error(const char* msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	cout << "[" << msg << "] : " << (char*)lpMsgBuf << endl;
	LocalFree(lpMsgBuf);
}

int NetworkMgr::recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;

		left -= received;
		ptr += received;
	}

	return (len - left);
}

void NetworkMgr::Free()
{
	closesocket(m_socket);
	WSACleanup();
}
