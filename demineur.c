#include "SDL.h"
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_ttf.h>

int transform(int pos_x, int pos_y, int* convert);
void init_grid(SDL_Rect* tab_rect);
void print_tab(SDL_Renderer* renderer, SDL_Rect* tab_rect, SDL_Color purple, SDL_Color dark_blue, SDL_Color dark, SDL_Surface* flag, SDL_Surface** number, SDL_Surface* meteorite);
void number_creator(SDL_Renderer* renderer, SDL_Rect* tab_rect);
int around(SDL_Rect* tab_rect, int x, int y);
int anticonvert(int x, int y);
int verif_input(int x, int y);
void bombarda(int* grilleTemp, SDL_Rect* tab_rect, int size);
void repulso(int** liste, int x, int y, int* size);
int the_end(SDL_Rect* tab_rect);
int* delete (int* tab, int size, int indice);
void revelio(SDL_Rect* tab_rect, int x, int y);
void boxText(SDL_Renderer* renderer, SDL_Color black, SDL_Color white, int msg_choice);


#define TOTAL_SIZE (SIZE*SIZE)
#define TOTAL_BOMB (int)(bomb_size) 
int SIZE = 10;
int bomb_size = 20;

typedef char bool;
#define true 1
#define true 0

int main(int argc, char* argv[])
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    int statut = EXIT_FAILURE;
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color orange = { 255, 127, 40, 255 };
    SDL_Color blue = { 51, 128, 255, 255 };
    SDL_Color dark_blue = { 38, 36, 193 , 255 };
    SDL_Color purple = { 95, 36, 193 ,255 };
    SDL_Color dark = { 17, 15, 77 ,255 };
    SDL_Color black = { 0, 0, 0 ,255 };

    SDL_Surface* image = IMG_Load("img/space.png");
    SDL_Surface* flag = IMG_Load("img/flag.png");
    SDL_Surface* meteorite = IMG_Load("img/meteorite.png");
    SDL_Surface* number[8] = {
        IMG_Load("img/1.png"),IMG_Load("img/2.png"),IMG_Load("img/3.png"),IMG_Load("img/4.png"),IMG_Load("img/5.png"),IMG_Load("img/6.png"),IMG_Load("img/7.png"),IMG_Load("img/8.png")
    };
    SDL_Surface* meteore[30] = {
        IMG_Load("img/meteore/pixil-frame-0.png"),IMG_Load("img/meteore/pixil-frame-1.png"),IMG_Load("img/meteore/pixil-frame-2.png"),IMG_Load("img/meteore/pixil-frame-3.png"),IMG_Load("img/meteore/pixil-frame-4.png"),IMG_Load("img/meteore/pixil-frame-5.png"),IMG_Load("img/meteore/pixil-frame-6.png"),IMG_Load("img/meteore/pixil-frame-7.png"),IMG_Load("img/meteore/pixil-frame-8.png"),IMG_Load("img/meteore/pixil-frame-9.png"),
        IMG_Load("img/meteore/pixil-frame-10.png"),IMG_Load("img/meteore/pixil-frame-11.png"),IMG_Load("img/meteore/pixil-frame-12.png"),IMG_Load("img/meteore/pixil-frame-13.png"),IMG_Load("img/meteore/pixil-frame-14.png"),IMG_Load("img/meteore/pixil-frame-15.png"),IMG_Load("img/meteore/pixil-frame-16.png"),IMG_Load("img/meteore/pixil-frame-17.png"),IMG_Load("img/meteore/pixil-frame-18.png"),IMG_Load("img/meteore/pixil-frame-19.png"),
        IMG_Load("img/meteore/pixil-frame-20.png"),IMG_Load("img/meteore/pixil-frame-21.png"),IMG_Load("img/meteore/pixil-frame-22.png"),IMG_Load("img/meteore/pixil-frame-23.png"),IMG_Load("img/meteore/pixil-frame-24.png"),IMG_Load("img/meteore/pixil-frame-25.png"),IMG_Load("img/meteore/pixil-frame-26.png"),IMG_Load("img/meteore/pixil-frame-27.png"),IMG_Load("img/meteore/pixil-frame-28.png"),IMG_Load("img/meteore/pixil-frame-29.png")
    };

    /*GESTION DES ERREURS DE SDL*/
    /* Initialisation, création de la fenêtre et du renderer. */
    if (0 != SDL_Init(SDL_INIT_EVERYTHING))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800, SDL_WINDOW_SHOWN);
    if (NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }
    SDL_SetWindowTitle(window, "Demineur");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }
    if (0 != SDL_SetRenderDrawColor(renderer, orange.r, orange.g, orange.b, orange.a))
    {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    }
    if (0 != SDL_RenderClear(renderer))
    {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    };
    if (!image)
    {
        printf("Erreur de chargement de l'image : %s", SDL_GetError());
        return -1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
    {
        printf("%s", Mix_GetError());
    }


    SDL_SetRenderDrawColor(renderer, blue.r, blue.g, blue.b, blue.a);

    SDL_Rect* tab_rect = (SDL_Rect*)malloc(sizeof(SDL_Rect) * TOTAL_SIZE);
    if (tab_rect == NULL)
        goto Quit;

    /*GAME LOOP*/
    SDL_Rect screen = { 0, 0, 1200, 800 };

    SDL_Texture* monImage = SDL_CreateTextureFromSurface(renderer, image);  //La texture monImage contient maintenant l'image importée
    SDL_RenderCopy(renderer, monImage, NULL, &screen);
    int game = 1;
    SDL_Event ev;
    int* liste = NULL;
    srand(time(NULL));
    int convert[2];
    init_grid(tab_rect);
    Mix_Music* musique;
    Mix_Music* victory;
    musique = Mix_LoadMUS("music/guardian.mp3");
    victory = Mix_LoadMUS("music/amongus.mp3");
    Mix_PlayMusic(musique, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
    SDL_RenderPresent(renderer);
    boxText(renderer, black, white, 1);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_KEYDOWN)
                quit = SDL_TRUE;
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, monImage, NULL, &screen);
    while (liste == NULL)
    {
        SDL_PollEvent(&ev);
        if (ev.type == SDL_QUIT)
            game = 0;
        else if (ev.type == SDL_MOUSEBUTTONUP)
        {
            if (ev.button.button == SDL_BUTTON_LEFT) {
                transform(ev.button.x, ev.button.y, convert);

                if (verif_input(convert[0], convert[1]) == 1) {
                    liste = malloc(sizeof(int) * TOTAL_SIZE);
                    if (liste == NULL)
                        return;
                    for (int i = 0; i < TOTAL_SIZE; i++) {
                        liste[i] = i;
                    }
                    int size_bomb = TOTAL_SIZE;
                    repulso(&liste, convert[0], convert[1], &size_bomb);
                    //Si je fais pas size --, le dernier element de la liste est out of renge
                    bombarda(liste, tab_rect, size_bomb);
                    number_creator(renderer, tab_rect);
                    revelio(tab_rect, convert[0], convert[1]);
                }
            }
            if (ev.button.button == SDL_BUTTON_RIGHT)
                transform(ev.button.x, ev.button.y, convert);
            if (verif_input(convert[0], convert[1]) == 1) {
                if (tab_rect[anticonvert(convert[0], convert[1])].flag == 0) {
                    tab_rect[anticonvert(convert[0], convert[1])].flag = 1;
                }
                else if (tab_rect[anticonvert(convert[0], convert[1])].flag == 1) {
                    tab_rect[anticonvert(convert[0], convert[1])].flag = 0;
                }
            }
        }
        print_tab(renderer, tab_rect, purple, dark_blue, dark, flag, number, meteorite);
        SDL_RenderPresent(renderer);
    }

    for (int i = 0; i < TOTAL_SIZE; i++) {
        tab_rect[i].flag = 0;
    }


    while (game == 1) {

        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&ev) != 0)
        {
            if (ev.type == SDL_QUIT)
                game = 0;
            else if (ev.type == SDL_MOUSEBUTTONUP)
            {
                if (ev.button.button == SDL_BUTTON_LEFT) {
                    transform(ev.button.x, ev.button.y, convert);
                    if (verif_input(convert[0], convert[1]) == 1) {
                        if (tab_rect[anticonvert(convert[0], convert[1])].bomb == 1)
                        {
                            for (int i = 0; i < TOTAL_SIZE; i++) {
                                if (tab_rect[i].bomb == 1) {
                                    tab_rect[i].hide = 0;
                                }
                            }
                            SDL_RenderPresent(renderer);
                            
                            printf("looser");
                            SDL_Rect looser = { 0, 0, 1200, 1200 };
                            for (int i = 0; i < 29; i++) {
                                SDL_RenderClear(renderer);
                                SDL_RenderCopy(renderer, monImage, NULL, &screen);
                                SDL_Texture* meteoregif = SDL_CreateTextureFromSurface(renderer, meteore[i]);  
                                SDL_RenderCopy(renderer, meteoregif, NULL, &looser);
                                SDL_RenderPresent(renderer);
                                SDL_Delay(50);
                            }

                            SDL_RenderClear(renderer);
                            SDL_RenderCopy(renderer, monImage, NULL, &screen);
                            print_tab(renderer, tab_rect, purple, dark_blue, dark, flag, number, meteorite);
                            SDL_RenderPresent(renderer);
                            SDL_Delay(4000);             
                            goto Quit;
                        }
                        revelio(tab_rect, convert[0], convert[1]);
                    }
                }
                if (ev.button.button == SDL_BUTTON_RIGHT)
                    transform(ev.button.x, ev.button.y, convert);
                
                if (tab_rect[anticonvert(convert[0], convert[1])].flag == 0) {
                    tab_rect[anticonvert(convert[0], convert[1])].flag = 1;
                }
                else if (tab_rect[anticonvert(convert[0], convert[1])].flag == 1) {
                    tab_rect[anticonvert(convert[0], convert[1])].flag = 0;
                }
            }
        }
        if (the_end(tab_rect) == 0) {
            Mix_PauseMusic(musique);
            Mix_PlayMusic(victory, -1);
            printf("gagné");
            SDL_Delay(3000);
            goto Quit;
        }
        print_tab(renderer, tab_rect, purple, dark_blue, dark, flag, number, meteorite);
    }
    Mix_FreeMusic(musique);
    Mix_FreeMusic(victory);
    Mix_CloseAudio();
    statut = EXIT_SUCCESS;

