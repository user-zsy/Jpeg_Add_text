
#include <stdio.h>
#include "dmfont.h"

#define  ASC16NAME "ASC16"
#define  HZK16NAME "HZK16"

FILE *m_hZM_ASC16;
FILE *m_hZM_HZK16;

unsigned char asczm[16]; //�ַ����󻺳���
unsigned char hzkzm[32]; //���ֵ��󻺳���

/*************/
//�ļ�I/O
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
//ASCII��ֵ 16�ֽ���ģ������(ASCII��ģ��8��16�ṹ��)
{
	int startpoit; //�ַ��׵�ַ
	startpoit = asciinum * 16;
	fseek (m_hZM_ASC16 , startpoit , SEEK_SET);//��ģλ�ö�λ
	fread ( (char *)zmbuf, 16 , 1, m_hZM_ASC16);
	return 1;
}

int GetZM_HZK16(unsigned char * hznum, unsigned char * zmbuf)
{
//	unsigned char hz[] = "��";
	int qm,wm,startpoit;
	qm= hznum[0];		/*ͨ����λ���������hzk16f�ļ��е�ƫ�Ƶ�ַ*/
	qm=qm-161;
	if (qm>87) return 0;
	wm=hznum[1];
	wm=wm-161;
	if (wm>94) return 0;
	startpoit=((long) qm * 94 + wm ) * 32;
	fseek (m_hZM_HZK16,startpoit,SEEK_SET);//��λ
	fread ((char *)zmbuf , 32, 1, m_hZM_HZK16); //16*16�����Ӧ32���ֽ�

	return 1;
}


/*****************TEST******************/
void testfont()
{
	if (OpenZMFile ())
	 {
		 unsigned char zh[] = "ʮ";
		 GetZM_HZK16(zh,hzkzm);
		 GetZM_ASC16('M', asczm);
	 }

	CloseZMFile ();
}
	
