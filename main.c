#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>

#define GENISLIK 500
#define YUKSEKLIK 480
#define MAX 20		//labirent boyut siniri
#define BEKLE 500	//adimlar arasindaki bekleme suresi(milisaniye)

struct yer {
	short x, y;
	struct yer* geri;
};

typedef struct yer yer;

void olustur(int boyut, short** labirent)
{
	int i,j;
	for(i=0; i<boyut; i++) {
		for(j=0; j<boyut; j++) {
			labirent[i][j] = rand()%2;
		}
	}
}

int yiginKontrol(yer* son, int x, int y) {
	yer* tm;
	for(tm = son; tm->x!=-1; tm = tm->geri) {
		if(tm->x == x && tm->y == y)
			return 0;
	}

	return 1;
}

yer* yolBul(int boyut, short** labirent, SDL_Window *window) {
    printf("11111111111111111");

	int x,y,x1,y1;// x,y giris x1,y1 cikis noktalarý

	//kullaniciya programi anlatma
	printf("Sirayla giris ve cikis noktalarini belirleyeceksiniz.\nEger bu noktalar matrisin kenarinda olmazsa ya da girilen nokta 1 deðilse koordinatlari yeniden girmeniz istenecek\nEger giridiniz sayi 0 dan kucuk ya da matris boyutundan buyuk olursa program noktayi kendisi atayacaktir\n");
	do {
		printf("Baslamak icin bir kenar secin.\n");
		printf("X[0-%d] araligi-Varsayilan: 0-: ", boyut-1);
		scanf("%d", &x);
		if(x<0 || x>=boyut)
			x = 0;

		printf("Y[0-%d] araligi-Varsayilan: 0-: ", boyut-1);
		scanf("%d", &y);
		if(y<0 || y>=boyut)
			y = 0;
	}while((x!=0 && x!=boyut-1) && (y!=0 && y!=boyut-1) || labirent[y][x] != 1);
	do {
		printf("Cikis icin bir kenar secin.\n");
		printf("X[0-%d] araligi-Varsayilan: %d-: ", boyut-1, boyut-1);
		scanf("%d", &x1);
		if(x1<0 || x1>=boyut)
			x1 = boyut-1;

		printf("Y[0-%d] araligi-Varsayilan: %d-: ", boyut-1, boyut-1);
		scanf("%d", &y1);
		if(y1<0 || y1>=boyut)
			y1 = boyut-1;
	}while((x1!=0 && x1!=boyut-1) && (y1!=0 && y1!=boyut-1) || labirent[y1][x1] != 1);

	printf("Girilen koordinatlar: %d,%d\n", x,y);
	printf("Cikis koordinatlari : %d,%d\n", x1,y1);

	yer* bas = (yer*) malloc(sizeof(yer));
	yer* son = (yer*) malloc(sizeof(yer));
	yer* s;

	bas->x = -1;
	son->x = x;
	son->y = y;
	son->geri = bas;


	while(x!=x1 || y!=y1) {
		yoluCiz(boyut, labirent, window, x, y);
		SDL_Delay(BEKLE);
		//bir yol sec(yukarý asagi sol sag) (x-1 x+1 y-1 y+1)
			//yol kenarý asmamalý ve gittigi yerde 1 olmalý ve bir onceki gidilmis yer olmamali
				//gidersen baglý listenin sonuna ekle
				//gidemezsen geri don ve baglý listenin son elemanýný sil
					//eger bas pointerina donuluyorsa null return et

		//eski versiyonda: && (son->geri->x != x+1 || son->geri->y != y)

		if(x+1<boyut && labirent[y][x+1] == 1 && yiginKontrol(son, x+1, y))  {
			x++;
			printf("x+1: x:%dy:%d\n", x, y);
			s = (yer*) malloc(sizeof(yer));
			s->geri = son;
			s->x = x;
			s->y = y;
			son = s;
		}
		else if(y+1<boyut && labirent[y+1][x] == 1 && yiginKontrol(son, x, y+1)) {
			y++;
			printf("y+1: x:%dy:%d\n", x, y);
			s = (yer*) malloc(sizeof(yer));
			s->geri = son;
			s->x = x;
			s->y = y;
			son = s;
		}
		else if(x-1>=0 && labirent[y][x-1] == 1 && yiginKontrol(son, x-1, y)) {
			x--;
			printf("x-1: x:%dy:%d\n", x, y);
			s = (yer*) malloc(sizeof(yer));
			s->geri = son;
			s->x = x;
			s->y = y;
			son = s;
		}
		else if(y-1>=0 && labirent[y-1][x] == 1 && yiginKontrol(son, x, y-1)) {
			y--;
			printf("y-1: x:%dy:%d\n", x, y);
			s = (yer*) malloc(sizeof(yer));
			s->geri = son;
			s->x = x;
			s->y = y;
			son = s;
		}
		else {
			printf("Geri donuluyor: x:%d, y:%d\n", x, y);

			//eger basa donuyorsa cýkýs yoktur
			if(son->geri->x == -1) return NULL;


			//geri don
			labirent[y][x] = 0;
			x = son->geri->x;
			y = son->geri->y;

			yer *temp = son;
			son = son->geri;
			free(temp);


		}


	}
	SDL_Delay(BEKLE);
    yoluCiz(boyut, labirent, window, x, y);


	return son;
}

