
#include <stdio.h>
#include "dmfont.h"

#define  ASC16NAME "ASC16"
#define  HZK16NAME "HZK16"

FILE *m_hZM_ASC16;
FILE *m_hZM_HZK16;

unsigned char asczm[16]; //字符点阵缓冲器
unsigned char hzkzm[32]; //汉字点阵缓冲期

/*************/
//文件I/O
/*************/
int OpenZMFile ()
{
	m_hZM_ASC16 = fopen(ASC16NAME,"r" );
	m_hZM_HZK16 = fopen(HZK16NAME,"r" );

	if (m_hZM_ASC16 == NULL || m_hZM_ASC16 == NULL)
	{
		return 0;
	}

	return 1;
}

int CloseZMFile ()
{
	fclose(m_hZM_ASC16);
	fclose(m_hZM_HZK16);
	return 1;
}

int GetZM_ASC16(unsigned char asciinum, unsigned char * zmbuf)
//ASCII码值 16字节字模缓冲区(ASCII字模是8×16结构的)
{
	int startpoit; //字符首地址
	startpoit = asciinum * 16;
	fseek (m_hZM_ASC16 , startpoit , SEEK_SET);//字模位置定位
	fread ( (char *)zmbuf, 16 , 1, m_hZM_ASC16);
	return 1;
}

int GetZM_HZK16(unsigned char * hznum, unsigned char * zmbuf)
{
//	unsigned char hz[] = "李";
	int qm,wm,startpoit;
	qm= hznum[0];		/*通过区位码计算其在hzk16f文件中的偏移地址*/
	qm=qm-161;
	if (qm>87) return 0;
	wm=hznum[1];
	wm=wm-161;
	if (wm>94) return 0;
	startpoit=((long) qm * 94 + wm ) * 32;
	fseek (m_hZM_HZK16,startpoit,SEEK_SET);//定位
	fread ((char *)zmbuf , 32, 1, m_hZM_HZK16); //16*16点阵对应32个字节

	return 1;
}


/*****************TEST******************/
void testfont()
{
	if (OpenZMFile ())
	 {
		 unsigned char zh[] = "十";
		 GetZM_HZK16(zh,hzkzm);
		 GetZM_ASC16('M', asczm);
	 }

	CloseZMFile ();
}
	
