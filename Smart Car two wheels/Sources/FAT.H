
byte BUFFER[512];
byte BPB_SecPerClus;
byte BPB_NumFATs;

word BPB_BytesPerSec;
word BPB_RsvdSecCnt;
word BPB_RootEntCnt;
word BPB_TotSec16;
word BPB_FATSz16;

dword BPB_HiddSec;
dword BPB_Firstsector;

//********************************************************************************************
//读一个扇区
void ReadBlock(dword LBA)
//********************************************************************************************
{
 	Rd_Block(LBA,BUFFER);
  return;
}

//********************************************************************************************
//写一个扇区
void WriteBlock(dword LBA)
//********************************************************************************************
{
	Wt_Block(LBA,BUFFER);
  return;
}

//********************************************************************************************
void CopyBytes(byte *s, byte *d, word size)
//********************************************************************************************
{
	word i;
	for(i = 0; i < size; i++)
		*d++ = *s++;
}

//********************************************************************************************
byte IsEqual(void* A, void* B, byte Size)
//********************************************************************************************
{
	byte i, *a = A, *b = B;
	for(i = 0; i < Size; i++)
		if(a[i] != b[i])
			return 0;
	return 1;
}

//********************************************************************************************
void EmptyBytes(byte *data, word size)
//********************************************************************************************
{
	word i;
	for(i = 0; i < size; i++)
	{
		*data++ = 0;
	}
}
//********************************************************************************************
//写BPB数据结构
void FATInit(void)
//********************************************************************************************
{
	FAT_BPB* BPB = (FAT_BPB*)BUFFER;
		//参数
	BPB->BPB_BytesPerSec = 0x0200;//每个扇区512字节
	BPB->BPB_SecPerClus  = 0x40;  //每个簇64个扇区
	BPB->BPB_RsvdSecCnt  = 0x0006;//一个保留扇区
	BPB->BPB_NumFATs     = 0x02; //2个FAT表
	BPB->BPB_RootEntCnt  = 0x0200;//512目录项个数
	BPB->BPB_TotSec16    = 0x0000;//总扇区数
	BPB->BPB_FATSz16     = 0xF1;//一个FAT表所占的扇区数
	BPB->BPB_HiddSec     = 0x00000000;  //本FAT表前隐藏的扇区数
	
	WriteBlock(0);//写引导区
}
//********************************************************************************************
//读取BPB数据结构
void ReadBPB(void)
//********************************************************************************************
{
	FAT_BPB* BPB = (FAT_BPB*)BUFFER;
	
	ReadBlock(0);
  if(BUFFER[0] != 0xEB && BUFFER[0] != 0xE9)
  {
    BPB_Firstsector = BUFFER[454];
    ReadBlock(BPB_Firstsector);
  }
	
	//获取参数 将小端模式的数据转换过来
	BPB_BytesPerSec = BPB->BPB_BytesPerSec>>8|BPB->BPB_BytesPerSec<<8;
	BPB_SecPerClus  = BPB->BPB_SecPerClus;
	BPB_RsvdSecCnt  = BPB->BPB_RsvdSecCnt>>8|BPB->BPB_RsvdSecCnt<<8;
	BPB_NumFATs     = BPB->BPB_NumFATs;
	BPB_RootEntCnt  = BPB->BPB_RootEntCnt>>8|BPB->BPB_RootEntCnt<<8;
	BPB_TotSec16    = BPB->BPB_TotSec16>>8|BPB->BPB_TotSec16<<8;
	BPB_FATSz16     = BPB->BPB_FATSz16>>8|BPB->BPB_FATSz16<<8;
	BPB_HiddSec     = BPB->BPB_HiddSec>>16>>8|BPB->BPB_HiddSec>>16<<8|BPB->BPB_HiddSec<<16>>8|BPB->BPB_HiddSec<<16<<8;
}
//********************************************************************************************
//获取FAT表开始扇区号
dword FATStartSec(void)
//********************************************************************************************
{
	return BPB_RsvdSecCnt + BPB_Firstsector;
}
//********************************************************************************************
//获取根目录开始扇区号
dword DirStartSec(void)
//********************************************************************************************
{
	return FATStartSec() + BPB_NumFATs * BPB_FATSz16;
}

//********************************************************************************************
//获取数据区开始扇区号
dword DataStartSec(void)
//********************************************************************************************
{
	return DirStartSec() + BPB_RootEntCnt * 32 / BPB_BytesPerSec;
}

//********************************************************************************************
//目录项占用的扇区数
word GetDirSecCount(void)
//********************************************************************************************
{
	return BPB_RootEntCnt * 32 / 512;
}

//********************************************************************************************
//获取一个簇的开始扇区
dword ClusConvLBA(word ClusID)
//********************************************************************************************
{
	return DataStartSec() + BPB_SecPerClus * (ClusID - 2);
}

