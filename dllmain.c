#include <intrin.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
	uint32_t lengthLeft;
	uint32_t length;
	uint32_t field_0x8;
	uint32_t field_0xc;
	uintptr_t outputBuffer;
	uint8_t* readHead;
	uint8_t* inputBuffer;
	uint8_t* field_0x1c;
	uint8_t* writeHead;

	uint8_t field_0x28;
	uint8_t field_0x29;
	uint8_t field_0x2a;
	uint8_t field_0x2b;

	uint8_t field_0x2d;
} SACompGCStatus;

#ifdef WIN64

inline uint64_t clz64(uint64_t value)
{
	uint32_t index = 0;
	return _BitScanReverse64(&index, value) ? 63 - index : 64;
}

#else

inline uint32_t clz(uint32_t value)
{
	uint32_t index = 0;
	return _BitScanReverse(&index, value) ? 31 - index : 32;
}

#endif

inline uint32_t ULongByteSwap(uint32_t num)
{
	return _byteswap_ulong(num);
}

inline uint32_t SACompGC_GetData(uint32_t numBits, SACompGCStatus* data)
{
	uint32_t retVal;

	if (data->field_0x29 < numBits)
	{
		uint8_t v6 = numBits - data->field_0x29;
		retVal = data->field_0xc << (numBits - data->field_0x29);
		if (data->field_0x8 || data->field_0x28)
		{
			uint32_t v9 = ULongByteSwap(*(uint32_t*)(data->readHead + data->field_0x8));
			data->field_0x8 += 4;
			retVal |= v9 << (32 - v6) >> (32 - v6);
			data->field_0x29 = 32 - v6;
			data->field_0xc = v9 >> v6;
			if (data->field_0x8 == 0x8000)
			{
				data->field_0x8 = 0x0;
				data->readHead += 0x8000;
				if (data->readHead == data->field_0x1c)
					data->readHead = data->inputBuffer;
				data->field_0x28--;
			}
		}
	}
	else
	{
		uint32_t v4 = data->field_0xc;
		data->field_0x29 -= numBits;
		data->field_0xc >>= numBits;
		retVal = v4 << (32 - numBits) >> (32 - numBits);
	}

	return retVal;
}

inline void SACompGCStatus_Process(SACompGCStatus* data)
{
	int32_t copyIdx;
	uint32_t numBytes;
	intptr_t clzsd;

	// loop for data
	while (SACompGC_GetData(1u, data))
	{
		*data->writeHead = SACompGC_GetData(8u, data);
		data->writeHead++;
		data->lengthLeft--;

	read:
		if (!data->lengthLeft)
			return;
	}

	// perform RLE lookback
	copyIdx = SACompGC_GetData(data->field_0x2a, data) + 1;
	numBytes = SACompGC_GetData(data->field_0x2b, data) + 2;
	data->lengthLeft -= numBytes;

#ifdef WIN64
	clzsd = clz64(data->outputBuffer ^ (uintptr_t)(data->writeHead - copyIdx));
#else
	clzsd = clz(data->outputBuffer ^ (uintptr_t)(data->writeHead - copyIdx));
#endif

	if (!((data->lengthLeft >> 31) | (data->outputBuffer << clzsd >> (sizeof(uintptr_t) * 8 - 1))))
	{
		while (numBytes)
		{
			*data->writeHead = data->writeHead[-copyIdx];
			data->writeHead++;
			numBytes--;
		}
		goto read;
	}
}

uint32_t SACompGC_GetDecompressedSize(void* inputBuffer, uint8_t** dataStart, size_t dataSize)
{
	uint32_t* SACompGCData = (uint32_t*)inputBuffer;

	if (inputBuffer)
	{

		while (*SACompGCData != 0x6f436153)
		{
			// do not read out of bounds
			if (dataSize &&
				(uintptr_t)((uint8_t*)SACompGCData - ((uintptr_t)inputBuffer - ((uintptr_t)inputBuffer % 4))) >= (dataSize - (dataSize % 4)))
				return 0;

			SACompGCData++;
		}

		if (dataStart)
			*dataStart = (uint8_t*)SACompGCData;

		return ULongByteSwap(SACompGCData[2]) & 0x0fffffff;
	}
	else
		return 0;
}

