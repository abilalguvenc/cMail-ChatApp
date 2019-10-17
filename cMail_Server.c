#include <stdio.h>
#include <winsock2.h>
#include <locale.h>
#include <windows.h>
#include "lib/ileti.h"

#define anarenk 241
#define kirmizi 252
#define LIMIT 1024
#define toplamAlici 256

#pragma comment(lib, "ws2_32.lib") //Winsock Library

int main(int argc , char *argv[])
{
    // T�rk�e karakterde okuma ve yazma i�lemini yapabilmek i�in gereken kod.
    setlocale(LC_ALL,"Turkish");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    struct sockaddr_in server, address;
    SOCKET master , new_socket , s;
    WSADATA wsa;
	fd_set readfds;
	KULLANICI kisi[1024] , y_kisi , *k;
	KSOKET kanal[256] ;
	S_ILETI y_ileti , iletiler[2000] ;
	
	char *arabellek, str[5];	// Gelecek mesajlar i�in arabellek (string) olu�turulur.
	arabellek =  (char*) malloc((LIMIT + 1) * sizeof(char));
	
	int activity, addrlen, i, j, valread, toplamKisi, idK, telK , bu , toplamIleti;
	char *message = "cPosta";
	SetConsoleTitle("cPosta Sunucusu v1.0");
	system("color f1");
	
    for(i = 0 ; i < toplamAlici ; i++) 
	{
		kanal[i].so = 0;
		kanal[i].ku=0;
	}
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Winsock y�klenemedi. (Hata %d)\n",WSAGetLastError());
        do {} while ( getch() != 13 );
        exit(EXIT_FAILURE);
    }

    printf("Winsock y�klendi. ");

    // Soket olu�turulur.
    if((master = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Soket olu�turulamad�. (Hata %d)\n" , WSAGetLastError());
        do {} while ( getch() != 13 );
        exit(EXIT_FAILURE);
    }

    printf("Soket olu�turuldu. ");

    // Struct� sockaddr_in olan server tan�m� yap�l�r.
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    // Ba� (Bind)
    if( bind(master ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Ba�lant� sa�lanamad�. (Hata %d)\n" , WSAGetLastError());
        do {} while ( getch() != 13 );
        exit(EXIT_FAILURE);
    }
	
    printf("Ba�lant� kuruldu. ");

    // Gelen ba�lant�lar� dinle ve kabul et.
    listen(master , 3);
    printf("\nE�ler bekleniyor...\n\n");
    addrlen = sizeof(struct sockaddr_in);
	
	// ADMIN hesab� olu�turulur ve ki�i say�s� ilk de�eri verilir.
	strcpy( kisi[0].kullaniciAdi , "admin");
	strcpy( kisi[0].telefon , "Y�NET�C�");
	strcpy( kisi[0].isim , "Y�netici");
	strcpy( kisi[0].sifre , "admin");
	toplamKisi=1;
	toplamIleti=0;
	
    while ( TRUE )
    {
		for ( bu=0 ; bu<toplamAlici ; bu++ )
		{
			s = kanal[bu].so;	// Soketin bilgisini tutar.
			k = kanal[bu].ku;	// Soketin ki�i bilgisini tutar.
			// �uanki kullan�c�ya g�nderilecek mesajlar� bulup g�nder.
			if ( (kanal[bu].so != NULL) && (kanal[bu].ku != NULL) )
			{	// "bu" kullan�c�ya gelen mesaj var m� kontrol et.
				for (i=0 ;i<toplamIleti;i++)
				{
					if ( strcmp( iletiler[i].hedef , k->telefon ) == 0 )
					{	// �leti hedefine g�nderilir.
						send( s , "m" , 1 , 0 );									// Mesaj kodu g�nderilir.
						recv( s , str , 5  ,0 );									// ONAY al
						send( s , (char*)&(iletiler[i]) , sizeof(S_ILETI) , 0 );	// Mesaj bilgileri g�nderilir.
						SetConsoleTextAttribute(hConsole, kirmizi);
						printf("Mesaj �letildi ");
						SetConsoleTextAttribute(hConsole, anarenk);
						printf("%s (%s) -> %s (%s)\n" , iletiler[i].isim , iletiler[i].kaynak , k->isim , k->telefon );
						toplamIleti--;
						for (j=i;j<toplamIleti;j++)									// G�nderilen mesaj silinir.
							iletiler[j] = iletiler[j+1];
					}
				}
			}
		}
		
        FD_ZERO(&readfds);						// Soket fd set s�f�rlan�r.
        FD_SET(master, &readfds);				// Master socket, fd set'e eklenir.
        for (  i = 0 ; i < toplamAlici ; i++)	// Soketler fd set'e eklenir
        {
            s = kanal[i].so;
            if(s > 0) FD_SET( s , &readfds);
        }
        activity = select( 0 , &readfds , 0 , 0 , 0);

        if ( activity == SOCKET_ERROR )			// Hata varm� kontrol et.
        {
            printf("Se�im �a�r�lamad�. (Hata %d)\n" , WSAGetLastError());
            do {} while ( getch() != 13 );
            exit(EXIT_FAILURE);
        }
        if (FD_ISSET(master , &readfds))		// Master soket, yeni ba�lant� oldu�unu g�sterir.
        {
            if ((new_socket = accept(master , (struct sockaddr *)&address, (int *)&addrlen))<0)
            {
                perror("Kabul hatas�.\n");
                do {} while ( getch() != 13 );
                exit(EXIT_FAILURE);
            }
			SetConsoleTextAttribute(hConsole, kirmizi);
			printf("Ba�la ");
			SetConsoleTextAttribute(hConsole, anarenk);
			printf(" %s:%d %d", inet_ntoa(address.sin_addr) , ntohs(address.sin_port) , new_socket );
            // Ba�lanan ki�iye ho�geldin mesaj� g�nder
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
            {
                perror("Al�c� ile sunucu arasindaki do�rulama e�le�medi.\n");
            }
            for (i = 0; i < toplamAlici; i++)
            {	// Ba�lanan ki�iyi listedeki ilk bo�lu�a ekle.
                if (kanal[i].so == 0)
                {
                    kanal[i].so = new_socket;
					kanal[i].ku = 0;
                    printf(" (kanal %d)\n" , i);
                    break;
                }
            }
        }
		
        for (bu = 0; bu < toplamAlici ; bu++)
        {	// Yeni mesajlar varsa gereken i�lemler yap�lacak.
            s = kanal[bu].so;	// Soketin bilgisini tutar.
			k = kanal[bu].ku;	// Soketin ki�i bilgisini tutar.
			
            if (FD_ISSET( s , &readfds))
            {	// Ba�lant� bir�ey g�nderiyorsa ba�lant�n�n bilgisi al�n�r.
                getpeername( s , (struct sockaddr*)&address , (int*)&addrlen );

                // Kapatma komutu olup olmad��� kontrol edilir.
				arabellek[0] = NULL;
                valread = recv( s , arabellek, LIMIT, 0);
                if( valread == SOCKET_ERROR)
                {
                    int hata_kodu = WSAGetLastError();
                    if( hata_kodu == WSAECONNRESET )
                    {	// Ba�lant� kesildi (disconnected)
                        SetConsoleTextAttribute(hConsole, kirmizi);
			            printf( "��k�� " );
						k = kanal[bu].ku;
			            SetConsoleTextAttribute(hConsole, anarenk);
                        if ( k == 0 ) printf ("%s:%d (%d)\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s);
						else printf("%s:%d %d - %s (%s) ayr�ld�.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s,k->isim,k->telefon);
                        // Soketi tekrar kullanabilmek i�in soket bilgileri s�f�rlan�r.
                        closesocket( s );
                        kanal[bu].so = 0;
						kanal[bu].ku = 0;
                    } else printf("Al�c� hata verdi. (Hata %d)\n" , hata_kodu);
                } else
                if ( valread == 0)
                {	// Ba�lant� kesildi (disconnect)
                    SetConsoleTextAttribute(hConsole, kirmizi);
                    printf("��k�� ");
					k = kanal[bu].ku;
			        SetConsoleTextAttribute(hConsole, anarenk);
					if ( k == 0 ) printf ("%s:%d (%d)\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s);
                    else printf("%s:%d %d - %s (%s) ayr�ld�.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s,k->isim,k->telefon);
                    // Soketi tekrar kullanabilmek i�in soket bilgileri s�f�rlan�r.
                    closesocket( s );
                    kanal[bu].so = 0;
					kanal[bu].ku = 0;
                } else
                {	// Hi�bir hata yoksa kullan�c� bir eylem ger�ekle�tirmek istiyor. �stenilen eylemin hangi eylem oldu�unu bul.
					if ( arabellek[0] == 'M' )
					{	// MESAJ G�NDERME ��LEM� YAP ##############################################################################
						send( s , "X" , 1 , 0 );								// ONAY yolla
						SetConsoleTextAttribute(hConsole, kirmizi);
						printf( "Mesaj " );
						recv( s , (char*)&(y_ileti) , sizeof(S_ILETI) , 0 );	// Mesaj bilgilerir al�n�r.
						strcpy( y_ileti.kaynak , k->telefon );					// Kaynak telefon numaras� g�ncellenir.
						y_ileti.tarih = TarihBul();								// Tarih bilgisi g�ncellenir.
						SetConsoleTextAttribute(hConsole, anarenk);
						printf("%s (%s) -> %s (%s)\n",k->isim,k->telefon,y_ileti.isim,y_ileti.hedef);
						SetConsoleTextAttribute(hConsole, 229);
						printf(" %s \n" , y_ileti.mesaj);
						strcpy( y_ileti.isim , k->isim );						// "isim" k�sm�na kayna��n ismi kopyalan�r.
						iletiler[toplamIleti]=y_ileti;							// "iletiler"e yeni ileti eklenir
						toplamIleti++;											// Toplam ileti say�s� artt�r�l�r.
						send( s , (char*)&(y_ileti) , sizeof(S_ILETI) , 0 );	// Mesaj� g�nderene mesaj�n g�ncel hali yollan�r.						
					} else if ( arabellek[0] == 'E' )
					{	// K��� EKLEME ��LEM� YAP #################################################################################
						send( s , "X" , 1 , 0 );								// ONAY yolla
						arabellek[ recv( s , arabellek , 11  ,0 )]=NULL;		// Telefon numaras�
						arabellek[10]=NULL;
						for (i=0;i<toplamKisi;i++) if ( strcmp( kisi[i].telefon , arabellek ) == 0) break;
						if ( i == toplamKisi )
						{	// Aran�lan numara kay�tl� de�il.	Y: yok
							send( s , "Y" , 1 , 0 );
							SetConsoleTextAttribute(hConsole, kirmizi);
							printf("Ki�i Yok ");
							SetConsoleTextAttribute(hConsole, anarenk);
							printf("%s (%s), %s numaral� telefonu aratt�.\n",k->isim,k->telefon,arabellek);
						} else
						{	// Aran�lan numara kay�tl�.			V: var
							send( s , "V" , 1 , 0 );
							recv( s , str , 5  ,0 );											// ONAY al
							send( s , kisi[i].isim , strlen( kisi[i].isim ) , 0 );
							SetConsoleTextAttribute(hConsole, kirmizi);
							printf("Ki�i Ekle ");
							SetConsoleTextAttribute(hConsole, anarenk);
							printf("%s (%s), %s (%s) ki�isini ekledi.\n",k->isim,k->telefon,kisi[i].isim,arabellek);
						}
					} else if ( arabellek[0] == 'G' )
					{	// G�R�� ��LEM� YAP #######################################################################################
						kanal[bu].ku = 0;
						send( s , "X" , 1 , 0 );												// ONAY yolla
						y_kisi.kullaniciAdi[ recv( s , y_kisi.kullaniciAdi , 25  ,0 )]=NULL;	// ID al
						y_kisi.kullaniciAdi[24]=NULL;
						send( s , "X" , 1 , 0 );												// ONAY yolla
						y_kisi.sifre[ recv( s , y_kisi.sifre , 25  ,0 )]=NULL;					// PW al
						y_kisi.sifre[24]=NULL;
						for (i=0;i<toplamKisi;i++)
						{	// Girilen kullan�c� ad� kay�tl� m� bul.
							if ( strcmp( y_kisi.kullaniciAdi , kisi[i].kullaniciAdi ) == 0 )
							{	// Kullan�c� ad� e�le�ti
								if ( strcmp( y_kisi.sifre , kisi[i].sifre ) == 0 )
								{	// �ifre de e�le�ti.
									send( s , "D" , 1 , 0 );									// G�R�� �ZN� yolla
									recv( s , str , 1  ,0 );									// ONAY al
									send( s , kisi[i].isim , strlen(kisi[i].isim) , 0 );		// AD yolla
									recv( s , str , 1  ,0 );									// ONAY al
									send( s , kisi[i].telefon , strlen(kisi[i].telefon) , 0 );	// TEL yolla
									kanal[bu].ku = &kisi[i];									
									SetConsoleTextAttribute(hConsole, kirmizi);
									printf( "Giri� " );
									SetConsoleTextAttribute(hConsole, anarenk);
									printf("%s:%d %d - %s (%s) giri� yapt�.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s,kisi[i].isim,kisi[i].telefon);
								} else
								{	// �ifre hatal� girildi.
									send( s , "Y" , 1 , 0 );									// HATA yolla
									SetConsoleTextAttribute(hConsole, kirmizi);
									printf( "Hatal� Giri� " );
									SetConsoleTextAttribute(hConsole, anarenk);
									printf("%s:%d %d - %s (%s) �ifre hatal�.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s,kisi[i].isim,kisi[i].telefon);
								}
								break;
							}
						}
						if ( i == toplamKisi ) 
						{	// Kullan�c� ad� kay�tl� de�il.
							send( s , "I" , 1 , 0 ); 											// HATA yolla
							SetConsoleTextAttribute(hConsole, kirmizi);
							printf( "Hatal� Giri� " );
							SetConsoleTextAttribute(hConsole, anarenk);
							printf("%s:%d %d - Kullan�c� ad� yok.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port));
						}
					} else if ( arabellek[0] == 'K' )
					{	// KAYIT ��LEM� YAP #######################################################################################
						send( s , "X" , 1 , 0 );												// ONAY yolla
						y_kisi.isim[ recv( s , y_kisi.isim , 25  ,0 )]=NULL; 					// AD al
						y_kisi.isim[24]=NULL;
						send( s , "X" , 1 , 0 );												// ONAY yolla
						y_kisi.telefon[ recv( s , y_kisi.telefon , 11  ,0 )]=NULL; 				// TEL al
						y_kisi.telefon[10]=NULL;
						send( s , "X" , 1 , 0 );												// ONAY yolla
						y_kisi.kullaniciAdi[ recv( s , y_kisi.kullaniciAdi , 25  ,0 )]=NULL; 	// ID al
						y_kisi.kullaniciAdi[24]=NULL;
						send( s , "X" , 1 , 0 );												// ONAY yolla
						y_kisi.sifre[ recv( s , y_kisi.sifre , 25  ,0 )]=NULL; 					// PW al
						y_kisi.sifre[24]=NULL;
						k = &y_kisi;
						if ( (strlen(k->isim)<4) || (strlen(k->telefon)<4) || (strlen(k->kullaniciAdi)<4) || (strlen(k->sifre)<4) )
						{
							send( s , "H" , 1 , 0 );												// HATA yolla
							SetConsoleTextAttribute(hConsole, kirmizi);
							printf("Hatal� Kay�t ");
							SetConsoleTextAttribute(hConsole, anarenk);
							printf("%s:%d %d Hatal� kay�t denemesi.\n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port) , s );
						} else
						{
							idK=0; telK=0;
							for(i=0;i<toplamKisi;i++) 
							{
								if ( strcmp( kisi[i].kullaniciAdi , k->kullaniciAdi ) == 0 ) idK=1;
								if ( strcmp( kisi[i].telefon , k->telefon ) == 0 ) telK=1;
							}
							if ( (idK + telK) == 2 ) 
							{
								send( s , "H" , 1 , 0 );											// HATA yolla
								SetConsoleTextAttribute(hConsole, kirmizi);
								printf("Hatal� Kay�t ");
								SetConsoleTextAttribute(hConsole, anarenk);
								printf("%s:%d %d - K. Ad� (%s) ve tel. (%s) zaten kay�tl�.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s,k->kullaniciAdi,k->telefon);
							} else  if ( telK == 1 ) 
							{
								send( s , "T" , 1 , 0 );											// HATA yolla
								SetConsoleTextAttribute(hConsole, kirmizi);
								printf("Hatal� Kay�t ");
								SetConsoleTextAttribute(hConsole, anarenk);
								printf("%s:%d %d - Telefon (%s) zaten kay�tl�.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s,k->telefon);
							} else  if ( idK == 1 ) 
							{
								send( s , "I" , 1 , 0 );											// HATA yolla
								SetConsoleTextAttribute(hConsole, kirmizi);
								printf("Hatal� Kay�t ");
								SetConsoleTextAttribute(hConsole, anarenk);
								printf("%s:%d %d - K. Ad� (%s) zaten kay�tl�.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s,k->kullaniciAdi);
							}else {
								strcpy( kisi[toplamKisi].isim , k->isim );
								strcpy( kisi[toplamKisi].telefon , k->telefon );
								strcpy( kisi[toplamKisi].kullaniciAdi , k->kullaniciAdi );
								strcpy( kisi[toplamKisi].sifre , k->sifre );
								kanal[bu].ku = &kisi[toplamKisi];
								SetConsoleTextAttribute(hConsole, kirmizi);
								printf( "Kay�t " );
								SetConsoleTextAttribute(hConsole, anarenk);
								printf (" %s (%s) - KA: %s �f: %s\n" , k->isim , k->telefon , k->kullaniciAdi , k->sifre );
								SetConsoleTextAttribute(hConsole, kirmizi);
								printf( "Giri� " );
								SetConsoleTextAttribute(hConsole, anarenk);
								printf("%s:%d %d - %s (%s) giri� yapt�.\n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port) , s , k->isim , k->telefon );
								send( s , "D" , 1 , 0 );											// G�R�� �ZN� yolla
								toplamKisi++;
							}
						}
					} else
					{	// Yanl�� bir mesaj yolland�. Ba�lant�y� kes. (disconnect)
						SetConsoleTextAttribute(hConsole, kirmizi);
						printf("��k�� ");
						SetConsoleTextAttribute(hConsole, anarenk);
						if ( k == 0 ) printf ("%s:%d (%d)\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s);
						else printf("%s:%d %d - %s (%s) ayr�ld�.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s,k->isim,k->telefon);
						// Soketi tekrar kullanabilmek i�in kapat�l�p s�f�rlan�r.
						closesocket( s );
						kanal[bu].so = 0;
						kanal[bu].ku = 0;
					}
                }
            }
        }
    }

    closesocket(s);
    WSACleanup();

    printf("OLAMAAAAZ PROGRAM ��KT� :/\n");
    do {} while ( getch() != 13 );
    return 0;
}
