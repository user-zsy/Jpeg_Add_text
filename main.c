#include <stdio.h>
#include "bmp_utils.h"
#include "jpeg_utils.h"

//����һ���ı�
static void imgovltBMP(
		unsigned char *pPix, 	// bmp imagedata address
		int stride, 			// image width
		int x, int y, 			// coordinate to place the text (upper-left corner of text)
		const char *text 	// text string (mbcs) Chinese character takes two bytes, ASCII takes one byte
		)
{
	int len, i, width;
	int cd_x, cd_y;
	unsigned char fontmap[32];  
	unsigned char* copy_pPix;
	int bit,nMap;
    printf("========>read \n");
    OpenZMFile ();
	len = strlen(text);
	for( i=0; i<len; )
	{
		if ( text[i] & 0x80 )		// Chinese character
		{
			GetZM_HZK16(&(text[i]),fontmap);
			i += 2;
			width = 16;//����Ϊ16*16
		}
		else if ( text[i] >= 0x20 )
		{
			GetZM_ASC16(text[i], fontmap);
			i ++;
			width = 8;//�ַ�8*16
		}
		else
		{
			i++;		// ignore non-printable character
			width = 0;
		}
		if( width == 0 ) continue;
        
        printf("========>read2 \n");
		nMap = 0;
		for( cd_y = y; cd_y < y+16; cd_y++ )
		{
			copy_pPix = (unsigned char*)pPix;
			copy_pPix = copy_pPix + cd_y*stride*3 + x*3;
			bit = 0x80;
			for(cd_x = x; cd_x < x+width-1; cd_x ++)
			{
				if( bit & fontmap[nMap] )
				{
					*copy_pPix = 151; //b
					copy_pPix ++;
					*copy_pPix = 0; //g  
					copy_pPix ++;
					*copy_pPix = 15; //r
					copy_pPix ++;
				}
				else
					copy_pPix += 3;
				if(  bit == 0x01 )
				{
					bit = 0x80;	
					if( width == 16 )
						nMap ++;
				
				}
				else
					bit >>= 1;
			}
			nMap ++; 

		}
printf("========>read3 \n");
		x += width; //���������ƶ�һ����/�ַ�λ�á�����������󣬿ɼӴ�widthֵ

	}
CloseZMFile ();
}
int bmp_to_jpg(const char* bmp_file, const char* jpg_file)
{
    unsigned char* buffer = NULL;
    int size;
    int width,height;

    read_bmp_file(bmp_file, &buffer, &size, &width, &height);
    swap_rgb(buffer, size);
    printf("size: %d, width: %d height: %d\n", size, width, height);
    write_jpg_file(jpg_file, buffer, width, height, 50);

    free(buffer);

    return 0;
}

int jpg_to_bmp(const char* jpg_file, const char* bmp_file)
{
    unsigned char* buffer = NULL;
    int size;
    int width, height;

    FILE* fp1;
    unsigned char* jpg_buffer = NULL;
    int jpg_size = 0;

#if 1

    fp1 = fopen(jpg_file, "rb");
    if (fp1 == NULL)
    {
        printf("error open file: %s.\n", jpg_file);
        return -1;
    }

    fseek(fp1, 0, SEEK_END);	// �ļ�β
    jpg_size = ftell(fp1);

    fseek(fp1, 0, SEEK_SET);	// ָ���ļ�ͷ

    jpg_buffer = (unsigned char *)malloc(sizeof(char) * jpg_size);
    if (jpg_buffer == NULL)
    {
        printf("malloc error\n");
        return -1;
    }
    memset(jpg_buffer, 0, jpg_size);

    fread(jpg_buffer, 1, jpg_size, fp1);

    printf("==== jpg size: %d\n", jpg_size);

    // ���ڴ�
    read_jpg_buffer(jpg_buffer, jpg_size, &buffer, &size, &width, &height);
#endif

    // ���ļ�
#if 0
    read_jpg_file(jpg_file, &buffer, &size, &width, &height);
#endif

    swap_rgb(buffer, size);
    imgovltBMP(buffer,width,0,height-50,"abbbbbbbb�й�");
    write_bmp_file(bmp_file, buffer, width, height);

    free(buffer);

    if (jpg_buffer != NULL)
    {
        free(jpg_buffer);
    }

    return 0;
}



#if 0
static void OSDTOBMP( BMPImage *snapBmp )
{
	// 4.2 create scaled BMP image file
	//�ڴ˽����ַ�����PHD
	int i,j, fd;
	struct stat st;
	if(1)
	{//����ȫ�����
		unsigned char* pPix;

		for( i=snapBmp->height-51; i<snapBmp->height; i++ )
		{
			pPix = (unsigned char*) snapBmp->imageData;
			pPix = pPix + i * snapBmp->width*3;
			for(j=0; j<snapBmp->width; j++)
			{
				*pPix = 255; //b
				pPix ++;
				*pPix = 255; //g  
				pPix ++;
				*pPix = 255; //r
				pPix ++;
			}
		}

		//�ַ�����......
		imgovltBMP( (unsigned char*) snapBmp->imageData,snapBmp->width, 0, snapBmp->height-50,"a");
		imgovltBMP( (unsigned char*) snapBmp->imageData,snapBmp->width, 0, snapBmp->height-34,"b");
		imgovltBMP( (unsigned char*) snapBmp->imageData,snapBmp->width, 0, snapBmp->height-18,"c");
	//	imgovltBMP( (unsigned char*) snapBmp->imageData,snapBmp->width, 0, snapBmp->height-51,"���վ: 000100   ����: 00101");
	//	imgovltBMP( (unsigned char*) snapBmp->imageData,snapBmp->width, 0, snapBmp->height-34,"ְԱ��: 02010101 ����: ��A12345");
	//	imgovltBMP( (unsigned char*) snapBmp->imageData,snapBmp->width, 0, snapBmp->height-17,"����:1  ����:1  �Ѷ�:10000");
	}

	//����END
	BMP_WriteToFile(snapBmp, "_SCALED_SNAP_BMP_.bmp" );
	BMP_destroy( &snapBmp );
	snapBmp = NULL;

	// step 5 - transcode to JPEG 
#if 0
	if ( bmp2jpeg_transcode("_SCALED_SNAP_BMP_.bmp", "_SCALED_SNAP_JPEG_.jpg", 75)==-1 )
	{
		TRACE_LOG(_T("���ź��ץ��ͼ����ѹ��ΪJPEGͼʧ��!\n"));
		return;
	}
#endif

	// step 6 - load scaled jpeg and replace original snapped jpeg image
	//fd = _topen( _T("_SCALED_SNAP_JPEG_.jpg"), O_RDONLY|O_BINARY );
	fstat(fd, &st);
	//theObj.szBigImage = st.st_size;
	//theObj.pBigImage = realloc( theObj.pBigImage,  theObj.szBigImage );
	//read( fd, theObj.pBigImage, theObj.szBigImage);
	close(fd);

	return;
}
#endif
int main(int argc ,char* argv[])
{
 #if 1
    char src_pic[32] = {0};
    char jpg_pic[32] = {0};
    char bmp_pic[32] = {0};
    int width = 0;

    width = 512;

    sprintf(src_pic, "dog1.bmp");
    sprintf(jpg_pic, "dog.jpg");
    sprintf(bmp_pic, "test1%d.bmp", width);

    //  bmp_to_jpg("dog1.bmp", "dog2.jpg");
    jpg_to_bmp("dog.jpg", "dog_a.bmp");
#endif
    // ����BMPͼƬ
    //analyse_bmp_file("dog1.bmp");
    return 0;
}