//********************************************************************************************
//读取文件分配表的指定项
word ReadFAT(word Index,unsigned char flag)
//********************************************************************************************
{
	if(flag)ReadBlock(BPB_RsvdSecCnt + Index / 256 + BPB_Firstsector); 
	return BUFFER[Index % 256 * 2] + BUFFER[Index % 256 * 2 + 1] * 256;
}

//********************************************************************************************
//写文件分配表的指定项
void WriteFAT(word Index, word Value)
//********************************************************************************************
{
	dword SecID;
	SecID = BPB_RsvdSecCnt + Index / 256 + BPB_Firstsector;
	ReadBlock(SecID);	
	BUFFER[Index % 256 * 2] = Value % 256;
	BUFFER[Index % 256 * 2 + 1] = Value / 256;
	WriteBlock(SecID);
}
//********************************************************************************************
//写FAT2
void WriteFAT2(word Index, word Value)
//********************************************************************************************
{
	word *RAM = (word*)BUFFER;
	dword SecID;
	SecID = BPB_RsvdSecCnt + BPB_FATSz16+ Index / 256 + BPB_Firstsector;
	ReadBlock(SecID);
  BUFFER[Index % 256 * 2] = Value % 256;
	BUFFER[Index % 256 * 2 + 1] = Value / 256;
	WriteBlock(SecID);
}
//********************************************************************************************
//获取根目录中可以使用的一项
word GetEmptyDIR()
//********************************************************************************************
{
	word DirSecCut, DirStart, i, j, ID = 0;
	DirSecCut = GetDirSecCount();
	DirStart = DirStartSec();
	for(i = 0; i < DirSecCut; i++)
	{
		ReadBlock(DirStart + i);
		for(j = 0; j < 16; j++)
		{
			if((BUFFER[j * 32] == 0) || (BUFFER[j * 32] == 0xE5))	return ID;
			else ID++;
		}
	}
	return ID;
}

//********************************************************************************************
//获得和文件名对应的目录项以及该目录的位置
byte GetFileID(byte Name[11], DIR* ID)
//********************************************************************************************
{
	word DirSecCut, DirStart, i, m,Index;
	DirSecCut = GetDirSecCount();
	DirStart = DirStartSec();
	for(i = 0; i < DirSecCut; i++)
	{
		ReadBlock(DirStart + i);
		for(m = 0; m <16; m++)
		{
			if(IsEqual(Name, &((DIR*)&BUFFER[m * 32])->FileName, 11))
			{
				*ID = *((DIR*)&BUFFER[m * 32]);
				Index = i*16+m;
				return Index; //找到对应的目录项,返回目录的位置.
			}
		}
	}
	return 0; //没有找到对应的目录项,返回0.
}

//********************************************************************************************
//获取一个空的FAT项
word GetNextFAT(word FATnum)
//********************************************************************************************
{
	word FAT_Count,i;
	FAT_Count = BPB_FATSz16 * 256; //FAT表总项数 FAT16为2个字节
	for(i = FATnum; i < FAT_Count; i++)
	{
		if(ReadFAT(i,i % 256 == 0 || i == FATnum) == 0) return i;
	}
	return 0;
}

//********************************************************************************************
//读取根目录的指定项??????????????????
void ReadDIR(word Index, DIR* Value)
//********************************************************************************************
{
	dword DirStart = DirStartSec();
	ReadBlock(DirStart + Index / 16);
	CopyBytes(&BUFFER[(Index % 16) * 32], Value, 32);
}

