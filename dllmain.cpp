#include <iostream>
#include <intrin.h>

struct astruct
{
    unsigned int field_0x0;
    unsigned int field_0x4;
    unsigned int field_0x8;
    unsigned int field_0xc;
    unsigned int field_0x10;
    unsigned int field_0x14;
    unsigned int field_0x18;
    unsigned int field_0x1c;
    unsigned int field_0x20;
    unsigned int field_0x24;
    unsigned char field_0x28;
    unsigned char field_0x29;
    unsigned char field_0x2a;
    unsigned char field_0x2b;
    unsigned char field_0x2c;
    unsigned char field_0x2d;
    int field_0x2e;
};

int DecompressedData = 0;
astruct* PointerTest;

uint32_t __inline clz(uint32_t value)
{
    unsigned long index = 0;
    return _BitScanReverse(&index, value) ? 31 - index : 32;
}

void ToBigEndian(int* num) //same here
{
    int retVal;
    char* floatToConvert = (char*)num;
    char* returnFloat = (char*)&retVal;

    // swap the bytes into a temporary buffer
    returnFloat[0] = floatToConvert[3];
    returnFloat[1] = floatToConvert[2];
    returnFloat[2] = floatToConvert[1];
    returnFloat[3] = floatToConvert[0];

    floatToConvert[0] = returnFloat[0];
    floatToConvert[1] = returnFloat[1];
    floatToConvert[2] = returnFloat[2];
    floatToConvert[3] = returnFloat[3];
}

int ToBigEndian(int num) //same here
{
    int retVal;
    char* floatToConvert = (char*)&num;
    char* returnFloat = (char*)&retVal;

    // swap the bytes into a temporary buffer
    returnFloat[0] = floatToConvert[3];
    returnFloat[1] = floatToConvert[2];
    returnFloat[2] = floatToConvert[1];
    returnFloat[3] = floatToConvert[0];
    return (*(int*)returnFloat);
    floatToConvert[0] = returnFloat[0];
    floatToConvert[1] = returnFloat[1];
    floatToConvert[2] = returnFloat[2];
    floatToConvert[3] = returnFloat[3];
}

unsigned int __fastcall ProcessBuffer(unsigned int a1)
{
    int v1; // r13
    astruct* v2; // r9
    unsigned int v3; // r5
    unsigned int v4; // r4
    unsigned int v5; // r10
    char v6; // r29
    unsigned int v7; // r3
    unsigned int v8; // r8
    unsigned int v9; // r7
    unsigned int v10; // r0

    v2 = PointerTest;
    v3 = v2->field_0x29;
    if (v3 < a1)
    {
        v6 = a1 - v3;
        v5 = v2->field_0xc << (a1 - v3);
        if (v2->field_0x8 || v2->field_0x28)
        {
            v7 = v2->field_0x8;
            v8 = v2->field_0x14;
            v9 = ToBigEndian(*(unsigned int*)(v8 + v7));
            v2->field_0xc = v9;
            v2->field_0x8 = v7 + 4;
            v5 |= v9 << (32 - v6) >> (32 - v6);
            v2->field_0x29 = 32 - v6;
            v2->field_0xc = v9 >> v6;
            if (v7 == 0x7FFC)
            {
                v10 = v2->field_0x1c;
                v2->field_0x8 = 0;
                v2->field_0x14 = v8 + 0x8000;
                if (v8 + 0x8000 == v10)
                    v2->field_0x14 = v2->field_0x18;
                --v2->field_0x28;
            }
        }
    }
    else
    {
        v4 = v2->field_0xc;
        v2->field_0x29 = v3 - a1;
        v2->field_0xc = v4 >> a1;
        v5 = v4 << (32 - a1) >> (32 - a1);
    }
    return v5;
}

void FreeBuffer_int(void* buffer)
{
    free(buffer);
}

