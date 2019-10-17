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
    // Türkçe karakterde okuma ve yazma iþlemini yapabilmek için gereken kod.
    setlocale(LC_ALL,"Turkish");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    struct sockaddr_in server, address;
    SOCKET master , new_socket , s;
    WSADATA wsa;
	fd_set readfds;
	KULLANICI kisi[1024] , y_kisi , *k;
	KSOKET kanal[256] ;
	S_ILETI y_ileti , iletiler[2000] ;
	
	char *arabellek, str[5];	// Gelecek mesajlar için arabellek (string) oluþturulur.
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
        printf("Winsock yüklenemedi. (Hata %d)\n",WSAGetLastError());
        do {} while ( getch() != 13 );
        exit(EXIT_FAILURE);
    }

    printf("Winsock yüklendi. ");

    // Soket oluþturulur.
    if((master = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Soket oluþturulamadý. (Hata %d)\n" , WSAGetLastError());
        do {} while ( getch() != 13 );
        exit(EXIT_FAILURE);
    }

    printf("Soket oluþturuldu. ");

    // Structý sockaddr_in olan server tanýmý yapýlýr.
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    // Bað (Bind)
    if( bind(master ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Baðlantý saðlanamadý. (Hata %d)\n" , WSAGetLastError());
        do {} while ( getch() != 13 );
        exit(EXIT_FAILURE);
    }
	
    printf("Baðlantý kuruldu. ");

    // Gelen baðlantýlarý dinle ve kabul et.
    listen(master , 3);
    printf("\nEþler bekleniyor...\n\n");
    addrlen = sizeof(struct sockaddr_in);
	
	// ADMIN hesabý oluþturulur ve kiþi sayýsý ilk deðeri verilir.
	strcpy( kisi[0].kullaniciAdi , "admin");
	strcpy( kisi[0].telefon , "YÖNETÝCÝ");
	strcpy( kisi[0].isim , "Yönetici");
	strcpy( kisi[0].sifre , "admin");
	toplamKisi=1;
	toplamIleti=0;
	
    while ( TRUE )
    {
		for ( bu=0 ; bu<toplamAlici ; bu++ )
		{
			s = kanal[bu].so;	// Soketin bilgisini tutar.
			k = kanal[bu].ku;	// Soketin kiþi bilgisini tutar.
			// Þuanki kullanýcýya gönderilecek mesajlarý bulup gönder.
			if ( (kanal[bu].so != NULL) && (kanal[bu].ku != NULL) )
			{	// "bu" kullanýcýya gelen mesaj var mý kontrol et.
				for (i=0 ;i<toplamIleti;i++)
				{
					if ( strcmp( iletiler[i].hedef , k->telefon ) == 0 )
					{	// Ýleti hedefine gönderilir.
						send( s , "m" , 1 , 0 );									// Mesaj kodu gönderilir.
						recv( s , str , 5  ,0 );									// ONAY al
						send( s , (char*)&(iletiler[i]) , sizeof(S_ILETI) , 0 );	// Mesaj bilgileri gönderilir.
						SetConsoleTextAttribute(hConsole, kirmizi);
						printf("Mesaj Ýletildi ");
						SetConsoleTextAttribute(hConsole, anarenk);
						printf("%s (%s) -> %s (%s)\n" , iletiler[i].isim , iletiler[i].kaynak , k->isim , k->telefon );
						toplamIleti--;
						for (j=i;j<toplamIleti;j++)									// Gönderilen mesaj silinir.
							iletiler[j] = iletiler[j+1];
					}
				}
			}
		}
		
        FD_ZERO(&readfds);						// Soket fd set sýfýrlanýr.
        FD_SET(master, &readfds);				// Master socket, fd set'e eklenir.
        for (  i = 0 ; i < toplamAlici ; i++)	// Soketler fd set'e eklenir
        {
            s = kanal[i].so;
            if(s > 0) FD_SET( s , &readfds);
        }
        activity = select( 0 , &readfds , 0 , 0 , 0);

        if ( activity == SOCKET_ERROR )			// Hata varmý kontrol et.
        {
            printf("Seçim çaðrýlamadý. (Hata %d)\n" , WSAGetLastError());
            do {} while ( getch() != 13 );
            exit(EXIT_FAILURE);
        }
        if (FD_ISSET(master , &readfds))		// Master soket, yeni baðlantý olduðunu gösterir.
        {
            if ((new_socket = accept(master , (struct sockaddr *)&address, (int *)&addrlen))<0)
            {
                perror("Kabul hatasý.\n");
                do {} while ( getch() != 13 );
                exit(EXIT_FAILURE);
            }
			SetConsoleTextAttribute(hConsole, kirmizi);
			printf("Baðla ");
			SetConsoleTextAttribute(hConsole, anarenk);
			printf(" %s:%d %d", inet_ntoa(address.sin_addr) , ntohs(address.sin_port) , new_socket );
            // Baðlanan kiþiye hoþgeldin mesajý gönder
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
            {
                perror("Alýcý ile sunucu arasindaki doðrulama eþleþmedi.\n");
            }
            for (i = 0; i < toplamAlici; i++)
            {	// Baðlanan kiþiyi listedeki ilk boþluða ekle.
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
        {	// Yeni mesajlar varsa gereken iþlemler yapýlacak.
            s = kanal[bu].so;	// Soketin bilgisini tutar.
			k = kanal[bu].ku;	// Soketin kiþi bilgisini tutar.
			
            if (FD_ISSET( s , &readfds))
            {	// Baðlantý birþey gönderiyorsa baðlantýnýn bilgisi alýnýr.
                getpeername( s , (struct sockaddr*)&address , (int*)&addrlen );

                // Kapatma komutu olup olmadýðý kontrol edilir.
				arabellek[0] = NULL;
                valread = recv( s , arabellek, LIMIT, 0);
                if( valread == SOCKET_ERROR)
                {
                    int hata_kodu = WSAGetLastError();
                    if( hata_kodu == WSAECONNRESET )
                    {	// Baðlantý kesildi (disconnected)
                        SetConsoleTextAttribute(hConsole, kirmizi);
			            printf( "Çýkýþ " );
						k = kanal[bu].ku;
			            SetConsoleTextAttribute(hConsole, anarenk);
                        if ( k == 0 ) printf ("%s:%d (%d)\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s);
						else printf("%s:%d %d - %s (%s) ayrýldý.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s,k->isim,k->telefon);
                        // Soketi tekrar kullanabilmek için soket bilgileri sýfýrlanýr.
                        closesocket( s );
                        kanal[bu].so = 0;
						kanal[bu].ku = 0;
                    } else printf("Alýcý hata verdi. (Hata %d)\n" , hata_kodu);
                } else
                if ( valread == 0)
                {	// Baðlantý kesildi (disconnect)
                    SetConsoleTextAttribute(hConsole, kirmizi);
                    printf("Çýkýþ ");
					k = kanal[bu].ku;
			        SetConsoleTextAttribute(hConsole, anarenk);
					if ( k == 0 ) printf ("%s:%d (%d)\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s);
                    else printf("%s:%d %d - %s (%s) ayrýldý.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s,k->isim,k->telefon);
                    // Soketi tekrar kullanabilmek için soket bilgileri sýfýrlanýr.
                    closesocket( s );
                    kanal[bu].so = 0;
					kanal[bu].ku = 0;
                } else
                {	// Hiçbir hata yoksa kullanýcý bir eylem gerçekleþtirmek istiyor. Ýstenilen eylemin hangi eylem olduðunu bul.
					if ( arabellek[0] == 'M' )
					{	// MESAJ GÖNDERME ÝÞLEMÝ YAP ##############################################################################
						send( s , "X" , 1 , 0 );								// ONAY yolla
						SetConsoleTextAttribute(hConsole, kirmizi);
						printf( "Mesaj " );
						recv( s , (char*)&(y_ileti) , sizeof(S_ILETI) , 0 );	// Mesaj bilgilerir alýnýr.
						strcpy( y_ileti.kaynak , k->telefon );					// Kaynak telefon numarasý güncellenir.
						y_ileti.tarih = TarihBul();								// Tarih bilgisi güncellenir.
						SetConsoleTextAttribute(hConsole, anarenk);
						printf("%s (%s) -> %s (%s)\n",k->isim,k->telefon,y_ileti.isim,y_ileti.hedef);
						SetConsoleTextAttribute(hConsole, 229);
						printf(" %s \n" , y_ileti.mesaj);
						strcpy( y_ileti.isim , k->isim );						// "isim" kýsmýna kaynaðýn ismi kopyalanýr.
						iletiler[toplamIleti]=y_ileti;							// "iletiler"e yeni ileti eklenir
						toplamIleti++;											// Toplam ileti sayýsý arttýrýlýr.
						send( s , (char*)&(y_ileti) , sizeof(S_ILETI) , 0 );	// Mesajý gönderene mesajýn güncel hali yollanýr.						
					} else if ( arabellek[0] == 'E' )
					{	// KÝÞÝ EKLEME ÝÞLEMÝ YAP #################################################################################
						send( s , "X" , 1 , 0 );								// ONAY yolla
						arabellek[ recv( s , arabellek , 11  ,0 )]=NULL;		// Telefon numarasý
						arabellek[10]=NULL;
						for (i=0;i<toplamKisi;i++) if ( strcmp( kisi[i].telefon , arabellek ) == 0) break;
						if ( i == toplamKisi )
						{	// Aranýlan numara kayýtlý deðil.	Y: yok
							send( s , "Y" , 1 , 0 );
							SetConsoleTextAttribute(hConsole, kirmizi);
							printf("Kiþi Yok ");
							SetConsoleTextAttribute(hConsole, anarenk);
							printf("%s (%s), %s numaralý telefonu arattý.\n",k->isim,k->telefon,arabellek);
						} else
						{	// Aranýlan numara kayýtlý.			V: var
							send( s , "V" , 1 , 0 );
							recv( s , str , 5  ,0 );											// ONAY al
							send( s , kisi[i].isim , strlen( kisi[i].isim ) , 0 );
							SetConsoleTextAttribute(hConsole, kirmizi);
							printf("Kiþi Ekle ");
							SetConsoleTextAttribute(hConsole, anarenk);
							printf("%s (%s), %s (%s) kiþisini ekledi.\n",k->isim,k->telefon,kisi[i].isim,arabellek);
						}
					} else if ( arabellek[0] == 'G' )
					{	// GÝRÝÞ ÝÞLEMÝ YAP #######################################################################################
						kanal[bu].ku = 0;
						send( s , "X" , 1 , 0 );												// ONAY yolla
						y_kisi.kullaniciAdi[ recv( s , y_kisi.kullaniciAdi , 25  ,0 )]=NULL;	// ID al
						y_kisi.kullaniciAdi[24]=NULL;
						send( s , "X" , 1 , 0 );												// ONAY yolla
						y_kisi.sifre[ recv( s , y_kisi.sifre , 25  ,0 )]=NULL;					// PW al
						y_kisi.sifre[24]=NULL;
						for (i=0;i<toplamKisi;i++)
						{	// Girilen kullanýcý adý kayýtlý mý bul.
							if ( strcmp( y_kisi.kullaniciAdi , kisi[i].kullaniciAdi ) == 0 )
							{	// Kullanýcý adý eþleþti
								if ( strcmp( y_kisi.sifre , kisi[i].sifre ) == 0 )
								{	// Þifre de eþleþti.
									send( s , "D" , 1 , 0 );									// GÝRÝÞ ÝZNÝ yolla
									recv( s , str , 1  ,0 );									// ONAY al
									send( s , kisi[i].isim , strlen(kisi[i].isim) , 0 );		// AD yolla
									recv( s , str , 1  ,0 );									// ONAY al
									send( s , kisi[i].telefon , strlen(kisi[i].telefon) , 0 );	// TEL yolla
									kanal[bu].ku = &kisi[i];									
									SetConsoleTextAttribute(hConsole, kirmizi);
									printf( "Giriþ " );
									SetConsoleTextAttribute(hConsole, anarenk);
									printf("%s:%d %d - %s (%s) giriþ yaptý.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s,kisi[i].isim,kisi[i].telefon);
								} else
								{	// Þifre hatalý girildi.
									send( s , "Y" , 1 , 0 );									// HATA yolla
									SetConsoleTextAttribute(hConsole, kirmizi);
									printf( "Hatalý Giriþ " );
									SetConsoleTextAttribute(hConsole, anarenk);
									printf("%s:%d %d - %s (%s) þifre hatalý.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s,kisi[i].isim,kisi[i].telefon);
								}
								break;
							}
						}
						if ( i == toplamKisi ) 
						{	// Kullanýcý adý kayýtlý deðil.
							send( s , "I" , 1 , 0 ); 											// HATA yolla
							SetConsoleTextAttribute(hConsole, kirmizi);
							printf( "Hatalý Giriþ " );
							SetConsoleTextAttribute(hConsole, anarenk);
							printf("%s:%d %d - Kullanýcý adý yok.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port));
						}
					} else if ( arabellek[0] == 'K' )
					{	// KAYIT ÝÞLEMÝ YAP #######################################################################################
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
							printf("Hatalý Kayýt ");
							SetConsoleTextAttribute(hConsole, anarenk);
							printf("%s:%d %d Hatalý kayýt denemesi.\n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port) , s );
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
								printf("Hatalý Kayýt ");
								SetConsoleTextAttribute(hConsole, anarenk);
								printf("%s:%d %d - K. Adý (%s) ve tel. (%s) zaten kayýtlý.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s,k->kullaniciAdi,k->telefon);
							} else  if ( telK == 1 ) 
							{
								send( s , "T" , 1 , 0 );											// HATA yolla
								SetConsoleTextAttribute(hConsole, kirmizi);
								printf("Hatalý Kayýt ");
								SetConsoleTextAttribute(hConsole, anarenk);
								printf("%s:%d %d - Telefon (%s) zaten kayýtlý.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s,k->telefon);
							} else  if ( idK == 1 ) 
							{
								send( s , "I" , 1 , 0 );											// HATA yolla
								SetConsoleTextAttribute(hConsole, kirmizi);
								printf("Hatalý Kayýt ");
								SetConsoleTextAttribute(hConsole, anarenk);
								printf("%s:%d %d - K. Adý (%s) zaten kayýtlý.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s,k->kullaniciAdi);
							}else {
								strcpy( kisi[toplamKisi].isim , k->isim );
								strcpy( kisi[toplamKisi].telefon , k->telefon );
								strcpy( kisi[toplamKisi].kullaniciAdi , k->kullaniciAdi );
								strcpy( kisi[toplamKisi].sifre , k->sifre );
								kanal[bu].ku = &kisi[toplamKisi];
								SetConsoleTextAttribute(hConsole, kirmizi);
								printf( "Kayýt " );
								SetConsoleTextAttribute(hConsole, anarenk);
								printf (" %s (%s) - KA: %s Þf: %s\n" , k->isim , k->telefon , k->kullaniciAdi , k->sifre );
								SetConsoleTextAttribute(hConsole, kirmizi);
								printf( "Giriþ " );
								SetConsoleTextAttribute(hConsole, anarenk);
								printf("%s:%d %d - %s (%s) giriþ yaptý.\n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port) , s , k->isim , k->telefon );
								send( s , "D" , 1 , 0 );											// GÝRÝÞ ÝZNÝ yolla
								toplamKisi++;
							}
						}
					} else
					{	// Yanlýþ bir mesaj yollandý. Baðlantýyý kes. (disconnect)
						SetConsoleTextAttribute(hConsole, kirmizi);
						printf("Çýkýþ ");
						SetConsoleTextAttribute(hConsole, anarenk);
						if ( k == 0 ) printf ("%s:%d (%d)\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s);
						else printf("%s:%d %d - %s (%s) ayrýldý.\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port),s,k->isim,k->telefon);
						// Soketi tekrar kullanabilmek için kapatýlýp sýfýrlanýr.
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

    printf("OLAMAAAAZ PROGRAM ÇÖKTÜ :/\n");
    do {} while ( getch() != 13 );
    return 0;
}
