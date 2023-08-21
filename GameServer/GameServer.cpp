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

    RingBuffer *b = new RingBuffer(15);
    //printf("capacity: %d\n", b->GetCapacity()); 

    char randomString[10];
    char* message = new char[10];

    for (int i = 0; i < 22; i++) 
    {
        printf("%d회\n", i + 1);

        generateRandomString(randomString, 10);
        printf("%s, %d\n", randomString, (int)strlen(randomString));

        b->Write(randomString, (int)strlen(randomString) + 1);
        printf("Write: free: %d, used: %d\n", b->GetFreeSize(), b->GetUsedSize());
        //printf("WC: %p, RC: %p\n", b->GetWriteCursor(), b->GetReadCursor());

        b->Peek(message, 10);
        printf("Peek : %s\n", message);

        b->Read(message, (int)strlen(randomString) + 1);
        printf("Read : free: %d, used: %d\n", b->GetFreeSize(), b->GetUsedSize());
        //printf("WC: %p, RC: %p\n", b->GetWriteCursor(), b->GetReadCursor());

        printf("%s, %d\n\n", message, (int)strlen(message));
    }

    /*Server server(20000, 500);

    if (server.Init())
    {
        printf("서버 초기화 실패");
        return 1;
    }

    server.Run();*/

    return 0;
}
