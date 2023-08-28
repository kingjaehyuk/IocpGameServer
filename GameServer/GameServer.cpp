#include "pch.h"
#include "Server.h"
#include "RingBuffer.h"

void generateRandomString(char* str, size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    for (size_t i = 0; i < length - 1; i++) {
        int index = rand() % (sizeof(charset) - 1);
        str[i] = charset[index];
    }

    str[length - 1] = '\0';
}

int main()
{
    srand(time(NULL));

    RingBuffer *b = new RingBuffer(5);
    //printf("capacity: %d\n", b->GetCapacity()); 

    //char randomString[10];
    //char* message = new char[10];

    //printf("시작포인터: %p | 끝  포인터: %p\n\n", b->GetBegin(), b->GetEnd());

    //b->Write("12345", 5);
    //printf("쓰기 완료\n");
    //printf("쓰기포인터: %p | 읽기포인터: %p\n\n", b->GetWriteCursor(), b->GetReadCursor());

    //b->Read(message, 5);
    //printf("읽기 완료\n");
    //printf("쓰기포인터: %p | 읽기포인터: %p\n\n", b->GetWriteCursor(), b->GetReadCursor());

    //b->Write("123", 3);
    //printf("쓰기 완료\n");
    //printf("쓰기포인터: %p | 읽기포인터: %p\n\n", b->GetWriteCursor(), b->GetReadCursor());

    //b->Read(message, 3);
    //printf("읽기 완료\n");
    //printf("쓰기포인터: %p | 읽기포인터: %p\n\n", b->GetWriteCursor(), b->GetReadCursor());

    //b->Write("12", 2);
    //printf("쓰기 완료\n");
    //printf("쓰기포인터: %p | 읽기포인터: %p\n\n", b->GetWriteCursor(), b->GetReadCursor());

    //b->Read(message, 2);
    //printf("읽기 완료\n");
    //printf("쓰기포인터: %p | 읽기포인터: %p\n\n", b->GetWriteCursor(), b->GetReadCursor());

    //b->Write("1", 1);
    //printf("쓰기 완료\n");
    //printf("쓰기포인터: %p | 읽기포인터: %p\n\n", b->GetWriteCursor(), b->GetReadCursor());

    //b->Read(message, 1);
    //printf("읽기 완료\n");
    //printf("쓰기포인터: %p | 읽기포인터: %p\n\n", b->GetWriteCursor(), b->GetReadCursor());

    Server server(20000, 500);

    if (server.Init())
    {
        printf("서버 초기화 실패");
        return 1;
    }

    server.Run();

    return 0;
}
