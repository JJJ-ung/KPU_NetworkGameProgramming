#include "framework.h"
#include "NetworkMgr.h"
#include "GameMgr.h"

IMPLEMENT_SINGLETON(NetworkMgr)

NetworkMgr::NetworkMgr()
{
}

NetworkMgr::~NetworkMgr()
{
    closesocket(m_socket);
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

    cout << "Winsock Initailize Succeed!" << endl;

    m_pGameMgr = GameMgr::GetInstance();
    if (!m_pGameMgr) return E_FAIL;

    InitializeCriticalSection(&m_Crt);
    m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Recv, this, 0, NULL);

    cout << "Receive Thread Initailize Succeed!" << endl;

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
    case CS_PACKET_PLAYER_INFO:
        hr = send(m_socket, (char*)p, sizeof(cs_packet_player_info), 0);
        break;
    default:
        cout << "None Send" << endl;
        hr = E_FAIL;
        break;
    }

    return hr;
}

HRESULT NetworkMgr::Send_LoginInfo(const char* name)
{
    cs_packet_login t = { sizeof(cs_packet_login), CS_PACKET_LOGIN, 0 };
    strcpy(t.name, name);
    return Send_ClientInfo(CS_PACKET_LOGIN, (void*)&t);
}

HRESULT NetworkMgr::Send_ColorInfo(D3DXVECTOR3 body, D3DXVECTOR3 cloth)
{
    cs_packet_change_color t = { sizeof(cs_packet_change_color), CS_PACKET_CHANGE_COLOR, body, cloth };
    return Send_ClientInfo(CS_PACKET_CHANGE_COLOR, (void*)&t);
}

HRESULT NetworkMgr::Send_ReadyInfo(bool ready)
{
    cout << ready << endl;
    cs_packet_ready t = { sizeof(cs_packet_ready), CS_PACKET_READY, ready };
    return Send_ClientInfo(CS_PACKET_READY, (void*)&t);
}

HRESULT NetworkMgr::Send_PlayerInfo(STATE::TYPE eState, D3DXVECTOR3 vPos, float fAngle)
{
    cs_packet_player_info t = {};
    t.size = sizeof(cs_packet_player_info);
    t.type = CS_PACKET_PLAYER_INFO;
    t.m_state = (char)eState;
    t.m_position.x = (short)vPos.x;
    t.m_position.y = (short)vPos.y;
    t.m_look = (short)fAngle;
    return Send_ClientInfo(CS_PACKET_PLAYER_INFO, (void*)&t);
}

unsigned NetworkMgr::Thread_Recv(void* pArg)
{
    NetworkMgr* pNetworkMgr = (NetworkMgr*)pArg;

    EnterCriticalSection(pNetworkMgr->Get_Crt());

    while (!pNetworkMgr->m_bFinish)
    {
        //cout << "Thread" << endl;
        char buf[BUF_SIZE];

        int left;
        int received;
        char* ptr = buf;

        received = recv(pNetworkMgr->m_socket, ptr, sizeof(char), 0);
        left = buf[0] - received;
        ptr += received;
        do
        {
            received = recv(pNetworkMgr->m_socket, ptr, left, 0);
            if (received == SOCKET_ERROR)
                return SOCKET_ERROR;
            else if (received == 0)
                break;
            left -= received;
            ptr += received;
        } while (left > 0);

        switch (buf[1])
        {
        case SC_PACKET_LOGIN_OK:
            cout << "SC_PACKET_LOGIN_OK" << endl;
            break;
        case SC_PACKET_CHANGE_COLOR:
            cout << "SC_PACKET_CHANGE_COLOR" << endl;
            break;
        case SC_PACKET_LOGIN_OTHER_CLIENT:
            cout << "SC_PACKET_LOGIN_OTHER_CLIENT" << endl;
            break;
        case SC_PACKET_REMOVE_OBJECT:
            break;
        case SC_PACKET_READY: 
            cout << "SC_PACKET_READY" << endl;
            break;
        default:
            break;
        }

        pNetworkMgr->m_pGameMgr->Setup_Recv(buf[1], buf);
        pNetworkMgr->m_pGameMgr->Recv_Networking(buf[1], buf);
    }

    LeaveCriticalSection(pNetworkMgr->Get_Crt());

    return 0;
}

void NetworkMgr::Free()
{
    m_bFinish = true;
    WaitForSingleObject(m_hThread, INFINITE);
    CloseHandle(m_hThread);
    DeleteCriticalSection(&m_Crt);
    cout << "Thread Closed" << endl;
    closesocket(m_socket);
    WSACleanup();
    cout << "Winsock Closed" << endl;
}
