#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include "lib/kutu.h"		// TÝPLER () - FONKSÝYONLAR (KonsolBoyutu, gotoxy, KutuCiz, GolgeCiz) - MAKROLAR ()
#include "lib/ileti.h"		// TÝPLER (ILETI, KISI, ALICI) - FONKSÝYONLAR () - MAKROLAR ()

#define HostName "localhost"
#define gecikme 10

int satirOku (short uzunluk, char *string);
int TusOku(char *ch);


int main () 
{	// Türkçe karakterde okuma ve yazma iþlemini yapabilmek için gereken kod.
    setlocale(LC_ALL,"Turkish");
	
    int i , j , sutun, satir, addrlen, tus, rehberS, sil, gks, uz, aktivite, ymsj;
    int isimHata, telefonHata, idHata, sifreHata, idKullanimda, telefonKullanimda, hata;
    char ch , ip[100] , arabellek[1024] , y_tel[11] , y_ad[25] , mesaj[101] , str[5];
	KULLANICI ben;
    
	HANDLE hKonsol = GetStdHandle(STD_OUTPUT_HANDLE);
	WSADATA wsa;
	fd_set readfds;
    SOCKET sunucu = NULL;
    struct hostent *he;
    struct sockaddr_in sunucu_bilgisi, address;
	TARIH simdi , trh;
	addrlen = sizeof(struct sockaddr_in);
	KISI rehber[100], y_kisi , *k , *gkutu[100];
	S_ILETI y_ileti;
	ILETI gelen , msj;
	
	static struct timeval tm = { 0 };
	tm.tv_usec = 25;
	
	sil=0;
	rehberS=0;
    SetConsoleTitle("c-Posta v1.0 (Hazirlayan Bilal Guvenc)");
	KonsolBoyutu(120,50);
	SetConsoleTextAttribute(hKonsol, 112);
	system("cls");
	
	// ############################################################ Sunucuya Baðlan
	
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		SetConsoleTextAttribute(hKonsol, 133);
		GolgeCiz(29,20,60,7);
		SetConsoleTextAttribute(hKonsol, 79);
		KutuCiz(29,20,60,7);
		gotoxy(50,22);
		printf("Hata! (Hata %d)%c", WSAGetLastError() , 7 );
		gotoxy(55,24);
		printf ("Kapat");
		do {} while ( getch()!=13 );
		return 1;
	}
    if ((sunucu = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
		SetConsoleTextAttribute(hKonsol, 133);
		GolgeCiz(29,20,60,7);
		SetConsoleTextAttribute(hKonsol, 79);
		KutuCiz(29,20,60,7);
		gotoxy(43,22);
        printf("Soket olusturulamadi. (Hata %d)%c", WSAGetLastError() , 7 );
		gotoxy(55,24);
		printf ("Kapat");
        do {} while ( getch()!=13 );
        return 1;
    }
    if ((he = gethostbyname(HostName)) == NULL)
    {
		SetConsoleTextAttribute(hKonsol, 133);
		GolgeCiz(29,20,60,7);
		SetConsoleTextAttribute(hKonsol, 79);
		KutuCiz(29,20,60,7);
		gotoxy(45,22);
        printf("Hata! (Get Host by Name %d)%c", WSAGetLastError() , 7 );
		gotoxy(55,24);
		printf ("Kapat");
        do {} while ( getch()!=13 );
        return 1;
    }	
	// Müsait IP adresini bul ve sakla.
    struct in_addr ** addr_list = (struct in_addr **) he->h_addr_list;
    for (i = 0; addr_list[i] != NULL; i++) strcpy(ip, inet_ntoa(*addr_list[i]));
	
    sunucu_bilgisi.sin_family = AF_INET;
    sunucu_bilgisi.sin_addr = *addr_list[0];
    sunucu_bilgisi.sin_port = htons(8888);

    while (connect(sunucu , (struct sockaddr *)&sunucu_bilgisi, sizeof(sunucu_bilgisi)) < 0)
    {
		SetConsoleTextAttribute(hKonsol, 133);
		GolgeCiz(29,17,60,11);
		SetConsoleTextAttribute(hKonsol, 79);
		KutuCiz(29,17,60,11);
		gotoxy(40,20);
        printf("Sunucuya baðlanýlamadý. (Hata %d)%c", WSAGetLastError() , 7 );
		gotoxy(52,24);
		printf ("Tekrar Dene");
        do {} while ( getch()!=13 );
		SetConsoleTextAttribute(hKonsol, 112); 
		system("cls");
    }	
	listen(sunucu, 3);
	arabellek[1023] = NULL;
	int boyut = recv( sunucu , arabellek , 1023  ,0);
	arabellek[boyut]=NULL;
	if ( strcmp( arabellek , "cPosta" )!=0 )
	{
		SetConsoleTextAttribute(hKonsol, 133);
		GolgeCiz(29,20,60,7);
		SetConsoleTextAttribute(hKonsol, 79);
		KutuCiz(29,20,60,7);
		gotoxy(45,22);
        printf("Sunucu doðrulama kodu hatalý. (%s)", arabellek , 7 );
		gotoxy(55,24);
		printf ("Kapat");
        do {} while ( getch()!=13 );
	}
	arabellek[0] = NULL;
	
    // ############################################################ Giriþ / Kayýt
	
	for (i=0;i<25;i++) { ben.isim[i]=NULL; ben.kullaniciAdi[i]=NULL; ben.sifre[i]=NULL; }
	for (i=0;i<11;i++) ben.telefon[i]=NULL; sutun=1; hata=1;
    do {
		SetConsoleTextAttribute(hKonsol, 112); 
		system("cls");
		if (sutun==1)
        {	// Giriþ / Kayýt Menüsü Ekrana Yazdýrýlýr
			SetConsoleTextAttribute(hKonsol, 133);
			GolgeCiz(44,15,30,14);
			SetConsoleTextAttribute(hKonsol, 59);
			KutuCiz(44,15,30,14);
			gotoxy(48,21); printf ("Kullanýcý Adý");
			gotoxy(48,24); printf ("Þifre");
			
			SetConsoleTextAttribute(hKonsol, 63);
			KutuCiz(44,15,15,3);
			gotoxy(49,16); printf("Giriþ");
			SetConsoleTextAttribute(hKonsol, 159);
			KutuCiz(59,15,15,3);
			gotoxy(64,16); printf("Kayýt");
			
			SetConsoleTextAttribute(hKonsol, 63);   
			gotoxy(48,22); printf ("%s",ben.kullaniciAdi);
			gotoxy(48,25); printf ("%s",ben.sifre);
			
			satir=1;
			do {
				i=0;
				
				if (satir==1) 
				{
					gotoxy(48 + strlen(ben.kullaniciAdi),22);
					i=satirOku(25, ben.kullaniciAdi);
				} else if (satir==2) 
				{
					gotoxy(48 + strlen(ben.sifre),25);
					i=satirOku(25, ben.sifre);
				}
				if ( (i==11) && (satir<2) )               // ENTER TUÞU
					{satir++; i=0;}
				else if ( (i==1) && (satir>1) ) satir--;  // ÜST TUÞU
				else if ( (i==2) && (satir<2) ) satir++;  // ALT TUÞU
				else if (i==4) sutun=2;                   // SAÐ TUÞU 
			} while ( (sutun==1) && (satir!=2 || i!=11) );
		} else
		{
			SetConsoleTextAttribute(hKonsol, 133);
			GolgeCiz(44,15,30,20);
			SetConsoleTextAttribute(hKonsol, 59);
			KutuCiz(44,15,30,20);
			gotoxy(48,21); printf ("Ad Soyad");
			gotoxy(48,24); printf ("Telefon Numarasý");
			gotoxy(48,27); printf ("Kullanýcý Adý");
			gotoxy(48,30); printf ("Þifre");
			
			SetConsoleTextAttribute(hKonsol, 159);
			KutuCiz(44,15,15,3);
			gotoxy(49,16); printf("Giriþ");
			SetConsoleTextAttribute(hKonsol, 63);
			KutuCiz(59,15,15,3);
			gotoxy(64,16); printf("Kayýt");
			
			SetConsoleTextAttribute(hKonsol, 63);   
			gotoxy(48,22); printf ("%s", ben.isim);
			gotoxy(48,25); printf ("%s", ben.telefon);
			gotoxy(48,28); printf ("%s", ben.kullaniciAdi);
			gotoxy(48,31); printf ("%s", ben.sifre);
			satir=1;
			do 
			{
				i=0;				
				if (satir==1) 
				{
					gotoxy(48 + strlen(ben.isim),22);
					i=satirOku(25, ben.isim);
				} else if (satir==2) 
				{
					gotoxy(48 + strlen(ben.telefon),25);
					i=satirOku(11, ben.telefon);
				} else if (satir==3) 
				{
					gotoxy(48 + strlen(ben.kullaniciAdi),28);
					i=satirOku(25, ben.kullaniciAdi);
				} else if (satir==4) 
				{
					gotoxy(48 + strlen(ben.sifre),31);
					i=satirOku(25, ben.sifre);
				}
				if ( (i==11) && (satir<4) )               // ENTER TUÞU
					{satir++; i=0;}
				else if ( (i==1) && (satir>1) ) satir--;  // ÜST TUÞU
				else if ( (i==2) && (satir<4) ) satir++;  // ALT TUÞU
				else if (i==3) sutun=1;                   // SOL TUÞU 
			} while ( (sutun==2) && (satir!=4 || i!=11) );
		}
		if (i==11)
		{	// BU KISIMDA sunucuya baglanip kontrol yapýlýr
			satir=1;
			if (sutun==1)
			{	// Giriþ yapýlýyor. Bilgiler kontrol edilecek.
				if ( (strlen(ben.kullaniciAdi)>0 ) && ( strlen(ben.sifre)>0 )  )
				{
					send( sunucu , "G" , 1 , 0 );
					recv( sunucu , str , 5  ,0 );					// ONAY al
					send( sunucu , ben.kullaniciAdi , strlen(ben.kullaniciAdi) , 0 );
					recv( sunucu , str , 5  ,0 );					// ONAY al
					send( sunucu , ben.sifre , strlen(ben.sifre) , 0 );
					arabellek[ recv( sunucu , arabellek , 1023  ,0 )]=NULL;
					if ( arabellek[0] == 'D' )
					{	// Kullanýcý Adý ve Þifre eþleþti. Kullanýcý bilgilerini al.
						send( sunucu , "X" , 1 , 0 );				// ONAY yolla
						ben.isim[ recv( sunucu , ben.isim , 25  ,0) ]=NULL;
						send( sunucu , "X" , 1 , 0 );				// ONAY yolla
						ben.telefon[ recv( sunucu , ben.telefon , 11  ,0) ]=NULL;
						hata=0;
					} else if ( arabellek[0] == 'I' )
					{	// Kullanýcý adý kayýtlý deðil. Kullanýcýya bilgi ver.
						SetConsoleTextAttribute(hKonsol, 133);
						GolgeCiz(31,17,56,9);
						SetConsoleTextAttribute(hKonsol, 79);
						KutuCiz(31,17,56,9);
						gotoxy(42,20);
						printf("Girdiðiniz kullanýcý adý hatalý. ");
						gotoxy(56,23);
						printf("Geri%c",7);
						do {} while (getch()!=13);
						hata=1;
					} else 
					{	// Kullanýcý adý veya þifre hatalý. Kullanýcýya bilgi ver.
						SetConsoleTextAttribute(hKonsol, 133);
						GolgeCiz(31,17,56,9);
						SetConsoleTextAttribute(hKonsol, 79);
						KutuCiz(31,17,56,9);
						gotoxy(37,20);
						printf("Girdiðiniz kullanýcý adý veya þifre hatalý. ");
						gotoxy(56,23);
						printf("Geri%c",7);
						do {} while (getch()!=13);
						hata=1;
					}
				}
			} else
			{	// Kayýt yapýlýyor. Bilgiler kontrol edilicek.
				isimHata=0; idHata=0; sifreHata=0; telefonHata=0; telefonKullanimda=0; idKullanimda=0;
				
				if ( strlen(ben.isim )<4) isimHata=1;
				if ( strlen(ben.kullaniciAdi)<4 ) idHata=1;
				if ( strlen(ben.sifre)<4 ) sifreHata=1;
				for (i=0;i<10;i++) if ( (ben.telefon[i]<'0') || (ben.telefon[i]>'9') ) telefonHata=1;
				hata = isimHata + idHata + sifreHata + telefonHata ;
				
				if ( hata == 0 ) 
				{	// Sunucuya bilgiler gönderilir ve cevap alýnýr.
					send( sunucu , "K" , 1 , 0 );										// "K"ayýt Kodu Gönder
					recv( sunucu , str , 5  ,0 );										// ONAY al
					send( sunucu , ben.isim , strlen(ben.isim) , 0 );					// Ýsim Gönder
					recv( sunucu , str , 5  ,0 );										// ONAY al
					send( sunucu , ben.telefon , strlen(ben.telefon) , 0 );				// Telefon Gönder
					recv( sunucu , str , 5  ,0 );										// ONAY al
					send( sunucu , ben.kullaniciAdi , strlen(ben.kullaniciAdi) , 0 );	// ID Gönder
					recv( sunucu , str , 5  ,0 );										// ONAY al
					send( sunucu , ben.sifre , strlen(ben.sifre) , 0 );					// Þifre Gönder
					arabellek[ recv( sunucu , arabellek , 1023  ,0 )]=NULL; // D: Doðru, T: Telefon dolu, A: ID dolu, I: Ýkisi dolu
					if ( arabellek[0] != 'D' )
					{	// Kullanýcý adý veya telefon numarasý zaten kullanýmda. Hangisi onu bul.
						if ( arabellek[0] == 'T' ) telefonKullanimda=1;					// Telefon kullanýmda.
						else if ( arabellek[0] == 'I' ) idKullanimda=1;					// ID kullanýmda.
						else { telefonKullanimda=1; idKullanimda=1; }					// Ýkisi de kullanýmda.
						hata = telefonKullanimda + idKullanimda ;
					}
				}
				if ( hata != 0 )
				{	// Hata varsa, kullanýcýya hata ile ilgili bilgi verilir.
					SetConsoleTextAttribute(hKonsol, 133);
					GolgeCiz(30,20,54,8+hata);
					SetConsoleTextAttribute(hKonsol, 79);
					KutuCiz(30,20,54,8+hata);
					for (i=0;i<hata;i++) 
					{
						gotoxy(36,23+i);
						if (telefonKullanimda==1) { printf("Girdiðiniz telefon numarasý kullanýlmaktadýr."); telefonKullanimda=0; }
						else if (idKullanimda==1) { printf("Girdiðiniz kullanýcý adý kullanýlmaktadýr."); idKullanimda=0; }
						else if (telefonHata==1) { printf("Girdiðiniz telefon numarasý hatalý."); telefonHata=0; }
						else if (idHata==1) { printf("Girdiðiniz kullanýcý adý çok kýsa."); idHata=0; }
						else if (sifreHata==1) { printf("Girdiðiniz þifre çok kýsa."); sifreHata=0; }
						else if (isimHata==1) { printf ("Girdiðiniz isim çok kýsa."); isimHata=0; }
					}
					gotoxy(54,25+i);
					printf("Geri%c",7);
					do {} while (getch()!=13);
				}
			}
		}
	} while ( hata != 0 );
	
	// ############################################################ Programa Giriþ
	while ( 1 )
	{
		satir=0; sutun=0;
		do {	// Menü Üst Sekmeleri Ekrana Yazdýrýlýr
			SetConsoleTextAttribute(hKonsol, 112); 
			system("cls");
			SetConsoleTextAttribute(hKonsol, 31);
			KutuCiz(0,0,120,3);
			gotoxy( 0 , 1 );
			printf ("          Gelen Kutusu               Rehber               Kiþi Ekle               Kiþi Sil               Çýkýþ          ");
			SetConsoleTextAttribute(hKonsol, 63);
			KutuCiz(0,3,120,1);			
			if ( sutun == 0 )
			{	// Gelen Kutusu Seçili Ýse ###########################################################################
				KutuCiz( 2 , 0 , 28 , 3 );
				gotoxy( 10 , 1 ); printf("Gelen Kutusu");
				// Tüm Kiþilerden En Az 1 Kere Mesajlaþýlmýþ Olanýný Bul
				gks=0;
				for (i=0;i<rehberS;i++)
				{
					if ( rehber[i].toplamMesaj > 0 )
					{
						gkutu[gks] = &rehber[i];
						gks++;
					}
				}
				if (gks==0)
				{	// Kimseyle Mesajlaþýlmadýysa bunu kullanýcýya bildir
					SetConsoleTextAttribute(hKonsol, 133);
					GolgeCiz(29,17,60,7);
					SetConsoleTextAttribute(hKonsol, 47);
					KutuCiz(29,17,60,7);
					gotoxy(42,20);
					printf("Gelen kutunuzda hiç mesajýnýz yok. %c",2);
				} else
				{
					for (i=0;i<(gks-1);i++)
					{	// Mesajlaþmalarý En YENÝden En ESKÝye Doðru Sýrala
						for (j=i;j<(gks-1);j++)
						{
							if ( TarihKarsilastir( gkutu[j]->ileti[ (gkutu[j]->toplamMesaj)-1 ].tarih , gkutu[j+1]->ileti[ (gkutu[j+1]->toplamMesaj)-1 ].tarih ) == '<' )
							{
								k = gkutu[j];
								gkutu[j] = gkutu[j+1];
								gkutu[j+1] = k;
							}
						}
					}
					if ( gks>9 ) j=9;	// Ekranda Gösterebileceðimizden Fazla Mesajlaþma Varsa Ýlk 9unu Göster
					else j=gks;
					for (i=0;i<j;i++)
					{	// Gelen Kutusunu Ekrana Yazdýr
						if (i==satir) 
						{
							SetConsoleTextAttribute(hKonsol, 143);
							GolgeCiz( 31 , 5+(5*i) , 60 , 3 );
							SetConsoleTextAttribute(hKonsol, 47);
							KutuCiz( 31 , 5+(5*i) , 55 , 3 );
							gotoxy( 34 , 6+(5*i) );
							printf( "%s" , gkutu[i]->isim );
							trh = gkutu[i]->ileti[(gkutu[i]->toplamMesaj)-1].tarih;
							gotoxy( 61 , 6+(5*i) );
							printf( "(%s)     %2.2d.%2.2d" , gkutu[i]->telefon , trh.saat , trh.dakika );
							SetConsoleTextAttribute(hKonsol, 175);
							KutuCiz( 86 , 5+(5*i) , 5 , 3 );
							gotoxy( 88 , 6+(5*i) );
							printf( "%d" , gkutu[i]->yeniMesaj );
						}
						else
						{
							SetConsoleTextAttribute(hKonsol, 143);
							GolgeCiz( 29 , 5+(5*i) , 60 , 3 );
							SetConsoleTextAttribute(hKonsol, 31);
							KutuCiz( 29 , 5+(5*i) , 55 , 3 );
							gotoxy( 32 , 6+(5*i) );
							printf( "%s" , gkutu[i]->isim );
							trh = gkutu[i]->ileti[(gkutu[i]->toplamMesaj)-1].tarih;
							gotoxy( 59 , 6+(5*i) );
							printf( "(%s)     %2.2d.%2.2d" , gkutu[i]->telefon , trh.saat , trh.dakika );
							SetConsoleTextAttribute(hKonsol, 159);
							KutuCiz( 84 , 5+(5*i) , 5 , 3 );
							gotoxy( 86 , 6+(5*i) );
							printf( "%d" , gkutu[i]->yeniMesaj );
						}
					}
				}
				
				tus=0;
				do { // Tuþ oku veya mesaj al.
					if( _kbhit() )
					{
						ch = _getch();
						if(ch == -32)
						{	// YÖN TUÞLARI
							ch=_getch();
							if ((ch==72)&&(satir>0)&&(gks!=0)) { tus=2; satir--; }	// ÜST OK
							if ((ch==80)&&(satir<(gks-1))) { tus=2; satir++; }		// ALT OK
							if (ch==77) { tus=2; sutun++; satir=0;}					// SAÐ OK
						} else
						{
							if (ch==27) { sutun=4; tus=1; }							// ESC TUÞU
							else if ((ch==13)&&(gks>0))								// ENTER TUÞU
							{
								k = &(*gkutu[satir]);
								tus=99;	// SOHBET EKRANINI AÇ
							}
						}
					}
					
					FD_ZERO( &readfds );
					FD_SET( sunucu , &readfds );
					aktivite = select( 0 , &readfds , NULL , NULL , &tm);
					if ( FD_ISSET( sunucu , &readfds) )
					{	// Sunucudan gelen birþeyler var ona bak.
						recv( sunucu , arabellek , 100 , 0);
						send( sunucu , "X" , 1 , 0 );			// ONAY yolla
						// Mesajý al ve gereken iþlemleri yap.
						recv( sunucu , (char*)&(y_ileti) , sizeof(S_ILETI) , 0);
						gelen.kaynak = 'O';
						gelen.tarih = y_ileti.tarih;
						strcpy( gelen.mesaj , y_ileti.mesaj );
						for ( i=0 ; i<rehberS ; i++ )
						{	// Tüm rehberde mesajýn kaynaðýný ara.
							if ( strcmp( y_ileti.kaynak , rehber[i].telefon ) == 0 )
							{	// Rehberde numara varsa mesajý ona ekle.
								rehber[i].ileti[rehber[i].toplamMesaj] = gelen;
								rehber[i].toplamMesaj++;
								rehber[i].yeniMesaj++;
								break;
							}
						}
						if ( i == rehberS )
						{	// Kiþi ekli deðilse yeni kiþi oluþturulur ve mesaj ona eklenir.
							strcpy( rehber[rehberS].telefon , y_ileti.kaynak );
							strcpy( rehber[rehberS].isim , y_ileti.isim );
							rehber[rehberS].yeniMesaj = 1;
							rehber[rehberS].toplamMesaj = 1;
							rehber[rehberS].ileti[0] = gelen;
							rehberS++;
						}
						tus=999;	// Yeni mesaj geldi ekran yenilensin.
					}
					
				} while (tus==0);
				
			} else if ( sutun == 1 )
			{	// Rehber Seçili Ýse #################################################################################
				KutuCiz( 29 , 0 , 22 , 3 );
				gotoxy( 37 , 1 ); printf("Rehber");
				
				if ( rehberS == 0 )
				{	// Rehberi Boþ Ýse Kullanýcýya Bunu Bildir
					SetConsoleTextAttribute(hKonsol, 133);
					GolgeCiz(29,17,60,7);
					SetConsoleTextAttribute(hKonsol, 47);
					KutuCiz(29,17,60,7);
					gotoxy(42,20);
					printf("Maalesef hiçbir arkadaþýnýz yok. %c",2);
				} else
				{
					// Rehberini Ekrana Yazdýr
					if ( rehberS>42 ) j=42;
					else j=rehberS;
					SetConsoleTextAttribute(hKonsol, 143);
					GolgeCiz( 35 , 5 , 48 , 2+j );
					SetConsoleTextAttribute(hKonsol, 31);
					gotoxy( 35 , 5 );
					printf("    Kiþi Ýsmi                    Telefon No     ");
					gotoxy( 68 , 5 );
					printf("Telefon No");
					SetConsoleTextAttribute(hKonsol, 63);
					KutuCiz( 35 , 6 , 48 , 1+j );
					for (i=0;i<j;i++)
					{	
						gotoxy( 39 , 6+i );
						printf( "%s" , rehber[i].isim );
						gotoxy( 67 , 6+i );
						printf( "(%s)" , rehber[i].telefon );
					}
					SetConsoleTextAttribute(hKonsol, 160);
					KutuCiz( 35 , 6+satir , 48 , 1 );
					gotoxy( 39 , 6+satir );
					printf( "%s" , rehber[satir].isim );
					gotoxy( 67 , 6+satir );
					printf( "(%s)" , rehber[satir].telefon );
				}
				
				tus=0;
				do { // Tuþ oku veya mesaj al.
					if( _kbhit() )
					{
						ch = _getch();
						if(ch == -32)
						{	// YÖN TUÞLARI
							ch=_getch();
							if (ch==72) {if ( satir>0 ) satir--; tus=2;}		// ÜST OK
							if (ch==80) {if ((satir+1)<rehberS) satir++; tus=2;}// ALT OK
							if (ch==75) { tus=2; sutun--; satir=0; }			// SOL OK
							if (ch==77) { tus=2; sutun++; }						// SAÐ OK
						} else
						{
							if (ch==27) { sutun=4; tus=1; }						// ESC TUÞU
							else if ((ch==13)&&(rehberS>0))						// ENTER TUÞU
							{
								tus=99;
								k = &rehber[satir];
							}
						}
					}
					
					FD_ZERO( &readfds );
					FD_SET( sunucu , &readfds );
					aktivite = select( 0 , &readfds , NULL , NULL , &tm);
					if ( FD_ISSET( sunucu , &readfds) )
					{	// Sunucudan gelen birþeyler var ona bak.
						recv( sunucu , arabellek , 100 , 0);
						send( sunucu , "X" , 1 , 0 );			// ONAY yolla
						// Mesajý al ve gereken iþlemleri yap.
						recv( sunucu , (char*)&(y_ileti) , sizeof(S_ILETI) , 0);
						gelen.kaynak = 'O';
						gelen.tarih = y_ileti.tarih;
						strcpy( gelen.mesaj , y_ileti.mesaj );
						for ( i=0 ; i<rehberS ; i++ )
						{	// Tüm rehberde mesajýn kaynaðýný ara.
							if ( strcmp( y_ileti.kaynak , rehber[i].telefon ) == 0 )
							{	// Rehberde numara varsa mesajý ona ekle.
								rehber[i].ileti[rehber[i].toplamMesaj] = gelen;
								rehber[i].toplamMesaj++;
								rehber[i].yeniMesaj++;
								break;
							}
						}
						if ( i == rehberS )
						{	// Kiþi ekli deðilse yeni kiþi oluþturulur ve mesaj ona eklenir.
							strcpy( rehber[rehberS].telefon , y_ileti.kaynak );
							strcpy( rehber[rehberS].isim , y_ileti.isim );
							rehber[rehberS].yeniMesaj = 1;
							rehber[rehberS].toplamMesaj = 1;
							rehber[rehberS].ileti[0] = gelen;
							rehberS++;
						}
						printf( "%c" , 7 );
					}
					
				} while (tus==0);
				
			} else if ( sutun == 2 )
			{	// Kiþi Ekle Seçili Ýse ##############################################################################
				KutuCiz( 50 , 0 , 25 , 3 );
				gotoxy( 58 , 1 ); printf("Kiþi Ekle");
				SetConsoleTextAttribute(hKonsol, 111);
				gotoxy( 0 , 0 );
				SetConsoleTextAttribute(hKonsol, 133);
				GolgeCiz(29,17,60,8);
				SetConsoleTextAttribute(hKonsol, 47);
				KutuCiz(29,17,60,8);
				gotoxy(42,20);
				printf("Telefon Numarasý");
				gotoxy(42,21);
				
				tus=0; y_tel[0]=NULL; j=0; y_ad[0]=NULL;
				do { // Tuþ oku veya mesaj al.
					if( _kbhit() )
					{
						ch = _getch();
						if(ch == -32)
						{	// YÖN TUÞLARI
							ch=_getch();
							if (ch==75) { tus=2; sutun--; }   	// SOL OK
							if (ch==77) { tus=2; sutun++; }		// SAÐ OK
						} else
						{
							if (ch==27) { sutun=4; tus=1; }		// ESC TUÞU
							
							else if (j==0)
							{	// TELEFON OKU ##########################################################
								i=strlen(y_tel);
								if ( ch == 13 )					// ENTER TUÞU
								{
									hata=0;
									for (i=0;i<rehberS;i++)
									{
										if ( strcmp( rehber[i].telefon , y_tel ) == 0 )
											hata=1;
									}
									if ( strcmp( ben.telefon , y_tel) == 0 ) hata=2;
									if ( hata == 1 )
									{
										SetConsoleTextAttribute(hKonsol, 79);
										KutuCiz(29,17,60,8);
										gotoxy(46,20);
										printf("Bu Numara Zaten Kayýtlý");
										gotoxy(55,22);
										printf("Tamam");
										j=2;
										y_ad[0]=NULL; y_tel[0]=NULL;
									} else if ( hata == 2 )
									{
										SetConsoleTextAttribute(hKonsol, 79);
										KutuCiz(29,17,60,8);
										gotoxy(43,20);
										printf("Kendi Kendinizi Ekleyemezsiniz");
										gotoxy(55,22);
										printf("Tamam");
										j=2;
										y_ad[0]=NULL; y_tel[0]=NULL;
									} else if (strlen(y_tel)==10)
									{
										send( sunucu , "E" , 1 , 0 );							// "E"kle Kodu Gönder
										recv( sunucu , str , 5  ,0 );							// ONAY al
										send( sunucu , y_tel , 11 , 0 );						// Telefon Gönder"
										arabellek[ recv( sunucu , arabellek , 25  ,0) ]=NULL;
										if ( arabellek[0] == 'V' )
										{
											send( sunucu , "X" , 1 , 0 );						// ONAY yolla
											y_ad[ recv( sunucu , y_ad , 25  ,0) ]=NULL;
											SetConsoleTextAttribute(hKonsol, 47);
											KutuCiz(29,17,60,8);
											gotoxy(42,20);
											printf("Kiþi Adý");
											gotoxy(42,21);
											printf("%s",y_ad);
											j=1;
										} else
										{
											SetConsoleTextAttribute(hKonsol, 79);
											KutuCiz(29,17,60,8);
											gotoxy(45,20);
											printf("Aradýðýnýz Kiþi Bulunamadý");
											gotoxy(55,22);
											printf("Tamam");
											j=2;
										}
									}
								}
								else if (ch == 8)				// SÝL TUÞU
								{
									if ( i>0 )
									{
										y_tel[i-1]=NULL;
										printf("%c %c",8,8);
									}
								}
								else if ( (ch>='0') && (ch<='9') )
								{
									if(i<10)
									{
										y_tel[i]=ch;
										y_tel[i+1]=NULL;
										printf("%c",ch);
									}
								}
							} else if (j==1)
							{	// ÝSÝM OKU ##########################################################
								i=strlen(y_ad);
								if ( ch == 13 )					// ENTER TUÞU @@@@@@@@@@
								{
									if (i>3)
									{
										SetConsoleTextAttribute(hKonsol, 160);
										KutuCiz(29,17,60,8);
										gotoxy(47,20);
										printf("Kiþi Baþarýyla Eklendi");
										gotoxy(55,22);
										printf("Tamam");
										strcpy( rehber[rehberS].isim , y_ad );
										strcpy( rehber[rehberS].telefon , y_tel );
										rehber[rehberS].toplamMesaj=0;
										rehberS++;
										y_ad[0]=NULL; y_tel[0]=NULL;
										j=2;
									}
								}
								else if (ch == 8)				// SÝL TUÞU
								{
									if (i>0)
									{
										y_ad[i-1]=NULL;
										printf("%c %c",8,8);
									}
								}
								else if ( i<24 )
								{
									if (ch==-104) ch='Ý';
									else if (ch==-115) ch='ý';
									else if (ch== -90) ch='Ð';
									else if (ch== -89) ch='ð';
									else if (ch==-102) ch='Ü';
									else if (ch==-127) ch='ü';
									else if (ch== -98) ch='Þ';
									else if (ch== -97) ch='þ';
									else if (ch==-103) ch='Ö';
									else if (ch==-108) ch='ö';
									else if (ch==-128) ch='Ç';
									else if (ch==-121) ch='ç';
									y_ad[i]=ch;
									y_ad[i+1]=NULL;
									printf("%c",ch);
								}
							} else if (j==2)
							{	// UYARI EKRANI ##########################################################
								if (ch==13)
								{
									SetConsoleTextAttribute(hKonsol, 47);
									KutuCiz(29,17,60,8);
									gotoxy(42,20);
									printf("Telefon Numarasý");
									gotoxy(42,21);
									y_ad[0]=NULL; y_tel[0]=NULL;
									j=0;
								}
							}
						}
					}
					
					FD_ZERO( &readfds );
					FD_SET( sunucu , &readfds );
					aktivite = select( 0 , &readfds , NULL , NULL , &tm);
					if ( FD_ISSET( sunucu , &readfds) )
					{	// Sunucudan gelen birþeyler var ona bak.
						recv( sunucu , arabellek , 100 , 0);
						send( sunucu , "X" , 1 , 0 );			// ONAY yolla
						// Mesajý al ve gereken iþlemleri yap.
						recv( sunucu , (char*)&(y_ileti) , sizeof(S_ILETI) , 0);
						gelen.kaynak = 'O';
						gelen.tarih = y_ileti.tarih;
						strcpy( gelen.mesaj , y_ileti.mesaj );
						for ( i=0 ; i<rehberS ; i++ )
						{	// Tüm rehberde mesajýn kaynaðýný ara.
							if ( strcmp( y_ileti.kaynak , rehber[i].telefon ) == 0 )
							{	// Rehberde numara varsa mesajý ona ekle.
								rehber[i].ileti[rehber[i].toplamMesaj] = gelen;
								rehber[i].toplamMesaj++;
								rehber[i].yeniMesaj++;
								break;
							}
						}
						if ( i == rehberS )
						{	// Kiþi ekli deðilse yeni kiþi oluþturulur ve mesaj ona eklenir.
							strcpy( rehber[rehberS].telefon , y_ileti.kaynak );
							strcpy( rehber[rehberS].isim , y_ileti.isim );
							rehber[rehberS].yeniMesaj = 1;
							rehber[rehberS].toplamMesaj = 1;
							rehber[rehberS].ileti[0] = gelen;
							rehberS++;
						}
						printf( "%c" , 7 );
					}
					
				} while (tus==0);
				
			} else if ( sutun == 3 )
			{	// Kiþi Sil Seçili Ýse ###############################################################################
				KutuCiz( 74 , 0 , 24 , 3 );
				gotoxy( 82 , 1 ); printf("Kiþi Sil");
				
				if ( (sil==1) && (rehberS!=0) )
				{	// Kiþi silme iþlemi yapýlýr.
					if ( rehberS>1 ) for (i=satir;i<rehberS;i++)
						rehber[i] = rehber[i+1];
					rehberS--;
					sil=0;
					satir=0;
				} else sil=0;
				
				if ( rehberS == 0 )
				{	// Rehberi Boþ Ýse Kullanýcýya Bunu Bildir
					SetConsoleTextAttribute(hKonsol, 133);
					GolgeCiz(29,17,60,7);
					SetConsoleTextAttribute(hKonsol, 47);
					KutuCiz(29,17,60,7);
					gotoxy(42,20);
					printf("Maalesef hiçbir arkadaþýnýz yok. %c",2);
				} else
				{
					// Rehberini Ekrana Yazdýr
					if ( rehberS>42 ) j=42;
					else j=rehberS;
					SetConsoleTextAttribute(hKonsol, 143);
					GolgeCiz( 35 , 5 , 48 , 2+j );
					SetConsoleTextAttribute(hKonsol, 31);
					gotoxy( 35 , 5 );
					printf("    Kiþi Ýsmi                    Telefon No     ");
					SetConsoleTextAttribute(hKonsol, 63);
					KutuCiz( 35 , 6 , 48 , 1+j );
					for (i=0;i<j;i++)
					{	
						gotoxy( 39 , 6+i );
						printf( "%s" , rehber[i].isim );
						gotoxy( 67 , 6+i );
						printf( "(%s)" , rehber[i].telefon );
					}
					SetConsoleTextAttribute(hKonsol, 192);
					KutuCiz( 35 , 6+satir , 48 , 1 );
					gotoxy( 39 , 6+satir );
					printf( "%s" , rehber[satir].isim );
					gotoxy( 67 , 6+satir );
					printf( "(%s)" , rehber[satir].telefon );
				}
				
				tus=0;
				do { // Tuþ oku veya mesaj al.
					if( _kbhit() )
					{
						ch = _getch();
						if(ch == -32)
						{	// YÖN TUÞLARI
							ch=_getch();
							if (ch==72) {if ( satir>0 ) satir--; tus=2;}		// ÜST OK
							if (ch==80) {if ((satir+1)<rehberS) satir++; tus=2;}// ALT OK
							if (ch==75) { tus=2; sutun--; }						// SOL OK
							if (ch==77) { tus=2; sutun++; }						// SAÐ OK
						} else
						{
							if (ch==27) { sutun=4; tus=1; }						// ESC TUÞU
							else if (ch==13) { tus=1; sil=1; }					// ENTER TUÞU
						}
					}
					
					FD_ZERO( &readfds );
					FD_SET( sunucu , &readfds );
					aktivite = select( 0 , &readfds , NULL , NULL , &tm);
					if ( FD_ISSET( sunucu , &readfds) )
					{	// Sunucudan gelen birþeyler var ona bak.
						recv( sunucu , arabellek , 100 , 0);
						send( sunucu , "X" , 1 , 0 );			// ONAY yolla
						// Mesajý al ve gereken iþlemleri yap.
						recv( sunucu , (char*)&(y_ileti) , sizeof(S_ILETI) , 0);
						gelen.kaynak = 'O';
						gelen.tarih = y_ileti.tarih;
						strcpy( gelen.mesaj , y_ileti.mesaj );
						for ( i=0 ; i<rehberS ; i++ )
						{	// Tüm rehberde mesajýn kaynaðýný ara.
							if ( strcmp( y_ileti.kaynak , rehber[i].telefon ) == 0 )
							{	// Rehberde numara varsa mesajý ona ekle.
								rehber[i].ileti[rehber[i].toplamMesaj] = gelen;
								rehber[i].toplamMesaj++;
								rehber[i].yeniMesaj++;
								break;
							}
						}
						if ( i == rehberS )
						{	// Kiþi ekli deðilse yeni kiþi oluþturulur ve mesaj ona eklenir.
							strcpy( rehber[rehberS].telefon , y_ileti.kaynak );
							strcpy( rehber[rehberS].isim , y_ileti.isim );
							rehber[rehberS].yeniMesaj = 1;
							rehber[rehberS].toplamMesaj = 1;
							rehber[rehberS].ileti[0] = gelen;
							rehberS++;
						}
						printf( "%c" , 7 );
					}
					
				} while (tus==0);
				
			} else if ( sutun == 4 )
			{	// Çýkýþ Seçili Ýse ##################################################################################
				KutuCiz( 97 , 0 , 21 , 3 );
				gotoxy( 105 , 1 ); printf("Çýkýþ");
				SetConsoleTextAttribute(hKonsol, 111);
				gotoxy( 0 , 0 );
				
				SetConsoleTextAttribute(hKonsol, 133);
				GolgeCiz(29,17,60,11);
				SetConsoleTextAttribute(hKonsol, 79);
				KutuCiz(29,17,60,11);
				gotoxy(42,20);
				printf("Programý kapatmak istiyor musunuz?");
				gotoxy(55,24);
				printf ("Evet");
				
				tus=0;
				do { // Tuþ oku veya mesaj al.
					if( _kbhit() )
					{
						ch = _getch();
						if(ch == -32)
						{	// YÖN TUÞLARI
							ch=_getch();
							if (ch==75) { tus=2; sutun--; }   	// SOL OK
						} else
						{
							if (ch==27) return 0;				// ESC TUÞU
							else if (ch==13) return 0;			// ENTER TUÞU
						}
					}
					
					FD_ZERO( &readfds );
					FD_SET( sunucu , &readfds );
					aktivite = select( 0 , &readfds , NULL , NULL , &tm);
					if ( FD_ISSET( sunucu , &readfds) )
					{	// Sunucudan gelen birþeyler var ona bak.
						recv( sunucu , arabellek , 100 , 0);
						send( sunucu , "X" , 1 , 0 );			// ONAY yolla
						// Mesajý al ve gereken iþlemleri yap.
						recv( sunucu , (char*)&(y_ileti) , sizeof(S_ILETI) , 0);
						gelen.kaynak = 'O';
						gelen.tarih = y_ileti.tarih;
						strcpy( gelen.mesaj , y_ileti.mesaj );
						for ( i=0 ; i<rehberS ; i++ )
						{	// Tüm rehberde mesajýn kaynaðýný ara.
							if ( strcmp( y_ileti.kaynak , rehber[i].telefon ) == 0 )
							{	// Rehberde numara varsa mesajý ona ekle.
								rehber[i].ileti[rehber[i].toplamMesaj] = gelen;
								rehber[i].toplamMesaj++;
								rehber[i].yeniMesaj++;
								break;
							}
						}
						if ( i == rehberS )
						{	// Kiþi ekli deðilse yeni kiþi oluþturulur ve mesaj ona eklenir.
							strcpy( rehber[rehberS].telefon , y_ileti.kaynak );
							strcpy( rehber[rehberS].isim , y_ileti.isim );
							rehber[rehberS].yeniMesaj = 1;
							rehber[rehberS].toplamMesaj = 1;
							rehber[rehberS].ileti[0] = gelen;
							rehberS++;
						}
						printf( "%c" , 7 );
					}
					
				} while (tus==0);	
			}
		} while ( tus != 99 ); // ENTER TUÞUNU TEKRAR AYARLA!!!
		
		// ############################################################ Mesajlaþma
		KonsolBoyutu(120,50); SetConsoleTextAttribute(hKonsol, 47); system("cls");
		gotoxy(111,47); printf ("Gönder");			// GÖNDER KISMI
		SetConsoleTextAttribute(hKonsol, 63);		// ÜST KISIM (isim telefon)
		KutuCiz(0,0,120,3);
		gotoxy(2,1); 
		printf("%s (%s)" , k->isim , k->telefon );
		SetConsoleTextAttribute(hKonsol, 31);		// ESC YAZISI
		KutuCiz(107,0,13,3);
		gotoxy(111,1);
		printf("ESC %c", 27);
		SetConsoleTextAttribute(hKonsol, 111);		// Mesaj Yazma Kutusu
		KutuCiz(0,45,108,5);
		gotoxy(2,46);
		mesaj[0] = NULL;
		SetConsoleTextAttribute(hKonsol, 112);		// BOÞ ARKA PLAN
		KutuCiz(0,3,120,42);
		tus=0;
		k->yeniMesaj = 0;							// Mesajlar okundu.
		if ( k->toplamMesaj > 0 )
		{	// Mesajlarý ekrana yazdýr.
			if ( k->toplamMesaj > 8 ) j=8;
			else j = k->toplamMesaj;
			for (i=0;i<j;i++)
			{	// "j" adet mesajý ekrana yazdýrýlýr.
				msj = k->ileti[ ((k->toplamMesaj)-1)-i ];
				uz = strlen(msj.mesaj);
				if ( msj.kaynak=='O' )
				{	// Karþý tarafýn mesajýný ekrana yazdýr.
					SetConsoleTextAttribute(hKonsol, 143);
					GolgeCiz( 2 , (40-(5*i)) , (uz+7) , 3 );
					SetConsoleTextAttribute(hKonsol, 224);
					KutuCiz( 2 , (40-(5*i)) , (uz+7) , 3 );
					gotoxy( 5 , (41-(5*i)) );
					printf("%s" , msj.mesaj );
					gotoxy( (uz+3) , (42-(5*i)) );
					SetConsoleTextAttribute(hKonsol, 230);
					printf("%2.2d.%2.2d" , msj.tarih.saat , msj.tarih.dakika );
				} else
				{	// Kullanýcýnýn mesajýný ekrana yazdýr.
					SetConsoleTextAttribute(hKonsol, 143);
					GolgeCiz( (111-uz) , (40-(5*i)) , (uz+7) , 3 );
					SetConsoleTextAttribute(hKonsol, 160);
					KutuCiz( (111-uz) , (40-(5*i)) , (uz+7) , 3 );
					gotoxy( (114-uz) , (41-(5*i)) );
					printf("%s" , msj.mesaj );
					gotoxy( 112 , (42-(5*i)) );
					SetConsoleTextAttribute(hKonsol, 162);
					printf("%2.2d.%2.2d" , msj.tarih.saat , msj.tarih.dakika );
				}
			}
		}
		ymsj=0;
		gotoxy( ( strlen(mesaj) +2) , 47 );	// Yazma koordinatlarýna git.
		
		do {
			
			FD_ZERO( &readfds );
			FD_SET( sunucu , &readfds );
			aktivite = select( 0 , &readfds , NULL , NULL , &tm);
			if ( FD_ISSET( sunucu , &readfds) )
			{	// Sunucudan gelen birþeyler var ona bak.
				recv( sunucu , arabellek , 100 , 0);
				send( sunucu , "X" , 1 , 0 );			// ONAY yolla
				// Mesajý al ve gereken iþlemleri yap.
				recv( sunucu , (char*)&(y_ileti) , sizeof(S_ILETI) , 0);
				gelen.kaynak = 'O';
				gelen.tarih = y_ileti.tarih;
				strcpy( gelen.mesaj , y_ileti.mesaj );
				hata=1;
				for ( i=0 ; i<rehberS ; i++ )
				{	// Tüm rehberde mesajýn kaynaðýný ara.
					if ( strcmp( y_ileti.kaynak , rehber[i].telefon ) == 0 )
					{	// Rehberde numara varsa mesajý ona ekle.
						rehber[i].ileti[rehber[i].toplamMesaj] = gelen;
						rehber[i].toplamMesaj++;
						rehber[i].yeniMesaj++;
						hata=0;
						break;
					}
				}
				if ( hata == 1 )
				{	// Kiþi ekli deðilse yeni kiþi oluþturulur ve mesaj ona eklenir.
					strcpy( rehber[rehberS].telefon , y_ileti.kaynak );
					strcpy( rehber[rehberS].isim , y_ileti.isim );
					rehber[rehberS].yeniMesaj = 1;
					rehber[rehberS].toplamMesaj = 1;
					rehber[rehberS].ileti[0] = gelen;
					rehberS++;
				}
				if ( strcmp( y_ileti.kaynak , k->telefon )==0 ) ymsj=1;
				else printf("%c",7);
				gotoxy( ( strlen(mesaj) +2) , 47 );	// Yazma koordinatlarýna git.
			}
			
			if( _kbhit() )
			{	// Tuþa basýldýysa yapýlacak iþlemler
				ch = _getch();
				if(ch == -32) ch=_getch();
				else
				{	// 2 bytelýk tuþa basýlmadý.
					uz = strlen(mesaj);
					gotoxy( (uz+2) , 47 );			// Yazma koordinatlarýna git.
					SetConsoleTextAttribute(hKonsol, 111);
					if (ch==27) tus=10;				// ESC TUÞU
					else if (ch==13) 				// ENTER TUÞU
					{
						KutuCiz( 2 , 47 , 101 , 1 );
						if ( uz>0 )
						{
							mesaj[101]=NULL;
							strcpy( y_ileti.hedef , k->telefon ); 	// Hedef Telefon numarasý
							strcpy( y_ileti.isim , k->isim );	 	// Hedefin kullanýcýda kayýtlý ismi
							strcpy( y_ileti.mesaj , mesaj );		// Gönderilecek mesaj
							y_ileti.kaynak[0]=NULL;
							send( sunucu , "M" , 1 , 0 );			// Mesaj Komutu Gönderilir
							recv( sunucu , str , 5  ,0 );			// ONAY al
							send( sunucu , (char*)&(y_ileti) , sizeof(S_ILETI) , 0 );	// Sunucuya mesaj ve bilgileri gönderilir.
							recv( sunucu , (char*)&(y_ileti) , sizeof(S_ILETI) , 0 );	// Sunucudan onay mahiyetine tarihle güncellenmiþ mesaj alýnýr.
							gelen.kaynak='B';	// Kaynak B: Ben
							gelen.tarih=y_ileti.tarih;
							strcpy( gelen.mesaj , y_ileti.mesaj );
							k->ileti[k->toplamMesaj]=gelen;
							k->toplamMesaj++;
							ymsj=1;
							gotoxy( 2 , 47 );	// Yazma koordinatlarýna git.
						}
						mesaj[0]=NULL;
					} else if (ch==8)					// SÝL TUÞU
					{
						if ( uz>0 )
						{
							mesaj[uz-1]=NULL;
							printf("%c %c" , 8 , 8 );
						}
					} else
					{
						if (ch==-104) ch='Ý';
						else if (ch==-115) ch='ý';
						else if (ch== -90) ch='Ð';
						else if (ch== -89) ch='ð';
						else if (ch==-102) ch='Ü';
						else if (ch==-127) ch='ü';
						else if (ch== -98) ch='Þ';
						else if (ch== -97) ch='þ';
						else if (ch==-103) ch='Ö';
						else if (ch==-108) ch='ö';
						else if (ch==-128) ch='Ç';
						else if (ch==-121) ch='ç';
						if ( uz<99 )
						{
							mesaj[uz]=ch;
							mesaj[uz+1]=NULL;
							printf("%c" , ch );
						}
					}
				}
			}
			if (ymsj==1)
			{	// Yeni mesaj var ekraný güncelle.
				ymsj=0;
				SetConsoleTextAttribute(hKonsol, 112);		// BOÞ ARKA PLAN
				KutuCiz(0,3,120,42);
				k->yeniMesaj=0;
				if ( (k->toplamMesaj) > 0 )
				{	// Mesajlarý ekrana yazdýr.
					if ( k->toplamMesaj > 8 ) j=8;
					else j = k->toplamMesaj;
					for ( i=0 ; i<j ; i++ )
					{	// "j" adet mesajý ekrana yazdýrýlýr.
						msj = k->ileti[ ((k->toplamMesaj)-1)-i ];
						uz = strlen(msj.mesaj);
						if ( msj.kaynak=='O' )
						{	// Karþý tarafýn mesajýný ekrana yazdýr.
							SetConsoleTextAttribute(hKonsol, 143);
							GolgeCiz( 2 , (40-(5*i)) , (uz+7) , 3 );
							SetConsoleTextAttribute(hKonsol, 224);
							KutuCiz( 2 , (40-(5*i)) , (uz+7) , 3 );
							gotoxy( 5 , (41-(5*i)) );
							printf("%s" , msj.mesaj );
							gotoxy( (uz+3) , (42-(5*i)) );
							SetConsoleTextAttribute(hKonsol, 230);
							printf("%2.2d.%2.2d" , msj.tarih.saat , msj.tarih.dakika );
						} else
						{	// Kullanýcýnýn mesajýný ekrana yazdýr.
							SetConsoleTextAttribute(hKonsol, 143);
							GolgeCiz( (111-uz) , (40-(5*i)) , (uz+7) , 3 );
							SetConsoleTextAttribute(hKonsol, 160);
							KutuCiz( (111-uz) , (40-(5*i)) , (uz+7) , 3 );
							gotoxy( (114-uz) , (41-(5*i)) );
							printf("%s" , msj.mesaj );
							gotoxy( 112 , (42-(5*i)) );
							SetConsoleTextAttribute(hKonsol, 162);
							printf("%2.2d.%2.2d" , msj.tarih.saat , msj.tarih.dakika );
						}
					}
				}
				gotoxy( 2 , 47 );	// Yazma koordinatlarýna git.
			}
		} while ( tus != 10 ); // ESC GERÝ
		
	}
	printf("Program Çöktü");
	getch();
	return 0;
}