void DecompressBuffer_int(unsigned char* input_ptr, unsigned char* output_ptr)
{
    unsigned int v3; // r28
    int v4; // r29
    int v5; // r28
    astruct* v6; // r26
    unsigned char v7; // r3
    unsigned char* v8; // r5
    int v9; // r27
    int v10; // r6
    unsigned int v11; // r4
    unsigned char v12; // r0
    unsigned int v13; // r3
    unsigned int v14; // ctr
    unsigned char* v15; // r4
    astruct v16; // [sp+8h] [-48h] BYREF

    PointerTest = &v16;
    memset(PointerTest, 0, sizeof(astruct));
    v3 = (int)PointerTest;
    v16.field_0x2a = input_ptr[12];
    v16.field_0x2b = input_ptr[13];
    *(unsigned char*)(v3 + 45) = input_ptr[8] >> 6;
    *(unsigned int*)((int)PointerTest + 8) = 16;
    v4 = *((unsigned int*)input_ptr + 2) & 0xFFFFFFF;
    *(unsigned int*)(int)PointerTest = v4;
    *(unsigned int*)((int)PointerTest + 4) = v4;
    v5 = *((unsigned int*)input_ptr + 2) & 0xFFFFFFF;
    *(unsigned char*)((int)PointerTest + 40) = -1;
    *(unsigned int*)((int)PointerTest + 16) = (int)output_ptr;
    *(unsigned int*)((int)PointerTest + 32) = (int)output_ptr;
    *(unsigned int*)((int)PointerTest + 24) = (unsigned int)input_ptr;
    *(unsigned int*)((int)PointerTest + 20) = (unsigned int)input_ptr;
    *(unsigned int*)((int)PointerTest + 28) = (unsigned int)&input_ptr[(v5 + 47) & 0xFFFFFFE0];
    *(unsigned char*)((int)PointerTest + 44) = 0;
    *(unsigned char*)((int)PointerTest + 41) = 0;
    *(unsigned int*)((int)PointerTest + 12) = 0;
    v6 = (astruct*)(int)PointerTest;
    while (ProcessBuffer(1u))
    {
        v7 = ProcessBuffer(8u);

        *(unsigned char*)v6->field_0x20 = v7;
        v6->field_0x20++;

        --v6->field_0x0;
    LABEL_8:
        if (!v6->field_0x0)
            return;
    }
    v9 = ProcessBuffer(v6->field_0x2a) + 1;
    v10 = ProcessBuffer(v6->field_0x2b) + 2;
    v11 = v6->field_0x10;
    int clzsd = clz(v11 ^ (v6->field_0x20 - v9));
    v13 = v6->field_0x0 - v10;
    v6->field_0x0 = v13;
    if (!((v13 >> 31) | (v11 << clzsd >> 31)))
    {
        v14 = v10;
        v15 = (unsigned char*)v6->field_0x20;
        do
        {

            *v15 = v15[-v9];
            ++v15;
            --v14;
        } while (v14);
        v6->field_0x20 = (unsigned int)v15;
        goto LABEL_8;
    }
    v6->field_0x2c = 1;
}

unsigned int GetDecompressedSize_int(unsigned char* InputBuffer)
{
    int SACompGCData = (int)InputBuffer;

    if (*(unsigned int*)SACompGCData != 0x6F436153)
    {
        do
            SACompGCData++;
        while (*(unsigned int*)SACompGCData != 0x6F436153);
    }
    ToBigEndian((int*)(SACompGCData + 8));
    return (*(unsigned int*)(SACompGCData + 8) & 0xfffffff);
}

void DecompressFile_int(const char* filename_src, const char* filename_dst)
{
    int outputSize;

    //read file
    FILE* f;
    fopen_s(&f, filename_src, "rb");

    //get size
    fseek(f, 0L, SEEK_END);
    outputSize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    //allocate file buffer
    void* InputBuffer = malloc(outputSize);

    fread(InputBuffer, 1, outputSize, f);
    fclose(f);

    int SACompGCData = (int)InputBuffer;

    if (*(unsigned int*)SACompGCData != 0x6F436153)
    {
        do
            SACompGCData++;
        while (*(unsigned int*)SACompGCData != 0x6F436153);
    }

    ToBigEndian((int*)(SACompGCData + 8));
    int size = (*(unsigned int*)(SACompGCData + 8) & 0xfffffff);
    DecompressedData = (int)malloc(size);
    memset((void*)DecompressedData, 0, size);

    DecompressBuffer_int((unsigned char*)SACompGCData, (unsigned char*)DecompressedData);
    fopen_s(&f, filename_dst, "wb");
    fwrite((void*)DecompressedData, 1, size, f);
    fclose(f);
    free((void*)InputBuffer);
    free((void*)DecompressedData);
}

int main(int argc, char* argv[])
{
    DecompressFile_int(argv[1], argv[2]);
}

extern "C"
{
    __declspec(dllexport) unsigned int GetDecompressedSize(unsigned char* InputBuffer)
    {
        return GetDecompressedSize_int(InputBuffer);
    }

    __declspec(dllexport) void DecompressFile(const char* filename_src, const char* filename_dst)
    {
        DecompressFile_int(filename_src, filename_dst);
    }

    __declspec(dllexport) void DecompressBuffer(unsigned char* InputBuffer, unsigned char* OutputBuffer)
    {
        DecompressBuffer_int(InputBuffer, OutputBuffer);
    }

    __declspec(dllexport) void FreeBuffer(void* buffer)
    {
        FreeBuffer_int(buffer);
    }
}