void SACompGC_DecompressBuffer(uint8_t* inputPtr, uint8_t* outputPtr)
{
	if (inputPtr && outputPtr)
	{
		SACompGCStatus data = { 0 };
		int32_t size = SACompGC_GetDecompressedSize(inputPtr, &inputPtr, (size_t)(outputPtr - inputPtr));
		assert(size);

		data.field_0x2a = inputPtr[12];
		data.field_0x2b = inputPtr[13];
		data.field_0x2d = inputPtr[8] >> 6;
		data.field_0x8 = 16;
		data.lengthLeft = size;
		data.length = size;
		data.field_0x28 = -1;
		data.outputBuffer = (uintptr_t)outputPtr;
		data.writeHead = outputPtr;
		data.inputBuffer = inputPtr;
		data.readHead = inputPtr;
		data.field_0x1c = &inputPtr[(size + 47) & 0xffffffe0];

		SACompGCStatus_Process(&data);
	}
}

void SACompGC_DecompressFile(const char* srcFile, const char* dstFile)
{
	size_t inputSize;
	uint32_t outputSize;
	
	void* InputBuffer;
	void* DecompressedData;
	uint8_t* SACompGCData;

	FILE* f;

	fopen_s(&f, srcFile, "rb");
	assert(f);

	fseek(f, 0L, SEEK_END);
	inputSize = ftell(f);
	fseek(f, 0L, SEEK_SET);

	InputBuffer = malloc(inputSize);
	assert(InputBuffer);

	fread(InputBuffer, 1, inputSize, f);
	fclose(f);
	
	outputSize = SACompGC_GetDecompressedSize(InputBuffer, &SACompGCData, inputSize);
	if (!outputSize)
	{
		fputs("File is not a SaCompGC archive.\n", stderr);
		exit(1);
	}

	DecompressedData = malloc(outputSize);
	assert(DecompressedData);
	memset(DecompressedData, 0, outputSize);

	SACompGC_DecompressBuffer(SACompGCData, (unsigned char*)DecompressedData);

	fopen_s(&f, dstFile, "wb");
	assert(f);
	fwrite(DecompressedData, 1, outputSize, f);
	fclose(f);

	free(InputBuffer);
	free(DecompressedData);

	printf("%s: Uncompressed %u bytes (compression ratio %.2f%%).\n", dstFile, outputSize, (double)inputSize / outputSize * 100.);
}

#ifdef SACOMPGC_EXE

int32_t main(int32_t argc, char* argv[])
{
	if (argc == 3)
	{
		SACompGC_DecompressFile(argv[1], argv[2]);
		return 0;
	}
	else if (argc == 2)
	{
		size_t newLength = (size_t)strnlen(argv[1], FILENAME_MAX) + strlen(".dec");

		if (newLength < FILENAME_MAX)
		{
			char* newString = calloc(1, newLength + 1);
			assert(newString);
			strcat(newString, argv[1]);
			strcat(newString, ".dec");
			SACompGC_DecompressFile(argv[1], newString);
			free(newString);
			return 0;
		}
		else
		{
			fprintf(stderr, "Automatic filename %s.dec too long.\n", argv[1]);
			return 1;
		}
	}

	fputs("Usage: SaCompGC <file.rel> [decompressed.rel]\n", stderr);
	return 1;
}

#else

__declspec(dllexport) unsigned int GetDecompressedSize(void* inputBuffer)
{
	return SACompGC_GetDecompressedSize(inputBuffer, NULL, 0);
}

__declspec(dllexport) unsigned int GetDecompressedSizeEx(void* inputBuffer, unsigned char** dataStart, size_t dataSize)
{
	return SACompGC_GetDecompressedSize(inputBuffer, dataStart, dataSize);
}

__declspec(dllexport) void DecompressFile(const char* srcFile, const char* dstFile)
{
	SACompGC_DecompressFile(srcFile, dstFile);
}

__declspec(dllexport) void DecompressBuffer(void* inputBuffer, void* outputBuffer)
{
	SACompGC_DecompressBuffer((unsigned char*)inputBuffer, (unsigned char*)outputBuffer);
}

__declspec(dllexport) void FreeBuffer(void* buffer)
{
	if (buffer)
		free(buffer);
}

#endif