int satirOku (short uzunluk, char *string)
{
	char ch, ch2;
	int strHarf;
	
	do {
		ch=_getch();
		if (ch==-32)
		{
			ch2=_getch();
			if (ch2==72) return 1;   // ÜST OK
			if (ch2==80) return 2;   // ALT OK
			if (ch2==75) return 3;   // SOL OK
			if (ch2==77) return 4;   // SAÐ OK
			//printf ("ERROR: 100");
			return 100;
		}
		if (ch==13) return 11;	// ENTER TUÞU
		if (ch==27) return 12;	// ESC TUÞU
		
		strHarf=strlen(string);
		
		if ((ch==8) && (strHarf>=1))
		{    // SÝLME TUÞU
			string[strHarf-1]=0;
			printf("%c%c%c", 8, 0, 8);
		} else if (strHarf<(uzunluk-1))
		{
			if ( (ch>='a' && ch<='z') || (ch>='A' && ch<='Z') || (ch>='0' && ch<='9') || (ch==' ') )
			{
				string[strHarf]=ch;
				printf("%c",ch);
			}
			else if (ch==-104) { string[strHarf]='Ý'; printf("Ý"); }
			else if (ch==-115) { string[strHarf]='ý'; printf("ý"); }
			else if (ch==-90)  { string[strHarf]='Ð'; printf("Ð"); }
			else if (ch==-89)  { string[strHarf]='ð'; printf("ð"); }
			else if (ch==-102) { string[strHarf]='Ü'; printf("Ü"); }
			else if (ch==-127) { string[strHarf]='ü'; printf("ü"); }
			else if (ch==-98)  { string[strHarf]='Þ'; printf("Þ"); }
			else if (ch==-97)  { string[strHarf]='þ'; printf("þ"); }
			else if (ch==-103) { string[strHarf]='Ö'; printf("Ö"); }
			else if (ch==-108) { string[strHarf]='ö'; printf("ö"); }
			else if (ch==-128) { string[strHarf]='Ç'; printf("Ç"); }
			else if (ch==-121) { string[strHarf]='ç'; printf("ç"); }
		}
	} while (1);
	return 0;
}

