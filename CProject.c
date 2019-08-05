#include <stdio.h>
#include <stdlib.h>
// MUHAMMED MUSTAFA ERMEHAN 330083


typedef struct Takim Takim;   
typedef struct Lig Lig;
int macEkle(char *mac);

struct Takim
{
    char isim[15];
    char etiket;
    int skor;
    int golAtilan, golYenilen;
    int galibiyet, maglubiyet, beraberlik;
    char *oynananMaclar; // BCDFS
};

struct Lig
{
    Takim *takimlar;
    int takimSayisi;
    int puanGalibiyet;
    int puanBeraberlik;
    int puanMaglubiyet;

}lig;

void iceAktar(char secim) 
{
    FILE * dosya;
    // Ayarlari ice aktar
    if(secim == 'a')
    {
        dosya = fopen("ayarlar.txt", "r");
        fscanf(dosya, "%d", &lig.takimSayisi);
        fscanf(dosya, "%d", &lig.puanGalibiyet);
        fscanf(dosya, "%d", &lig.puanBeraberlik);
        fscanf(dosya, "%d", &lig.puanMaglubiyet);
    }
    // Takimlari ice aktar
    else if(secim == 't')
    {
        dosya = fopen("takimlar.txt", "r");
        int i=0;
		for(i=0; i<lig.takimSayisi; i++)
        {
            fscanf(dosya, "%c", &lig.takimlar[i].etiket);
            fseek(dosya, 1, SEEK_CUR);
            fscanf(dosya, "%s", &lig.takimlar[i].isim);            
            fseek(dosya, 1, SEEK_CUR);
        }

    } 
    // Maclari ice aktar
    else if(secim == 'm')
    {
        dosya = fopen("maclar.txt", "r");
        while(1)
        {
            char *mac = calloc(4, sizeof(char));
            fscanf(dosya, "%c", &mac[0]);
            fseek(dosya, 1, SEEK_CUR);
            fscanf(dosya, "%c", &mac[1]);
            fseek(dosya, 1, SEEK_CUR);
            fscanf(dosya, "%c", &mac[2]);
            fseek(dosya, 1, SEEK_CUR);
            fscanf(dosya, "%c", &mac[3]);
            // mac = {A3B0}
            if(feof(dosya))
                break;
            
            fseek(dosya, 1, SEEK_CUR);

            if( macEkle(mac) == 0 )
                printf("Mac eklenemedi! %s \n", mac);
        }

    }

    fclose(dosya);
}

int macEkle(char *mac)   
{
    char e = mac[0], m = mac[2];         // e: evsahibi, m: misafir
    int ep = mac[1]-48, mp = mac[3]-48;  // ep: evsahibi puani, mp: misafir puani
    
    Takim* evSahibi = NULL;
    Takim* misafir = NULL;
    
    // Ev sahibi ve misafir takim bulunur        
    int i=0;
    for( i=0; i<lig.takimSayisi; i++)
    {
        if(lig.takimlar[i].etiket == e){
            evSahibi = &lig.takimlar[i];
        }else if(lig.takimlar[i].etiket == m){
            misafir = &lig.takimlar[i];
        }
    }

    // Mac daha once oynanmis mi kontrolu yapilir
    int j=0;
    while(evSahibi->oynananMaclar[j] != ' ')
    {
        if( evSahibi->oynananMaclar[j] == misafir->etiket ){
            return 0;
        } 
        j++;
    } 

    // Mac gecerli ise;
    evSahibi->oynananMaclar[j] = misafir->etiket;
    evSahibi->golAtilan += ep;
    evSahibi->golYenilen += mp;
    misafir->golAtilan += mp;
    misafir->golYenilen += ep;

    if( ep == mp )
    {
        evSahibi->beraberlik++;
        misafir->beraberlik++;
    }
    else if(ep > mp)
    {
        evSahibi->galibiyet++;
        misafir->maglubiyet++;
    }
    else if(ep < mp)
    {
        evSahibi->maglubiyet++;
        misafir->galibiyet++;
    }

    evSahibi->skor = (evSahibi->galibiyet * lig.puanGalibiyet) + (evSahibi->beraberlik * lig.puanBeraberlik) + (evSahibi->maglubiyet * lig.puanMaglubiyet);
    misafir->skor = (misafir->galibiyet * lig.puanGalibiyet) + (misafir->beraberlik * lig.puanBeraberlik) + (misafir->maglubiyet * lig.puanMaglubiyet);

    return 1;
}

