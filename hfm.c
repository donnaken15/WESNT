
/*
 * WESLEY'S NT
 *
 * Title: Huffman coding
 * Author: Wesley
 * License: MIT (may change) \
 * (i might be too protective of my creations, \
 *  especially when i worked hard on this) \
 * (well a video did get stolen from me \
 *  netting 10 million views uncredited)
 *
 * Dependencies: C standard library, Bitstreams
 *
 * Lazy copy from testing code
 * Also messy
 *
 * Compression and decompression
 * is about 300 lines
 *
 * massive TODO: Decompression is slow
 * Iterate through symbol codes and
 * their lengths instead of brute force
 * through i < 65536 & j < 16
 * Might require multibit bitstr_get
 *
 * maybe FIX: Allocating 256 nodes twice (*tempdata,*nodes,*tree) in HFM_compress
 * could use a pointer array or something
 *
 * FIX: one or two specific files crash with this
 *
 * Worked on from about October 2021
 * to November/December 2021
 * Fixed up in February 2022
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define hard_cast(t,i) *(t*)&(i)
// hard_cast(uint32_t,byte3)

//#define NO_INLINES
#define HFM_compress_log 0
#define HFM_decompress_log 0
#if (HFM_compress_log || HFM_decompress_log)
#define BITSTR_STDIO
#endif
#include "bitstr.c" // wtf
#include "bitstr.h"

typedef struct _HFM_NODE
{
	char symbol;
	uint32_t frequency;
	struct _HFM_NODE*childA;
	struct _HFM_NODE*childB;
} _HFM_NODE;
typedef struct _HFM_SYM
{
	char data;
	char codeL; // bit count
	char code;  //
	char codeh; // WHY DOES DECLARING THIS A SHORT MAKE IT MORE THAN TWO BYTES LARGER
				// (havent tested if this still happens after moving length before these)
				// will attr packed fix it
} _HFM_SYM;
typedef struct _HFM_DATA
{
	char*data;
	size_t dataSize;
	struct _HFM_SYM*code;
} _HFM_DATA;
typedef struct _HFM_FILE
{
	char magic[3];
	char codeCount;
	size_t dataSize;
	struct _HFM_SYM*code;
	char*data;
} _HFM_FILE;

/// memory optimizer-ish, also for use with next function
/// tiny c ruined the fun in this function
/// this conversion is stupid because one struct has symbol data spaced out instead of all together
/// and wastes more stuff and adds an extra step
_HFM_FILE HFM_tofile(_HFM_DATA data)
{
	_HFM_FILE out;
	hard_cast(uint32_t,out.magic) = 0x004D4648; // "HFM\0"
	out.code = (_HFM_SYM*)calloc(sizeof(_HFM_SYM),256); // SAME SIZE AS DWORD, JUST CAST IT STUPID!
	for (uint16_t i = 0; i < 256; i++)
	{
		if ((data.code[i].codeL) > 0)
		{
			memcpy(&(((char*)out.code)[out.codeCount*sizeof(_HFM_SYM)]), &(((char*)data.code)[i*sizeof(_HFM_SYM)]), sizeof(_HFM_SYM));
			out.codeCount++;
		}
	}
	out.data = data.data;
	out.dataSize = data.dataSize;
	
	return out;
}
_HFM_DATA HFM_toraw(_HFM_FILE data)
{
	_HFM_DATA out;
	out.code = (_HFM_SYM*)calloc(sizeof(_HFM_SYM),256);
	for (uint16_t i = 0; i < data.codeCount; i++)
	{
		memcpy(&(((char*)out.code)[data.code[i].data*sizeof(_HFM_SYM)]),
				&(((char*)data.code)[i*sizeof(_HFM_SYM)]),
				sizeof(_HFM_SYM));
	}
	out.data = data.data;
	out.dataSize = data.dataSize;
	
	return out;
}
char wtfff[8];
void HFM_fwrite(_HFM_FILE file, char*fname)
{
	FILE*newf;
	newf = fopen(fname, "wb");
	
	fwrite(&file, 1, 8, newf);
	fwrite(((uint32_t*)file.code), file.codeCount, 4, newf);
	fwrite(file.data, (file.dataSize>>3)+1, 1, newf);
	
	if (ftell(newf)&7 != 0)
		fwrite(&wtfff, 8-(ftell(newf)&7), 1, newf); // how to put zero padding without allocating?
	fclose(newf);
}
_HFM_FILE HFM_fread(char*fname)
{
	FILE*file;
	file = fopen(fname, "rb");
	_HFM_FILE data;
	
	fread(&data, 1, 8, file);
	data.code = (_HFM_SYM*)calloc(data.codeCount, 4);
	fread(data.code, sizeof(_HFM_SYM), data.codeCount, file);
	uint32_t filecur = 0;
	data.data = (char*)calloc(sizeof(char), data.dataSize);
	for(int i = 0; i < data.dataSize; i++)
	{
		fread((data.data+filecur), 1, 1, file);
		if (feof(file))
			break;
		filecur++;
	}
	
	fclose(file);
	
	return data;
}
_HFM_DATA HFM_compress(const char*data,size_t size)
{
	_HFM_NODE*tempdata = (_HFM_NODE*)calloc(sizeof(_HFM_NODE),256);
	#if HFM_compress_log==1
	puts("\nData:");
	#endif
	for(size_t i=0;i<size;i++)
	{
		#if HFM_compress_log==1
		printf("%c",data[i]);
		#endif
		tempdata[data[i]].frequency++;
	}
	_HFM_NODE*nodes = (_HFM_NODE*)calloc(sizeof(_HFM_NODE),256);
	uint32_t nodeplace = 0;
	// WEIRD SORTING, IT WORKS, WHATEVER, OKAY?
	// (i dont even know if anyone else does it this way)
	// i mean this is a test after all, with doing it on my own
	// now im certified for the tech wing of the military
	#if HFM_compress_log==1
	puts("\n\nCreating and sorting nodes...\nNodes:");
	#endif
	for(uint32_t j=0;j<size;j++)
	{
		for(uint8_t i=0;i<255;i++)
		{
			if(tempdata[i].frequency)
			{
				if(tempdata[i].frequency == j)
				{
					nodes[nodeplace] = tempdata[i]; // aaaaaaaaa
					nodes[nodeplace].symbol = i;
					nodeplace++;
				}
			}
		}
	}
	#if HFM_compress_log==1
	for(uint8_t i=0;i<nodeplace;i++)
	{
		printf("%c : %d\n",nodes[i].symbol,nodes[i].frequency);
	}
	puts("");
	#endif
	_HFM_NODE*tree = (_HFM_NODE*)calloc(sizeof(_HFM_NODE),nodeplace<<1);
	_HFM_NODE tempnew, mainnode;
	memcpy(tree,nodes,sizeof(_HFM_NODE)*nodeplace-1);
	uint16_t treesize = nodeplace, treecursor = nodeplace;
	// from 0 is from bottom and goes to left
	uint32_t Fmaxmin[2] = { 0, 0 };
	uint32_t FmmID[3] = { 0, 1, 0 };
	char*exclude = (char*)calloc(sizeof(char),nodeplace<<1);
	#if HFM_compress_log==1
	puts("Creating tree...");
	#endif
	while(1)
	{
		{
			FmmID[2] = FmmID[0];
			Fmaxmin[0] = -1;
			Fmaxmin[1] = -1;
			for(uint32_t i=0;i<treecursor;i++)
			{
				if(tree[i].frequency>Fmaxmin[1] && !exclude[i])
				{
					Fmaxmin[1] = tree[i].frequency;
					FmmID[1] = i;
				}
			}
			for(uint32_t i=0;i<treecursor;i++)
			{
				if(tree[i].frequency<Fmaxmin[0] && !exclude[i])
				{
					Fmaxmin[0] = tree[i].frequency;
					FmmID[0] = i;
				}
			}
			for(uint32_t i=0;i<treecursor;i++)
			{
				if(tree[i].frequency<Fmaxmin[1] && !exclude[i] && i != FmmID[0] && tree[i].frequency>=Fmaxmin[0])
				{
					Fmaxmin[1] = tree[i].frequency;
					FmmID[1] = i;
				}
			}
		}
		#if HFM_compress_log==1
		printf("\n\nSelectors:\n%u , %u : %u , %u\n\n",FmmID[0],FmmID[1],Fmaxmin[0],Fmaxmin[1]);
		#endif
		if (Fmaxmin[1] == -1)
		{
			#if HFM_compress_log==1
			puts("End of tree");
			#endif
			tempnew.frequency = 
				Fmaxmin[0];
			tempnew.childA = &tree[FmmID[2]];
			tempnew.childB = &tree[FmmID[1]];
			treecursor--;
			tree[treecursor] = tempnew;
			mainnode = tempnew;
			break;
		}
		exclude[FmmID[0]] = 1;
		exclude[FmmID[1]] = 1;
		#if HFM_compress_log==1
		puts("Nodes:\n");
		for(uint32_t i=0;tree[i].frequency>0;i++) {
			if (!tree[i].childA && !tree[i].childB)
				printf("%c        :      %3u | ",tree[i].symbol,tree[i].frequency,tree[i]);
			else
				printf("(tree)   :      %3u | ",tree[i].frequency);
		}
		puts("");
		for(uint32_t i=0;tree[i].frequency>0;i++)
		{
			if (tree[i].childA || tree[i].childB)
				printf("%c        :        %c | ",*(char*)(tree[i].childA),*(char*)(tree[i].childB));
			else
				printf("         :          | ");
		}
		#endif
		tempnew.frequency =
			tree[FmmID[0]].frequency +
			tree[FmmID[1]].frequency;
		tempnew.symbol = 0;
		tempnew.childA = &tree[FmmID[0]];
		tempnew.childB = &tree[FmmID[1]];
		tree[treecursor] = tempnew;
		treecursor++;
	}
	#if HFM_compress_log==1
	puts("Generating codes...\n");
	#endif
	uint32_t treecode, depth, nodeplace2 = 0;
	_HFM_NODE*codenodecursor;
	_HFM_SYM*codes = (_HFM_SYM*)calloc(sizeof(_HFM_SYM),256);
	exclude = (char*)calloc(sizeof(char),256);
	for (uint32_t i = 0; i < 65536; i++) // really? it's fast anyway...
	// i should add a max depth value for earlier code
	{
		codenodecursor = &mainnode;
		for (uint8_t j = 0; j < 16; j++)
		{
			if ((i >> j) & 1 == 1)
			{
				if (codenodecursor->childB)
					codenodecursor = codenodecursor->childB;
				else
					goto newsym;
			}
			else
			{
				if (codenodecursor->childA)
					codenodecursor = codenodecursor->childA;
				else
					goto newsym;
			}
			if (0)
			{
				// lol
			newsym:
				if (!exclude[codenodecursor->symbol])
				{
					exclude[codenodecursor->symbol] = 1;
					codes[codenodecursor->symbol].data = codenodecursor->symbol;
					hard_cast(uint16_t,codes[codenodecursor->symbol].code) = i;
					codes[codenodecursor->symbol].codeL = j;
					#if HFM_compress_log==1
					printf("Reached end:\n%c: %u,%u\n",
						codes[codenodecursor->symbol].data,
						hard_cast(uint16_t, codes[codenodecursor->symbol].code),
						codes[codenodecursor->symbol].codeL);
					#endif
					nodeplace2++;
				}
				break;
			}
		}
		if (nodeplace2 == nodeplace)
			break;
	}
	_HFM_DATA out;
	out.code = codes;
	#if HFM_compress_log==1
	puts("");
	for(size_t i=0;i<size;i++)
	{
		printf("%c    ", data[i]);
	}
	puts("");
	for(size_t i=0;i<size;i++)
	{
		printf("%u%u%u%u ",
			(hard_cast(uint16_t,codes[data[i]].code) >> 0) & 1,
			(hard_cast(uint16_t,codes[data[i]].code) >> 1) & 1,
			(hard_cast(uint16_t,codes[data[i]].code) >> 2) & 1,
			(hard_cast(uint16_t,codes[data[i]].code) >> 3) & 1
		);
		printf(" ");
	}
	puts("");
	for(size_t i=0;i<size;i++)
	{
		printf("%4u ", codes[data[i]].codeL);
	}
	#endif
	size_t k = 0;
	out.data = (char*)calloc(sizeof(char),size);
	out.dataSize = 0;
	#if HFM_compress_log==1
	puts("\n\nOutput:");
	#endif
	for(size_t i=0;i<size;i++)
	{
		for(size_t j=0;j<codes[data[i]].codeL;j++)
		{
			#if HFM_compress_log==1
			printf("%u", (hard_cast(uint16_t,codes[data[i]].code) >> j) & 1);
			#endif
			out.data[k>>3] |= ((hard_cast(uint16_t,codes[data[i]].code) >> j) & 1) << (7-(k&7));
			k++;
			out.dataSize++;
		}
	}
	// optimize bitwise checking to use own bitstr funcs?
	#if HFM_compress_log==1
	puts("");
	for(size_t i=0;i<out.dataSize;i++)
	{
		printf("%u", out.data[i>>3] >> (7-(i&7)) & 1);
	}
	puts("");
	// cant see how to compare this with actual binary data to hex
	// to see if i messed up the bitwise or not,
	// despite printing back what i put in bit for bit
	puts("\n");
	printf("Compressed from %u bytes to %u bytes\nWith %u additional bytes for decompression (in a file)\n", size, out.dataSize>>3, (8+(nodeplace<<2)));//, _itof(size)/(_itof(out.dataSize)/8.0f));
	puts("Done\n");
	#endif
	//free(codes); // goes in _HFM_DATA
	free(exclude);
	free(tempdata);
	free(nodes);
	free(tree);
	return out;
	// should i evade using inline/static struct returns or whatever
	// aka make the return value a pointer to the new struct
	// most likely
	// also TCC inserts a memcpy when this is done
}
char*HFM_decompress(_HFM_DATA data)
{
	char*out = (char*)calloc(1, data.dataSize);
	#if HFM_compress_log==1
	for(size_t i=0;i<(data.dataSize>>3)+1;i++)
		printf("%02X ", data.data[i]);
	puts("");
	for(size_t i=0;i<data.dataSize;i++)
		printf("%u", data.data[i>>3] >> (7-(i&7)) & 1);
	puts("");
	#endif
	{
		uint16_t codebuf = 0, codebuf2 = 0, cutoff = 0;
		char decode = 0, curbit = 0, curbit2 = 0;
		uint32_t parsed = 0, bitplace = 0;
		while(bitplace<data.dataSize)
		{
			for (uint16_t x = 0; x < 65535; x++)
			{
				codebuf = 0;
				codebuf2 = 0;
				cutoff = 0;
				for (char y = 0; y < 16; y++)
				{
					curbit2 = bitstr_get(data.data, y+bitplace);
					curbit  = hard_cast(uint16_t,data.code[x].code) >> y & 1;
					if (curbit2)
						bitstr_set((char*)&codebuf2, y);
					if (curbit)
						bitstr_set((char*)&codebuf, y);
					if (y <= 8)
						cutoff |= 1 << (8-y);
					else
						cutoff |= 1 << (8-y+16);
					if (data.code[x].codeL)
					{
						if (data.code[x].codeL == y)
						{
							if ((codebuf & cutoff) == (codebuf2 & cutoff) && data.code[x].codeL == y)
							{
								#if HFM_decompress_log==1
								codebuf &= cutoff;
								codebuf2 &= cutoff;
								printf("Matching code: ");
								bitstr_print((char*)&codebuf2,16);
								printf(" in %2u bits @ bit %9u of %9u & placing @ byte %7u: %c\n", y, bitplace, data.dataSize, parsed, x);
								#endif
								decode = x;
								bitplace += data.code[x].codeL;
								goto foundCode;
							}
						}
					}
				}
			}
			fail:
			#if HFM_decompress_log==1
			puts("Code not found!\n"); // move on anyway xd // definitely should still continue if there's an error // but then get caught in an infinite loop possibly
			bitplace++; // ???
			#endif
			foundCode:
			out[parsed] = decode;
			parsed++;
		}
	}
	return out;
}

#if (1)
const int   __HFM_TEST_wesleyL = 63;
const char* __HFM_TEST_wesleyS =
	"WESLEY_SELWEY_WELSEY_LESLEY__ESLWEY_WESLEY_LELSWE_YESLEY_WESLEY";
_HFM_DATA   __HFM_TEST_wesleyC;
_HFM_FILE   __HFM_TEST_wesleyF;
char*       __HFM_TEST_wesleyD;

int  randomL;
char*random;
FILE*randomFF;
_HFM_DATA randomC;
_HFM_FILE randomF;
char*randomD;

int _start()
{
	puts("Huffman test");
	puts(__HFM_TEST_wesleyS);
	puts("COMPRESSED:");
	__HFM_TEST_wesleyC = HFM_compress(__HFM_TEST_wesleyS,__HFM_TEST_wesleyL);
	puts(__HFM_TEST_wesleyC.data);
	puts("\nDECOMPRESSED:");
	__HFM_TEST_wesleyD = HFM_decompress(__HFM_TEST_wesleyC);
	puts(__HFM_TEST_wesleyD);
	
	puts("Load file");
	randomFF = fopen(__FILE__,"rb");
	fseek(randomFF,0,SEEK_END);
	randomL = ftell(randomFF);
	rewind(randomFF);
	random = (char*)malloc(randomL);
	fread(random,1,randomL,randomFF);
	fclose(randomFF);
	puts(random);
	puts("COMPRESSED:");
	randomC = HFM_compress(random,randomL);
	puts(randomC.data);
	puts("\nWriting to file");
	randomF = HFM_tofile(randomC);
	HFM_fwrite(randomF, __FILE__".hfm");
	puts("\nDECOMPRESSED:");
	randomD = HFM_decompress(randomC);
	puts(randomD);
}

#endif