int TusOku(char *ch)
{
	if( _kbhit() )
	{
		ch = _getch();
		if(ch == -32)
		{	// YÖN TUÞLARI
			ch=_getch();
			if (ch==72) return 11;   // ÜST OK
			if (ch==80) return 12;   // ALT OK
			if (ch==75) return 13;   // SOL OK
			if (ch==77) return 14;   // SAÐ OK
		} else
		{
			if (ch== 8) return 2;	// SÝL TUÞU
			if (ch==13) return 3;	// ENTER TUÞU
			if (ch==27) return 10;	// ESC TUÞU
			if (ch==-104) { ch='Ý'; return 1; }
			if (ch==-115) { ch='ý'; return 1; }
			if (ch== -90) { ch='Ð'; return 1; }
			if (ch== -89) { ch='ð'; return 1; }
			if (ch==-102) { ch='Ü'; return 1; }
			if (ch==-127) { ch='ü'; return 1; }
			if (ch== -98) { ch='Þ'; return 1; }
			if (ch== -97) { ch='þ'; return 1; }
			if (ch==-103) { ch='Ö'; return 1; }
			if (ch==-108) { ch='ö'; return 1; }
			if (ch==-128) { ch='Ç'; return 1; }
			if (ch==-121) { ch='ç'; return 1; }
			return 1;
		}
	}
	else return 0;
}
