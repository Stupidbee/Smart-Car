#ifndef   FAT16_H
#define   FAT16_H

#include"MC9S12XS128.h"

typedef struct
{
	byte BS_jmpBoot[3];       //jumpinstruction
	byte BS_OEMName[8];       //system name
	word BPB_BytesPerSec;     //logical bytes per sector
	byte BPB_SecPerClus;      //sectors per cluster
	word BPB_RsvdSecCnt;      //number of reserved sectors starting from 0
	byte BPB_NumFATs;         //number of FATs
	word BPB_RootEntCnt;      //number of root dir entries
	word BPB_TotSec16;        //total sectors. 0 if ofs 15h used 
	byte BPB_Media;           //media descriptor 
	word BPB_FATSz16;         //number of sectors per FAT
	word BPB_SecPerTrk;       //number of logical sectors per track
	word BPB_NumHeads;        //number of logical heads
	dword BPB_HiddSec;        //number of hidden sectors
	dword BPB_TotSec32;       //total sectors
	byte BS_DrvNum;           //physical drive number
	byte BS_Reservedl;        //reserved
	byte BS_BootSig;          //signature byte for extended boot record
	dword BS_VolID;           //serial number
	byte BS_VolLab[11];       //label
	byte BS_FilSysType[8];    //FAT type 
	//byte ExecutableCode[448];
	//byte ExecutableMarker[2];
} FAT_BPB;

typedef struct
{
	byte NAME[8];
	byte TYPE[3];
} FILE_NAME;

typedef struct
{
	word Start;
	dword Size;
} FILE_POSIT;

typedef struct
{
	FILE_NAME FileName;     //name
	byte FileAttrib;        //extension
	byte UnUsed[10];        //flags and reserved
	byte FileUpdateTime[2]; //time and data
	byte FileUpdateData[2]; //first cluter
	FILE_POSIT FilePosit;   //size
} DIR;

typedef struct
{
	word ClusID;
	word SecOfClus;
	word ByteOfSec;
} DATA_POSIT;

/* File object structure */
typedef struct _FIL 
{
  word	Id;				       /* Owner file system mount ID */
	byte	Csect;			     /* Sector address in the cluster */
	dword	Fsize;			     /* File size */
	word	Org_clust;	     /* File start cluster */
	word	Curr_clust;	     /* Current cluster */
  word  Curr_clustnum;   /* Current cluster */
	dword	Curr_sect;       /* Current sector */
} FIL;

//********************************************************************************************
//读一个扇区
void ReadBlock(dword LBA);
//********************************************************************************************
//写一个扇区
void WriteBlock(dword LBA);
//********************************************************************************************
void CopyBytes(byte *s, byte *d, word size);
//********************************************************************************************
byte IsEqual(void *A, void *B, byte Size);
//********************************************************************************************
void EmptyBytes(byte *data, word size);
//********************************************************************************************
//写BPB数据结构
void FATInit(void);
//********************************************************************************************
//读取BPB数据结构
void ReadBPB(void);
//********************************************************************************************
//获取FAT表开始扇区号
dword FATStartSec(void);
//********************************************************************************************
//获取根目录开始扇区号
dword DirStartSec(void);
//********************************************************************************************
//获取数据区开始扇区号
dword DataStartSec(void);
//********************************************************************************************
//目录项占用的扇区数
word GetDirSecCount(void);
//********************************************************************************************
//获取一个簇的开始扇区
dword ClusConvLBA(word ClusID);
//********************************************************************************************
//读取文件分配表的指定项
word ReadFAT(word Index, unsigned char flag);
//********************************************************************************************
//写文件分配表的指定项
void WriteFAT(word Index, word Value);
//********************************************************************************************
//写备份文件分配表
void WriteFAT2(word Index, word Value);
//********************************************************************************************
//获取根目录中可以使用的一项
word GetEmptyDIR(void);
//********************************************************************************************
//获得和文件名对应的目录项
byte GetFileID(byte Name[11], DIR* ID);
//********************************************************************************************
//获取一个空的FAT项
word GetNextFAT(word FATnum);
//********************************************************************************************
//读取根目录的指定项
void ReadDIR(word Index, DIR* Value);
//********************************************************************************************
//写根目录的指定项
void WriteDIR(word Index, DIR* Value);
//********************************************************************************************
//创建一个空文件
void CreateFile(byte *FileName, FIL *file, DIR *FileDir);
//********************************************************************************************
//复制文件分配表,使其和备份一致
void CopyFAT(void);
//********************************************************************************************
//操作文件的数据
void OperateFile(byte Write ,byte Name[11], dword Start, dword Length, byte* Data, FIL *file,DIR *FileDir);
//********************************************************************************************
//删除文件
void DelFile(byte Name[11]);
//********************************************************************************************
//格式化
void FormatCard(void);
//********************************************************************************************
//---------------------------------------------------------------------------------------------------------------------------------
#endif