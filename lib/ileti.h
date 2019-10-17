#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <winsock2.h>

/*
	STRUCTLAR
	
	TARIH (yil, ay, gun, saat, dakika, saniye)	
	ILETI (kaynak[11], hedef[11], tarih, mesaj[101])
	KISI (telefon[11], isim[25], yeniMesaj, toplamMesaj, *ileti)
	ALICI (telefon[11], isim[25], ...)
*/

/*
	FONKSİYONLAR
	
	delay (ms olarak gecikme sağlar)
	TarihiBul TARIH ()
	TarihiKarsilastir char (t1, t2)
*/

void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

typedef struct
{
     int yil;
     int ay;
     int gun;
     int saat;
     int dakika;
     int saniye;
} TARIH;

TARIH TarihBul()
{
     time_t t = time(NULL);
     struct tm tm = *localtime(&t);
     TARIH simdi;

     simdi.yil= tm.tm_year + 1900;
     simdi.ay= tm.tm_mon + 1;
     simdi.gun= tm.tm_mday;
     simdi.saat= tm.tm_hour;
     simdi.dakika= tm.tm_min;
     simdi.saniye= tm.tm_sec;

     return simdi;
}
char TarihKarsilastir(TARIH t1, TARIH t2)
{
     if (t1.yil > t2.yil)       return '>';
     if (t1.yil < t2.yil)       return '<';
     if (t1.ay > t2.ay)         return '>';
     if (t1.ay < t2.ay)         return '<';
     if (t1.gun > t2.gun)       return '>';
     if (t1.gun < t2.gun)       return '<';
     if (t1.saat > t2.saat)     return '>';
     if (t1.saat < t2.saat)     return '<';
     if (t1.dakika > t2.dakika) return '>';
     if (t1.dakika < t2.dakika) return '<';
     if (t1.saniye > t2.saniye) return '>';
     if (t1.saniye < t2.saniye) return '<';
     return '=';
}

typedef struct
{
	char kaynak[11];
	char isim[25];
	char hedef[11];
	TARIH tarih;
	char mesaj[101];
} S_ILETI;

typedef struct
{
	char kaynak;
	TARIH tarih;
	char mesaj[101];
} ILETI;

typedef struct
{
	char telefon[11];
	char isim[25];
	int yeniMesaj;
	int toplamMesaj;
	ILETI ileti[100];
} KISI;

typedef struct
{
	char telefon[11];
	char isim[25];
	char kullaniciAdi[25];
	char sifre[25];
} KULLANICI;

typedef struct
{
	SOCKET so;
	KULLANICI *ku;
} KSOKET;
