#include <iostream>
#include <intrin.h>

#define uint unsigned int
#define byte unsigned char
#define _BYTE unsigned char
#define _DWORD unsigned int

struct astruct
{
    uint field_0x0;
    uint field_0x4;
    uint field_0x8;
    uint field_0xc;
    uint field_0x10;
    uint field_0x14;
    uint field_0x18;
    uint field_0x1c;
    uint field_0x20;
    uint field_0x24;
    byte field_0x28;
    byte field_0x29;
    byte field_0x2a;
    byte field_0x2b;
    byte field_0x2c;
    byte field_0x2d;
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

/* WARNING: Variable defined which should be unmapped: uStack72 */
/* WARNING: Heritage AFTER dead removal. Example location: s0xffffffb8 : 0x80006578 */
/* WARNING: Restarted to delay deadcode elimination for space: stack */

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
            v9 = ToBigEndian(*(_DWORD*)(v8 + v7));
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

void DecompressBuffer_int(byte* input_ptr, byte* output_ptr)
{
    unsigned int v3; // r28
    int v4; // r29
    int v5; // r28
    astruct* v6; // r26
    unsigned char v7; // r3
    _BYTE* v8; // r5
    int v9; // r27
    int v10; // r6
    unsigned int v11; // r4
    unsigned char v12; // r0
    unsigned int v13; // r3
    unsigned int v14; // ctr
    _BYTE* v15; // r4
    astruct v16; // [sp+8h] [-48h] BYREF

    PointerTest = &v16;
    memset(PointerTest, 0, sizeof(astruct));
    v3 = (int)PointerTest;
    v16.field_0x2a = input_ptr[12];
    v16.field_0x2b = input_ptr[13];
    *(_BYTE*)(v3 + 45) = input_ptr[8] >> 6;
    *(_DWORD*)((int)PointerTest + 8) = 16;
    v4 = *((_DWORD*)input_ptr + 2) & 0xFFFFFFF;
    *(_DWORD*)(int)PointerTest = v4;
    *(_DWORD*)((int)PointerTest + 4) = v4;
    v5 = *((_DWORD*)input_ptr + 2) & 0xFFFFFFF;
    *(_BYTE*)((int)PointerTest + 40) = -1;
    *(_DWORD*)((int)PointerTest + 16) = (int)output_ptr;
    *(_DWORD*)((int)PointerTest + 32) = (int)output_ptr;
    *(_DWORD*)((int)PointerTest + 24) = (_DWORD)input_ptr;
    *(_DWORD*)((int)PointerTest + 20) = (_DWORD)input_ptr;
    *(_DWORD*)((int)PointerTest + 28) = (_DWORD)&input_ptr[(v5 + 47) & 0xFFFFFFE0];
    *(_BYTE*)((int)PointerTest + 44) = 0;
    *(_BYTE*)((int)PointerTest + 41) = 0;
    *(_DWORD*)((int)PointerTest + 12) = 0;
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
        v15 = (_BYTE*)v6->field_0x20;
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

uint GetDecompressedSize_int(unsigned char* InputBuffer)
{
    int SACompGCData = (int)InputBuffer;

    if (*(uint*)SACompGCData != 0x6F436153)
    {
        do
            SACompGCData++;
        while (*(uint*)SACompGCData != 0x6F436153);
    }
    ToBigEndian((int*)(SACompGCData + 8));
    return (*(uint*)(SACompGCData + 8) & 0xfffffff);
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

    if (*(uint*)SACompGCData != 0x6F436153)
    {
        do
            SACompGCData++;
        while (*(uint*)SACompGCData != 0x6F436153);
    }
    
    ToBigEndian((int*)(SACompGCData + 8));

    int size = (*(uint*)(SACompGCData + 8) & 0xfffffff);
    //13a7f
    DecompressedData = (int)malloc(size);
    memset((void*)DecompressedData, 0, size);

    DecompressBuffer_int((unsigned char*)SACompGCData, (unsigned char*)DecompressedData);
    fopen_s(&f, filename_dst, "wb");
    fwrite((void*)DecompressedData, 1, size, f);
    fclose(f);
    //std::cout << "Size:" << size << std::endl;
    free(InputBuffer);
    free((void*)DecompressedData);
}

int main(int argc, char* argv[])
{
    DecompressFile_int(argv[1], argv[2]);
}

extern "C"
{
    __declspec(dllexport) uint GetDecompressedSize(unsigned char* InputBuffer)
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
}