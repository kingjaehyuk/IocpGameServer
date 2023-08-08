#include "pch.h"
#include "Server.h"

int main()
{
    Server server(20000, 500);

    if (server.Init())
    {
        printf("서버 초기화 실패");
        return 1;
    }

    server.Run();

    return 0;
}