void yazdir(int boyut, short** labirent) {
	int i,j;
	for(i=0; i<boyut; i++) {
		for(j=0; j<boyut; j++) {
			printf("%d ", labirent[i][j]);
		}
		printf("\n");
	}
}

void yoluCikar(int boyut, short** labirent, yer* son)
{
	int i,j;
	for(i=0; i<boyut; i++) {
		for(j=0; j<boyut; j++) {
			labirent[i][j] = 0;
		}
	}

	yer* s;
	for(s=son; s->x != -1; s=s->geri) {
		labirent[s->y][s->x] = 1;
	}
}

void ciz(int boyut, short** labirent, SDL_Window *window)
{
	SDL_Surface *kare= SDL_GetWindowSurface(window);
	int i,j;
	int rect[4] = {15, 15, 0, 0};
	int genislik = 300/boyut;
	rect[2] = genislik;
	rect[3] = genislik;

	for(i=0; i<boyut; i++) {
		for(j=0; j<boyut; j++) {
			if(labirent[i][j] == 0)
				SDL_FillRect(kare, rect, SDL_MapRGB(kare->format, 0xFF, 0x00, 0x00));
			else
				SDL_FillRect(kare, rect, SDL_MapRGB(kare->format, 0x00, 0xFF, 0x00));
			rect[0]+=genislik+3;
		}
		rect[0] =15;
		rect[1]+=genislik+3;
	}

	SDL_UpdateWindowSurface(window);
	SDL_FreeSurface(kare);
}

void yoluCiz(int boyut, short** labirent, SDL_Window *window, int x, int y)
{
	SDL_Surface *kare= SDL_GetWindowSurface(window);
	int i,j;
	int rect[4] = {15, 15, 0, 0};
	int genislik = 300/boyut;
	rect[2] = genislik;
	rect[3] = genislik;

	for(i=0; i<boyut; i++) {
		for(j=0; j<boyut; j++) {
			if(labirent[i][j] == 0)
				SDL_FillRect(kare, rect, SDL_MapRGB(kare->format, 0xFF, 0x00, 0x00));
			/*else
				SDL_FillRect(kare, rect, SDL_MapRGB(kare->format, 0x00, 0xFF, 0x00));
			*/
			if(i==y && j==x)
				SDL_FillRect(kare, rect, SDL_MapRGB(kare->format, 0xFF, 0xFF, 0x00));

			rect[0]+=genislik+3;
		}
		rect[0] =15;
		rect[1]+=genislik+3;
	}

	SDL_UpdateWindowSurface(window);

}

void sonucCiz(int boyut, short** labirent, SDL_Window *window, yer *son)
{
	int i,j;
	int genislik = 300/boyut;
	int rect[4] = {15, 15, genislik, genislik};

	SDL_Surface *kare= SDL_GetWindowSurface(window);

	for(i=0; i<boyut; i++) {
		for(j=0; j<boyut; j++) {
				SDL_FillRect(kare, rect, SDL_MapRGB(kare->format, 0xFF, 0x00, 0x00));

			rect[0]+=genislik+3;
		}
		rect[0] =15;
		rect[1]+=genislik+3;
	}

	int temp_x = rect[0];
	yer *s = son;
	for(i=0; i<boyut; i++) {
		for(j=0; j<boyut; j++) {
			if( j==s->x && i==s->y) {
				SDL_FillRect(kare, rect, SDL_MapRGB(kare->format, 0x00, 0xFF, 0x00));
				s = s->geri;
			}
			rect[0]-=genislik+3;
		}
		rect[0] =temp_x;
		rect[1]-=genislik+3;
	}


	SDL_UpdateWindowSurface(window);
	SDL_FreeSurface(kare);
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("Labiret Oyunu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GENISLIK, YUKSEKLIK, SDL_WINDOW_SHOWN);
	SDL_Surface *background = SDL_GetWindowSurface(window);
	SDL_FillRect(background, NULL, SDL_MapRGB(background->format, 0xFF, 0xFF, 0xFF));
	SDL_UpdateWindowSurface(window);

	SDL_Event eventS;
	short isRunnin = 1;

	while(isRunnin) {
		int boyut;
		while(SDL_PollEvent(&eventS)) {
			if(eventS.type == SDL_QUIT) {
				isRunnin = 0;
				break;
			}

			do {
				printf("Matrisin boyutunu giriniz(Max: %d): ", MAX);
				scanf("%d", &boyut);
			}while(boyut>MAX);

			SDL_FillRect(background, NULL, SDL_MapRGB(background->format, 0xFF, 0xFF, 0xFF));
			SDL_UpdateWindowSurface(window);

			int i;
			short** labirent;
			labirent = (short **) malloc(boyut*sizeof(short));
			for(i=0;i<boyut;i++) {
				labirent[i] = (short *) malloc(boyut*sizeof(short));
			}

			srand(time(NULL));
			olustur(boyut, labirent);

			printf("Labirent\n");
			yazdir(boyut, labirent);
			ciz(boyut, labirent, window);

			yer* son = yolBul(boyut, labirent, window);
			if(son == NULL) {
				printf("Cikis yok\n");
			}
			else {
				printf("Ulasilan yol\n");
				yoluCikar(boyut, labirent, son);
				yazdir(boyut, labirent);
				sonucCiz(boyut, labirent, window, son);
			}
			SDL_Delay(5000);
		}
	}

	SDL_FreeSurface(background);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