Quit:
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    if (NULL != image)
        SDL_DestroyTexture(image);
    if (NULL != flag)
        SDL_DestroyTexture(flag);
    for (int i = 0; i < 7; i++)
        if (NULL != number[i])
            SDL_DestroyTexture(number[i]);
    if (NULL != number)
        SDL_DestroyTexture(number);
    for (int i = 0; i < 29; i++)
        if (NULL != meteore[i])
            SDL_DestroyTexture(meteore[i]);
    if (NULL != meteore)
        SDL_DestroyTexture(meteore);

    SDL_Quit();
    TTF_Quit();
    return statut;
}

void init_grid(SDL_Rect* tab_rect) {
    for (size_t i = 0; i < TOTAL_SIZE; i++) {
        tab_rect[i].h = 50;
        tab_rect[i].w = 50;
        tab_rect[i].x = 350 + ((50 * (int)(i / SIZE)));
        tab_rect[i].y = 200 + (50 * (i % SIZE));
        tab_rect[i].value = 0;
        tab_rect[i].hide = 1;
        tab_rect[i].bomb = 0;
        tab_rect[i].flag = 0;
    }
}


void print_tab(SDL_Renderer* renderer, SDL_Rect* tab_rect, SDL_Color purple, SDL_Color dark_blue, SDL_Color dark, SDL_Surface* flag, SDL_Surface** number, SDL_Surface* meteorite) {
    for (size_t i = 0; i < TOTAL_SIZE; i++) {
        if (tab_rect[i].hide == 0) {
            SDL_SetRenderDrawColor(renderer, dark.r, dark.g, dark.b, dark.a);
            SDL_RenderFillRect(renderer, &tab_rect[i]);
            if (tab_rect[i].value != 0) {

                SDL_Rect rectNumb = { tab_rect[i].x,tab_rect[i].y, 50, 50 };
                SDL_Texture* numberTest = SDL_CreateTextureFromSurface(renderer, number[(tab_rect[i].value) - 1]);  //La texture monImage contient maintenant l'image importée
                SDL_RenderCopy(renderer, numberTest, NULL, &rectNumb);
            }
            if (tab_rect[i].bomb == 1) {
                SDL_Rect meteor = { tab_rect[i].x,tab_rect[i].y, 50, 50 };
                SDL_Texture* metorText = SDL_CreateTextureFromSurface(renderer, meteorite);
                SDL_RenderCopy(renderer, metorText, NULL, &meteor);
            }
        }
        else {
            SDL_SetRenderDrawColor(renderer, dark_blue.r, dark_blue.g, dark_blue.b, dark_blue.a);
            if ((i + (int)(i / SIZE)) % 2 == 0)//calcul qui sert a alterner les couleurs pour le cadrillage
                SDL_SetRenderDrawColor(renderer, purple.r, purple.g, purple.b, purple.a);
            SDL_RenderFillRect(renderer, &tab_rect[i]);
            if (tab_rect[i].flag == 1) {
                SDL_Rect rectImg = { tab_rect[i].x,tab_rect[i].y, 50, 50 };
                SDL_Texture* monImage1 = SDL_CreateTextureFromSurface(renderer, flag);  //La texture monImage contient maintenant l'image importée
                SDL_RenderCopy(renderer, monImage1, NULL, &rectImg);
            }
        }
    }
}

