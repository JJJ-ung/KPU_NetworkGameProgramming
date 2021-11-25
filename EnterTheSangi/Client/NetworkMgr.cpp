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
	cout << "Network connect!\n";
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

HRESULT NetworkMgr::Send_ClientInfo(char type, void* p)
{
	if (type >= CS_PACKET_TYPE_END)
		return E_FAIL;

	HRESULT hr = -1;

	switch (type)
	{
		case CS_PACKET_LOGIN:
			hr = send(m_socket, (char*)p, sizeof(cs_packet_login), 0);
			break;
		case CS_PACKET_CHANGE_COLOR:
			hr = send(m_socket, (char*)p, sizeof(cs_packet_change_color), 0);
			break;
		case CS_PACKET_READY:
			hr = send(m_socket, (char*)p, sizeof(cs_packet_ready), 0);
			break;
		default:
			hr = E_FAIL;
			break;
	}

	return hr;
}

HRESULT NetworkMgr::Send_LoginInfo(const char* name)
{
	cs_packet_login t = {sizeof(cs_packet_login), CS_PACKET_LOGIN, 0 };
	memcpy(t.name, name, sizeof(MAX_NAME_SIZE));
	return Send_ClientInfo(CS_PACKET_LOGIN, (void*)&t);
}

HRESULT NetworkMgr::Send_ColorInfo(D3DXVECTOR3 body, D3DXVECTOR3 cloth)
{
	cs_packet_change_color t = { sizeof(cs_packet_change_color), CS_PACKET_CHANGE_COLOR, body, cloth };
	return Send_ClientInfo(CS_PACKET_CHANGE_COLOR, (void*)&t);
}

HRESULT NetworkMgr::Send_ReadyInfo(bool ready)
{
	cs_packet_ready t = { sizeof(cs_packet_ready), CS_PACKET_READY, ready };
	return Send_ClientInfo(CS_PACKET_READY, (void*)&t);
}

char NetworkMgr::Recv_ServerInfo(void* p)
{
	char buf[BUF_SIZE];

	if(FAILED(recv(m_socket, buf, BUF_SIZE, 0)))
		return -1;

	if (p == nullptr)
		p = malloc(buf[0]);

	switch (buf[1])
	{
	case SC_PACKET_LOGIN_OK: // 서버에서 받아온 로그인 ok신호!!!
		memcpy(p, &buf, sizeof(sc_packet_login_ok));
		break;
	case SC_PACKET_CHANGE_COLOR: // 서버에서 받아온 색깔!!!!
		memcpy(p, &buf, sizeof(sc_packet_change_color));
		break;
	case SC_PACKET_LOGIN_OTHER_CLIENT:  // 나 말고 다른 플레이어 정보 받아옴!!!
		memcpy(p, &buf, sizeof(sc_packet_login_other_client));
		break;
	case SC_PACKET_REMOVE_OBJECT:  // 나 말고 다른 플레이어 정보 받아옴!!!
		//memcpy(p, &buf, sizeof(sc_packet_login_other_client));
		break;
	case SC_PACKET_READY:  // 나 말고 다른 플레이어 정보 받아옴!!!
		//memcpy(p, &buf, sizeof(sc_packet_login_other_client));
		break;
	default:
		break;
	}

	return buf[1];
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