void ilkDegerAta() 
{
    // takim boyutu belirlenir
    lig.takimlar = calloc(lig.takimSayisi, sizeof(Takim));
    
    // takimlarin oynanan maclar dizisi belirlenir
    int i=0;
	for( i=0; i<lig.takimSayisi; i++)
    {
        lig.takimlar[i].oynananMaclar = calloc(lig.takimSayisi, sizeof(char));
		int j=0;
        for( j=0; j<lig.takimSayisi; j++)
            lig.takimlar[i].oynananMaclar[j] = ' ';
        

        lig.takimlar[i].etiket = ' ';
        
        lig.takimlar[i].galibiyet = 0;
        lig.takimlar[i].beraberlik = 0;
        lig.takimlar[i].maglubiyet = 0;
        
        lig.takimlar[i].golAtilan = 0;
        lig.takimlar[i].golYenilen = 0;
        
        lig.takimlar[i].skor = 0;
    }

}

void bastirPuanTablosu() 
{
    int i=0;
	for( i=0; i<lig.takimSayisi; i++){
        printf("%d. ", i+1);
        printf("%c ", lig.takimlar[i].etiket);
        printf("%s ", lig.takimlar[i].isim);
        printf("Gal:%d ", lig.takimlar[i].galibiyet);
        printf("Ber:%d ", lig.takimlar[i].beraberlik);
        printf("Mgl:%d ", lig.takimlar[i].maglubiyet);
        printf("Gol:%d-", lig.takimlar[i].golAtilan);
        printf("%d ", lig.takimlar[i].golYenilen);
        printf("Av:%d ", lig.takimlar[i].golAtilan - lig.takimlar[i].golYenilen );
        printf("Skr:%d \n", lig.takimlar[i].skor);
    }
}

void klavyedenGir() 
{
    char girdi[20];
    printf("Maci girin (A 3 B 2): ");
    scanf("%[^\n]s", girdi);
    char mac[4] = {girdi[0], girdi[2], girdi[4], girdi[6]};
    if( macEkle(mac) == 0) 
        printf("Mac eklenemedi! %s\n", mac);
}

void yerDegistir(Takim* takim1, Takim* takim2) 
{
    Takim gecici;
    gecici.oynananMaclar = calloc( lig.takimSayisi, sizeof(char));
    
    
    
    int i=0;
	for( i=0; takim1->isim[i] != '\0' || i<15; i++)
        gecici.isim[i] = takim1->isim[i];
    
	for( i=0; i<lig.takimSayisi; i++)
        gecici.oynananMaclar[i] = takim1->oynananMaclar[i];
    gecici.etiket = takim1->etiket;
    gecici.galibiyet = takim1->galibiyet;
    gecici.beraberlik = takim1->beraberlik;
    gecici.maglubiyet = takim1->maglubiyet;
    gecici.skor = takim1->skor;
    gecici.golAtilan = takim1->golAtilan;
    gecici.golYenilen = takim1->golYenilen;


	
    for( i=0; takim2->isim[i] != '\0' || i<15; i++)
        takim1->isim[i] = takim2->isim[i];
    for( i=0; i<lig.takimSayisi; i++)
        takim1->oynananMaclar[i] = takim2->oynananMaclar[i];
    takim1->etiket = takim2->etiket;
    takim1->galibiyet = takim2->galibiyet;
    takim1->beraberlik = takim2->beraberlik;
    takim1->maglubiyet = takim2->maglubiyet;
    takim1->skor = takim2->skor;
    takim1->golAtilan = takim2->golAtilan;
    takim1->golYenilen = takim2->golYenilen;



    for( i=0; gecici.isim[i] != '\0' || i<15; i++)
        takim2->isim[i] = gecici.isim[i];
    for( i=0; i<lig.takimSayisi; i++)
        takim2->oynananMaclar[i] = gecici.oynananMaclar[i];
    takim2->etiket = gecici.etiket;
    takim2->galibiyet = gecici.galibiyet;
    takim2->beraberlik = gecici.beraberlik;
    takim2->maglubiyet = gecici.maglubiyet;
    takim2->skor = gecici.skor;
    takim2->golAtilan = gecici.golAtilan;
    takim2->golYenilen = gecici.golYenilen;
}