int transform(int pos_x, int pos_y, int* convert) {
    pos_x -= 350;
    pos_y -= 200;

    convert[0] = (int)(pos_x / 50);
    convert[1] = (int)(pos_y / 50);
}

void number_creator(SDL_Renderer* renderer, SDL_Rect* tab_rect) {
    // Création des nombres qui déterminent les bombes autour de la case ciblée
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            // Pour chaque élément du tableau cela va lancer la fonction around

            tab_rect[anticonvert(i, j)].value = around(tab_rect, i, j);
        }
    }
}

int around(SDL_Rect* tab_rect, int x, int y) {
    if (tab_rect[anticonvert(x, y)].bomb == 1) {
        // Si la case choisie est une bombe alors on la touche pas et on return
        return 0;
    };

    int count;
    count = 0;
    // On vérifie combien de bombe y-a-t-il autour de la case ciblée

    // partie gauche
    if (y != 0 && tab_rect[anticonvert(x - 1, y - 1)].bomb == 1) {
        count++;
    }
    if (y != 0 && tab_rect[anticonvert(x, y - 1)].bomb == 1) {
        count++;
    }
    if (y != 0 && tab_rect[anticonvert(x + 1, y - 1)].bomb == 1) {
        count++;
    }

    // milieu
    if (x != 0 && tab_rect[anticonvert(x - 1, y)].bomb == 1) {
        count++;
    }
    if (x != SIZE && tab_rect[anticonvert(x + 1, y)].bomb == 1) {
        count++;
    }

    // Droite
    if (y != SIZE - 1 && tab_rect[anticonvert(x - 1, y + 1)].bomb == 1) {
        count++;
    }
    if (y != SIZE - 1 && tab_rect[anticonvert(x, y + 1)].bomb == 1) {
        count++;
    }
    if (y != SIZE - 1 && tab_rect[anticonvert(x + 1, y + 1)].bomb == 1) {
        count++;
    }
    return count;
}