//********************************************************************************************
//写根目录的指定项???????????????????
void WriteDIR(word Index, DIR* Value)
//********************************************************************************************
{
	dword LBA = DirStartSec() + Index / 16;
	ReadBlock(LBA);
	CopyBytes(Value, &BUFFER[(Index % 16) * 32], 32);
	WriteBlock(LBA);
}
//********************************************************************************************
//创建一个空文件
//说明:FileName     文件名必须是数字或者大写字母，扩展名也是如此
//文件大小事先不用设置,均为0;
void CreateFile(byte *FileName, FIL *file, DIR *FileDir)
//********************************************************************************************
{                                                             
	word ClusID;
  word NextFAT;
        
	EmptyBytes(FileDir, sizeof(DIR));
  CopyBytes(FileName, &(FileDir -> FileName), 11);
        
  FileDir -> FileAttrib = 0x20;//归档    
	FileDir -> FilePosit.Size = 0xffffff00;
	//FileDir -> FilePosit.Size = FileDir -> FilePosit.Size>>16>>8|FileDir -> FilePosit.Size>>16<<8|FileDir -> FilePosit.Size<<16>>8|FileDir -> FilePosit.Size<<16<<8;
	ClusID = 2;
	FileDir -> FilePosit.Start = ClusID>>8|ClusID<<8;   
  
  file -> Fsize = 0;
  file -> Org_clust = ClusID;
  file -> Curr_clust = ClusID;
  file -> Curr_sect = ClusConvLBA(ClusID);
  file -> Id = 0;// GetEmptyDIR();//
  file -> Curr_clustnum = 1;
  
//WriteFAT(ClusID, 0xffff);
  //WriteFAT2(ClusID, 0xffff);
 //	WriteDIR(file -> Id, FileDir);
}
//********************************************************************************************
//删除文件
void DelFile(byte Name[11])
{
  DIR FileDir;
  word ClusID,Index;
  Index = GetFileID(Name,&FileDir);
  FileDir.FileName.NAME[0] = 0xE5; 
  ClusID =  FileDir.FilePosit.Start>>8| FileDir.FilePosit.Start<<8;
  WriteFAT(ClusID,0x0000);
  WriteFAT2(ClusID,0x0000);
  WriteDIR(Index,&FileDir);     
}
//********************************************************************************************
//********************************************************************************************
//复制文件分配表,使其和备份一致
void CopyFAT(void)
//********************************************************************************************
{
	word FATSz16, RsvdSecCnt, i;
	FATSz16 = BPB_FATSz16;
	RsvdSecCnt = BPB_RsvdSecCnt + BPB_Firstsector;
	for(i = 0; i < FATSz16; i++)
	{
		ReadBlock(RsvdSecCnt + i);
		WriteBlock(RsvdSecCnt + FATSz16 + i);
	}
}

//********************************************************************************************
//操作文件的数据
//说明：Write       1时是写操作,0为读操作
//      Name        文件名支持数字和大写字母
//      Start       开始字节
//      Length      操作长度
//      Data        操作数据
//存在的BUG是，如果如果一次写入数据量大于一个簇的字节数，FAT表分配的时候会出现错误

void OperateFile(byte Write ,byte Name[11], dword Start, dword Length, byte* data, FIL *file,DIR *FileDir)
//********************************************************************************************
{
	dword BytePerClus, SecPerClus, ClusNum,ClusNext,ClusID,LBA,i,j;
	

	SecPerClus = BPB_SecPerClus;
	BytePerClus = BPB_SecPerClus * 512; //每簇的字节数

	//计算开始位置所在簇的簇号
	ClusNum = file -> Fsize / BytePerClus + 1;
	ClusID = file -> Curr_clust;
   
  //一次只能增加一簇
	for(i = file -> Curr_clustnum; i < ClusNum; i++)
	{
     ClusNext = file -> Curr_clust + 1;//GetNextFAT(file -> Curr_clust + 1);     
		 //WriteFAT(ClusID,ClusNext);
     //WriteFAT2(ClusID,ClusNext);
	   
	   file -> Curr_clust = ClusNext;
	   ClusID = ClusNext;
	   //WriteFAT(ClusID,0xffff);
     //WriteFAT2(ClusID,0xffff);
     file -> Curr_clustnum++;
	}
   
  //更新文件大小
  file -> Fsize += Length; 	
	//FileDir -> FilePosit.Size = file -> Fsize>>16>>8|file -> Fsize>>16<<8|file -> Fsize<<16>>8|file -> Fsize<<16<<8;
	//WriteDIR(file -> Id,FileDir);
	
	//计算开始位置所在扇区簇内偏移
	i = (Start % BytePerClus) / 512;

	//计算开始位置扇区内偏移
	j = (Start % BytePerClus) % 512;

	LBA = ClusConvLBA(ClusID) + i;
  
  WriteBlock(LBA);
  /*
	for(j=0; j < 512; j++)
	{
		if(Write) BUFFER[j] = *data++;
		else *data++ = BUFFER[j];

		//如果读取完成就退出
		if(--Length == 0)
		{
			if(Write) WriteBlock(LBA); //回写扇区
			return;
		}
	}
	*/
	
}


//********************************************************************************************
//格式化
//不是真正意义上的格式化，只是清除根目录以及FAT表
void FormatCard(void)
//********************************************************************************************
{
    word i,DirStart,DirSecNum,FatStart,FatSecNum;
   
    ReadBPB();
    //清除根目录
    for(i = 0; i <= 512;i++ )BUFFER[i]=0;
    DirSecNum = GetDirSecCount();
    DirStart = DirStartSec();
    for(i=0;i<DirSecNum;i++)
    {
      WriteBlock(DirStart+i);
    }
    
    //清除FAT表
    FatStart = BPB_RsvdSecCnt + BPB_Firstsector;
    FatSecNum = BPB_FATSz16;
    for(i=0;i<FatSecNum;i++)
    {
      WriteBlock(FatStart+i);
    }
    WriteFAT(0,0xf8ff);
    WriteFAT(1,0xffff);
    
    //备份FAT表
    CopyFAT();
}

//********************************************************************************************
//---------------------------------------------------------------------------------------------------------------------------------
        
        