int isimKarsilastir(char *isim1, char *isim2) 
{
    int i=0;
	for( i=0; isim1[i] != '\0' || isim2[i] != '\0'; i++){
        if( isim1[i] > isim2[i] )
            return 1;
        else if( isim2[i] > isim1[i] )
            return 0;
        if( isim1[i] == '\0' )
            return 1;
        if( isim2[i] == '\0' )
            return 0;
    }
}

void sirala(char secim) 
{
    if(secim == 's')    // Skora gore sirala
    {
        int i=0;
		for( i=0; i<lig.takimSayisi; i++)
        {
        	int j=0;
            for( j=i; j<lig.takimSayisi; j++)
            {
                if( lig.takimlar[j].skor > lig.takimlar[i].skor )
                {   
                    yerDegistir(&lig.takimlar[i], &lig.takimlar[j]);
                }
            }
        }
    } 
    else if(secim == 'i') // isme gore sirala
    {
        int i=0;
		for( i=0; i<lig.takimSayisi; i++)
        {
        	int j=0;
            for( j=0; j<lig.takimSayisi; j++)
            {
                if( isimKarsilastir(lig.takimlar[j].isim, lig.takimlar[i].isim) == 1)
                {   
                    yerDegistir(&lig.takimlar[i], &lig.takimlar[j]);
                }

            }
        }
    } 
    else if(secim == 'e') // Etikete gore sirala
    {
        int i=0;
		for( i=0; i<lig.takimSayisi; i++)
        {
        	int j=0;
            for( j=i; j<lig.takimSayisi; j++)
            {
                if( lig.takimlar[j].etiket < lig.takimlar[i].etiket )
                {   
                    yerDegistir(&lig.takimlar[i], &lig.takimlar[j]);
                }
            }
        }
    }

}

void harfleriBuyut() 
{
    int i=0;
	for( i=0; i<lig.takimSayisi; i++)
    {
    	int j=0;
        for( j=0; lig.takimlar[i].isim[j] != '\0' && j<15; j++)
        {
            if(lig.takimlar[i].isim[j] >= 97 && lig.takimlar[i].isim[j] <= 122 )
                lig.takimlar[i].isim[j] -= 32;
        }
    }
}

void dosyayaKaydet() 
{
    FILE *dosya;
    dosya = fopen("fikstür.txt", "w");
    int i=0;
	for( i=0; i<lig.takimSayisi; i++)
    {
        fprintf(dosya, "%d. ", i+1);
        fprintf(dosya, "%c ", lig.takimlar[i].etiket);
        fprintf(dosya, "%s ", lig.takimlar[i].isim);
        fprintf(dosya, "%d ", lig.takimlar[i].galibiyet);
        fprintf(dosya, "%d ", lig.takimlar[i].beraberlik);
        fprintf(dosya, "%d ", lig.takimlar[i].maglubiyet);
        fprintf(dosya, "%d-", lig.takimlar[i].golAtilan);
        fprintf(dosya, "%d ", lig.takimlar[i].golYenilen);
        fprintf(dosya, "%d ", lig.takimlar[i].golAtilan - lig.takimlar[i].golYenilen );
        fprintf(dosya, "%d \n", lig.takimlar[i].skor);
    }
    fclose(dosya);
}


int main()
{
    iceAktar('a'); // ayarlar ice aktarildi
    ilkDegerAta(); // lig'in ilk deger atamasi
    iceAktar('t'); // takimlar ice aktarildi
    iceAktar('m'); // maclar ice aktarildi


    //klavyedenGir(); // maci klavyeden girmeyi saglayan fonksiyon
    bastirPuanTablosu(); // puan tablosunu bastirir

    harfleriBuyut(); // kucuk harfleri buyuk harfe donusturur

    sirala('s'); // takim skoruna gore siralar
    sirala('i'); // takim ismine gore siralar
    sirala('e'); // takim etiketine gore siralar
    bastirPuanTablosu();  

    dosyayaKaydet(); // fiksturu dosyaya kaydeder 
    return 0;
}