void revelio(SDL_Rect* tab_rect, int x, int y) {
    // En gros je regarde si la case à coté est vide ET non révélée sur la grille
    // du joueur (emplaçement non révélée = 10) Puis je fais un appel récursif
    // pour revele la case et y appliquer la fonction

    if (x < 0 || x >= SIZE)
        return;

    if (y < 0 || y >= SIZE)
        return;

    if (tab_rect[anticonvert(x, y)].hide == 0)
        return;

    tab_rect[anticonvert(x, y)].hide = 0;

    if (tab_rect[anticonvert(x, y)].value != 0)
        return;

    revelio(tab_rect, x - 1, y);
    revelio(tab_rect, x + 1, y);
    revelio(tab_rect, x, y - 1);
    revelio(tab_rect, x + 1, y - 1);
    revelio(tab_rect, x - 1, y - 1);
    revelio(tab_rect, x, y + 1);
    revelio(tab_rect, x - 1, y + 1);
    revelio(tab_rect, x + 1, y + 1);
}

int anticonvert(int x, int y) {
    // Convertit des coordonnées x et y en coordonnées unidimentionelles (de 0 à
    // 400)
    int coo = 0;
    coo = x * SIZE + y;
    return coo;
}

int verif_input(int x, int y) {
    // Fonction qui retourne 0 si la saisie est invalide et 1 si valide
    if (0 > x || x >= SIZE || 0 > y || y >= SIZE) {
        return 0;
    }

    return 1;
}

void repulso(int** liste, int x, int y, int* size) {
    //Fonction qui crée le petit ilot au début de la partie 
    int elem = anticonvert(x, y);
    int sous = 0;
    int taille = TOTAL_SIZE;

    if (SIZE < 5)
    {
        *liste = delete(*liste, taille, elem);
        *size = taille - 1;
        return;
    }

    for (int i = 1; i >= -1; i--) {
        for (int j = 1; j >= -1; j--) {
            if (verif_input(x + i, y + j) != 0)
            {
                *liste = delete(*liste, taille + sous, anticonvert(x + i, y + j));
                sous--;
            }
        }
    }
    *size = taille + sous;
}

int the_end(SDL_Rect* tab_rect) {
    //Fonction qui vérifie si le joueur a gagné
    for (int i = 0; i < TOTAL_SIZE; i++) {
        if (tab_rect[i].hide == 1 && tab_rect[i].bomb == 0) {
            return 1;
        }
    }
    return 0;
}

void bombarda(int* grilleTemp, SDL_Rect* tab_rect, int size) {

    int i = 0;
    while (i < TOTAL_BOMB) {

        int n = rand() % (size - i - 1);
        int indice = grilleTemp[n];
        tab_rect[indice].bomb = 1;

        grilleTemp = delete(grilleTemp, size - i, n);

        i++;
    }
    free(grilleTemp);
}

int* delete (int* tab, int size, int indice) {
    //fonction qui enleve un élément d'une liste, parametre( le tableau visé, la taille du tableau, l'indice de l'element qu'on veut virer)
    size--; //size -- car i vaut 0 dans la premiere boucle de bombarda

    int* newTab = (int*)malloc(sizeof(int) * size);
    for (int i = 0; i < indice; i++) {
        newTab[i] = tab[i];
    }
    for (int i = indice; i < size - 1; i++) {
        newTab[i] = tab[i + 1];
    }
    free(tab);
    return newTab;
}

void boxText(SDL_Renderer* renderer, SDL_Color black, SDL_Color white, int msg_choice) {
    TTF_Init();
    SDL_Color textColor = { 255, 255, 255 };
    TTF_Font* font = TTF_OpenFont("font/fontRegular.ttf", 24);

    SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
    SDL_Rect test = { 359,529,502,152 };
    SDL_RenderDrawRect(renderer, &test);

    SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
    SDL_Rect box = { 360,530,500,150 };
    SDL_RenderDrawRect(renderer, &box);
    SDL_RenderFillRect(renderer, &box);

    char* a;
    a = "rejouer?";
    if (msg_choice == 1)
        a = "Pressez_une_touche";
    char a_to_display[22];

    SDL_Rect texttt = { 360,530,20,75 };

    for (int j = 0; j < strlen(a); j++) {
        a_to_display[j] = a[j];
        a_to_display[j + 1] = '\0';
        SDL_Surface* message = TTF_RenderText_Solid(font, a_to_display, textColor);
        SDL_Texture* Text = SDL_CreateTextureFromSurface(renderer, message);
        SDL_RenderCopy(renderer, Text, NULL, &texttt);
        SDL_RenderPresent(renderer);
        texttt.w += 20;
        SDL_Delay(100);
        if (NULL != Text)
            SDL_DestroyTexture(Text);
    }

    if (NULL != font)
        TTF_CloseFont(font);
}
