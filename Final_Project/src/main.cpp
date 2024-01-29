#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
using namespace std;

const int ScreenW = 740;    //�C���������e��
const int ScreenH = 780;    //�C������������
const int BlockW = 36;      //������e��
const int BlockH = 36;      //���������
const int Lines = 20;       //��20�Ӿ�C
const int Cols = 10;        //��10�Ӫ���
int gamevel = 400;          //�C������l�t��


TTF_Font *gFont = NULL;

Mix_Music *gMusic = NULL;

SDL_Window* window = NULL;

SDL_Renderer* render = NULL;    //��V��

SDL_Texture* gameover = NULL;   //�C������������
SDL_Texture* scoreboard = NULL; //�O���O������
SDL_Texture* gamestart = NULL;  //�C����l����
SDL_Texture* start = NULL;      //��l�e����START���s

string inputText = "";  //�x�s���a��J�W�٪��ܼ�

string name[5];     //�p���O�W���W�r
int scores[5];      //�p���O�W�W�r�ҹ����쪺����

//Texture wrapper class
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

#if defined(SDL_TTF_MAJOR_VERSION)
	//Creates image from font string
	bool loadFromRenderedText( std::string textureText, SDL_Color textColor , SDL_Renderer* gRenderer);
#endif

	//Deallocates texture
	void free();

	//Renders texture at given point
	void render( int x, int y, SDL_Renderer* gRenderer, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

class Tetris
{
public:
	void setCurrentTime(Uint32 t)
	{
		currentTime = t;
	}
	bool isrunning()
	{
		return running;
	}
	bool isvalid();                 //�P�_���a���ʤ���O�_�X�z
	int loadLeaderboard();          //�N�e���W���a�W�r�Τ���Ū�i��
	void save_leaderboard(int);     //��s�e���W���a�W�r�Τ��ƨüg�^rank.txt
	bool init(const char* title);   //��l�ƦU��SDL�M��A�H�Υ[���U�عϤ�
	bool loadMedia();               //�[����r�B����
	void nextTetrimino();           //���ͤU�@�Ӥ��
	void swapnextTetrimino();       //�Phold������洫�A�í��s���ͤ��
	void handleEvents();            //�B�z�U����L�ƥ�
	void setRectPos(SDL_Rect& rect, int x = 0, int y = 0,int w = BlockW, int h = BlockH);
	void moveRectPos(SDL_Rect& rect, int x, int y);
	void gameplay();                //�B�z������ʡB����B�çP�_�O�_����������rows
	void updateRender();            //��s�ù��e��
	void clean();

private:
	SDL_Texture* background = NULL, * blocks = NULL;
	SDL_Rect srcR = { 0, 0, BlockW, BlockH }, destR =
        { 0, 0, BlockW, BlockH }, holdR = { 0, 0, BlockW, BlockH }, tempR = { 0, 0, BlockW, BlockH };
    SDL_Rect nextR = { 0, 0, BlockW, BlockH };
	//Rendered texture
    LTexture leveltext;             //�Ψ���ܹC������
    LTexture scoretext;             //�Ψ���ܪ��a����
    LTexture nexttext;              //�Ψ���ܤU�@�Ӥ��
    LTexture holdtext;              //�Ψ����hold�����

	int field[Lines][Cols] = {0};   //�C�����
	static const int figures[7][4]; //�`�@7�ӹϧΤ��
	int temp_figure[4];             //��e���ϧ�
	int next_figure[4];             //�U�@�ӹϧ�
	struct Point                    //���4���I�y�Ъ���m
	{
		int x, y;
	} items[4], backup[4];
	int color = 1 + rand() % 7;     //�H�����ͤ@���C��
    int kind = rand() % 7;          //�H�����ͤ@�Ӻ���
	int next_kind;                  //�U�@�Ӥ��������
	int next_color;                 //�U�@�Ӥ�����C��
	int dx = 0;                     //���k���@�p�q�첾
	int score = -100;               //����
	int level = 1;                  //�C������
	int time = 1;                   //�b�@�^�X�����������
	int temp_kind;                  //�{�b���������
	int temp_color;                 //�{�b������C��
	int delay = gamevel;            //�C���t��
	bool have_rows = false;         //�P�_�O�_���@��C������
	bool rotate = false;            //�O�_�������
	bool swap = false;              //�O�_�w�g����洫
	bool can_swap = true;           //�O�_�i�H�Phold�洫
	bool running = false;           //�C���O�_����
	bool first = true;              //�O�_�Ĥ@�����ͤ��
	int startTime = 0, currentTime = 0;
};


//Scene textures
LTexture gPromptTextTexture1;   //���ܷ��a�C�������ɪ��W��
LTexture gPromptTextTexture2;   //���ܷ��a�C�������ɪ��ݭn��J�W�r�]Please Enter Your Name)
LTexture gInputTextTexture;     //���a��J�W�r���T��
LTexture playername[5];         //�x�s�e���W���a���W�r
LTexture playerscore[5];        //�x�s�e���W���a������

//�^�Ǯ�����Ʃұo�쪺����
int countscore(int time){
    //time��ܦb�@�^�X�����X��
    if (time == 4){
        return 800;
    }

    if (time == 3){
        return 400;
    }

    if (time == 2){
        return 200;
    }

    if (time == 1){
        return 100;
    }
    return 0;
}

//�ΨӳB�z���a��J�W�r
void handleuserinput();

void handleScoreBoard();

//�B�z�C���}�l����
void handlegamestart();

int main(int argc, char* argv[])
{
	//Start up SDL and create window
	srand(time(0));
	Tetris* tetris = new Tetris();
	const char* title = "Tetris";

	if(tetris->init(title) != true)
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !tetris->loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
		    //render�C���}�l�e��
		    tetris -> loadLeaderboard();
		    for (int i=0 ; i<5 ; ++i){
                SDL_Color textColor = { 0, 0, 0, 0xFF };
                playername[i].loadFromRenderedText( name[i], textColor, render );
                playerscore[i].loadFromRenderedText( to_string(scores[i]), textColor, render );
            }
		    handlegamestart();

		    //Play the music
            Mix_PlayMusic( gMusic, -1 );

            //�}�l�C��
		    while (tetris -> isrunning())
            {
                tetris->setCurrentTime(SDL_GetTicks());
                tetris->handleEvents();
                tetris->gameplay();
				tetris->updateRender();
            }

            //�����C���Arender�C�������e��
            if (tetris -> isrunning() == false){
                Mix_HaltMusic();
                SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear( render );
                SDL_RenderCopy(render, gameover, NULL, NULL);
                SDL_RenderPresent(render);
            }

            //�����a��J�W�r
            SDL_Delay(3000);
            int rank = tetris -> loadLeaderboard();
            if (rank != -1){
                SDL_Color textColor = { 0, 0, 0, 0xFF };
                string s = "You are the " + to_string(rank+1) + " place";
                gPromptTextTexture1.loadFromRenderedText( s, textColor, render );
                gPromptTextTexture2.loadFromRenderedText( "Please enter your name:", textColor, render );
                handleuserinput();
                tetris -> save_leaderboard(rank);
            }

            for (int i=0 ; i<5 ; ++i){
                SDL_Color textColor = { 0, 0, 0, 0xFF };
                playername[i].loadFromRenderedText( name[i], textColor, render );
                playerscore[i].loadFromRenderedText( to_string(scores[i]), textColor, render );
            }

            //render�O���O
            handleScoreBoard();
		}
	}

    tetris->clean();

	return 0;
}


/*
	0	1	2	3
	4	5	6	7
*/
const int Tetris::figures[7][4] =
{
	0,1,2,3,	// I
	0,4,5,6,	// J
	2,6,5,4,	// L
	1,2,5,6,	// O
	2,1,5,4,	// S
	1,4,5,6,	// T
	0,1,5,6,	// Z
};


bool Tetris::init(const char* title)
{
	bool success = true;
	if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		window = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenW, ScreenH, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			render = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( render == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( render, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				 //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}

				 //Initialize SDL_mixer
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}

				//�[���U�عϤ�
                SDL_Surface* loadSurf = IMG_Load("load/background.png");
				background = SDL_CreateTextureFromSurface(render, loadSurf);
				SDL_FreeSurface(loadSurf);
				loadSurf = IMG_Load("load/blocks.png");
				blocks = SDL_CreateTextureFromSurface(render, loadSurf);
				SDL_FreeSurface(loadSurf);
                loadSurf = IMG_Load("load/gameover.png");
                gameover = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);
				loadSurf = IMG_Load("load/scoreboard.png");
                scoreboard = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);
                loadSurf = IMG_Load("load/gamestart.png");
                gamestart = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);
                loadSurf = IMG_Load("load/start.png");
                start = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);
				nextTetrimino();
			}
		}
	}

	running = true;
	return success;
}

bool Tetris::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont( "load/font.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 255, 255, 255 };
		if( !nexttext.loadFromRenderedText( "Next", textColor, render ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}

		if( !holdtext.loadFromRenderedText( "hold", textColor, render ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}

		//Load music
        gMusic = Mix_LoadMUS( "load/music.mp3" );
        if( gMusic == NULL )
        {
            printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }
	}

	return success;
}

void Tetris::nextTetrimino()
{
    can_swap = true;
    if (first != true){
        color = next_color;
        kind = next_kind;
    }
    first = false;
	for (int i = 0; i < 4; i++)
	{
		items[i].x = figures[kind][i] % 4;
		items[i].y = int(figures[kind][i] / 4);
	}
	next_kind = rand() % 7;
	next_color =  1 + rand() % 7;
	for (int i=0 ; i<4 ; ++i){
        next_figure[i] = figures[next_kind][i];
	}
}

void Tetris::swapnextTetrimino()
{
	for (int i = 0; i < 4; i++)
	{
		items[i].x = figures[temp_kind][i] % 4;
		items[i].y = int(figures[temp_kind][i] / 4);
	}
}

void Tetris::handleEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
			case SDLK_UP:           //���V���J�u�W�v�A�������C
				rotate = true;
				break;
			case SDLK_LEFT:         //���V���J�u���v�A��������C
				dx = -1;
				break;
			case SDLK_RIGHT:        //���V���J�u�k�v�A�k������C
				dx = 1;
				break;
            case SDLK_z:            //����L��J�u�C�v�A����hold�C
                if (can_swap == true){
                    tempR = srcR;
                    can_swap = false;
                    if (swap == false){
                        swap = true;
                        for (int i=0 ; i<4 ; ++i){
                            temp_figure[i] = figures[kind][i];
                        }
                        temp_kind = kind;
                        temp_color = color;
                        nextTetrimino();
                        can_swap = false;
                    } else {
                        int temp;
                        for (int i=0 ; i<4 ; ++i){
                            temp_figure[i] = figures[kind][i];
                        }
                        swapnextTetrimino();
                        temp = kind;
                        kind = temp_kind;
                        temp_kind = temp;
                        temp = color;
                        color = temp_color;
                        temp_color = temp;
                    }
                }
                break;
			default:
				break;
			}
		default:
			break;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_DOWN])       //���J��V��u�U�v�A����U���@��C
		delay = 50;
    if (state[SDL_SCANCODE_SPACE]){     //����L��J�u�ť���v�A������������C
        if (delay != 0){
            SDL_Delay(100);
            delay = 0;
        }
    }
}

void Tetris::setRectPos(SDL_Rect& rect, int x, int y, int w, int h)
{
	rect = { x, y, w, h };
}

void Tetris::moveRectPos(SDL_Rect& rect, int x, int y)
{
	rect.x += x;
	rect.y += y;
}

bool Tetris::isvalid()
{
	for (int i = 0; i < 4; i++)
		if (items[i].x < 0 || items[i].x >= Cols || items[i].y >= Lines)
			return false;
		else if (field[items[i].y][items[i].x])
			return false;
	return true;
}

void Tetris::gameplay()
{

	//backup
	for (int i = 0; i < 4; i++)
		backup[i] = items[i];
	//move
	if (dx)
	{
		for (int i = 0; i < 4; i++)
		{
			items[i].x += dx;
		}
        if (!isvalid())     //�Y���ʧ@�O���ŦX�W�w���]isvalid()�^��false�^�h�N����O���b��Ӫ����A
            for (int i = 0; i < 4; i++)
				items[i] = backup[i];
	}

	//rotate
	if (rotate)
	{
		Point p = items[2];	// center of rotation
		for (int i = 0; i < 4; i++)
		{
			int x = items[i].y - p.y;
			int y = items[i].x - p.x;
			items[i].x = p.x - x;
			items[i].y = p.y + y;
		}
		if (!isvalid())
			for (int i = 0; i < 4; i++)
				items[i] = backup[i];
	}

    //��ɶ��t�j��dalay�A����|�U���@��C
	if (currentTime - startTime > delay)
	{
		for (int i = 0; i < 4; i++)
			backup[i] = items[i];

        for (int i = 0; i < 4; i++){
            items[i].y++;
        }

        //��isvalid()�^��false�ɡA�N�������m�T�w�b�C�����e���W�C
		if (!isvalid())
		{
			for (int i = 0; i < 4; i++)
				field[backup[i].y][backup[i].x] = color;
            if (delay == 0){
                delay = gamevel;
            }
			nextTetrimino();
		}

		startTime = currentTime;
	}

	//check lines
    //�P�_�C���O�_����
	for (int j=0 ; j<Cols ; ++j){
        if (field[0][j] != 0){
            running = false;
        }
	}

    //�P�_�O�_���@��C���Q�����
	int k = Lines - 1;
	for (int i = k; i > 0; i--)
	{
		int count = 0;
		for (int j = 0; j < Cols; j++)
		{
			if (field[i][j]){
                count++;
			}
			field[k][j] = field[i][j];
		}

		if (count == Cols){
            have_rows = true;
            time++;
		} else {
            k--;
            have_rows = false;
		}
	}

	dx = 0;
	rotate = false;
	if (delay == 0){
        delay = 0;
	} else {
        delay = gamevel;
	}
}

void Tetris::updateRender()
{
    //�N��e�Ҧ������render
	SDL_RenderCopy(render, background, NULL, NULL);
	for (int i = 0; i < Lines; i++){
        for (int j = 0; j < Cols; j++){
            if (field[i][j]){
				setRectPos(srcR, field[i][j] * BlockW);
				setRectPos(destR, j * BlockW, i * BlockH);
				moveRectPos(destR, BlockW, ScreenH - (Lines + 1) * BlockH);
				SDL_RenderCopy(render, blocks, &srcR, &destR);
			}
        }
	}

	//render��e���������
	for (int i = 0; i < 4; i++){
		setRectPos(srcR, color * BlockW);
		setRectPos(destR, items[i].x * BlockW, items[i].y * BlockH);
		moveRectPos(destR, BlockW, ScreenH - (Lines + 1) * BlockH);
		SDL_RenderCopy(render, blocks, &srcR, &destR);
	}

	//render Score
	SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0xFF);
    SDL_Color textColor = { 255, 255, 255 };

    if (have_rows == false && time != 0){
        score += countscore(time);
        time = 0;
    }
    string SCORE = "Score :" + to_string(score);
    if( !scoretext.loadFromRenderedText( SCORE , textColor, render ) )
    {
        printf( "Failed to render text texture!\n" );
    }
    scoretext.render(450, 50, render);

    //redner Level
    if (score / 2000 >= level){
        level++;
        gamevel -= 50;
    }
    string LEVEL = "Level " + to_string(level);
    if( !leveltext.loadFromRenderedText( LEVEL , textColor, render) )
    {
        printf( "Failed to render text texture!\n" );
    }
    leveltext.render(450, 660, render);

    //render Next
    nexttext.render(450, 200, render);
    for (int i = 0; i < 4; i++)
    {
        setRectPos(nextR, next_color * BlockW);
        setRectPos(holdR, (next_figure[i] % 4)*BlockW + 450, int(next_figure[i] / 4) * BlockW + 270);
        SDL_RenderCopy(render, blocks, &nextR, &holdR);
    }

    holdtext.render(450, 420, render);
    if (swap == true){
        for (int i = 0; i < 4; i++)
        {
            setRectPos(holdR, (temp_figure[i] % 4)*BlockW + 450, int(temp_figure[i] / 4) * BlockW + 490);
            SDL_RenderCopy(render, blocks, &tempR, &holdR);
        }
    }

    //�e�X20 X 10 ���C�����
    SDL_Rect outlineRect = { 35, 25, 360, 720};
    SDL_SetRenderDrawColor(render, 0x00, 0xFF, 0x00, 0xFF );
    SDL_RenderDrawRect(render, &outlineRect );

    for (int i=0 ; i < 360 ; i+= 36){
        for (int j=0 ; j < 720 ; j+=36){
            SDL_Rect outlineRect = { i+35, j+25, 36, 36};
            SDL_SetRenderDrawColor(render, 0x00, 0xFF, 0x00, 0xFF );
            SDL_RenderDrawRect(render, &outlineRect );
        }
    }
	SDL_RenderPresent(render);

}

void Tetris::clean()
{
	//Free loaded images
	scoretext.free();
	leveltext.free();
	nexttext.free();
	gPromptTextTexture1.free();
	gPromptTextTexture2.free();
	gInputTextTexture.free();
	for (int i=0 ; i<5 ; ++i){
        playername[i].free();
        playerscore[i].free();
	}

	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Free the music
	Mix_FreeMusic( gMusic );
	gMusic = NULL;

	//Destroy window
	SDL_DestroyRenderer( render );
	SDL_DestroyTexture(blocks);
	SDL_DestroyTexture(scoreboard);
	SDL_DestroyTexture(gameover);
	SDL_DestroyTexture(gamestart);
	SDL_DestroyTexture(start);
	SDL_DestroyWindow( window );
	window = NULL;
	render = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
	Mix_Quit();
}

int Tetris::loadLeaderboard() {
    int n = -1;
    fstream file;
    file.open("load/rank.txt");
    for (int i = 0; i < 5; i++) {
        file >> name[i] >> scores[i];
    }
    for (int i=0 ; i<4 ; ++i){
        for (int j=0 ; j<4-i ; ++j){
            if (scores[j] < scores[j+1]){
                int temp = scores[j];
                scores[j] = scores[j+1];
                scores[j+1] = temp;
                string s = name[j];
                name[j] = name[j+1];
                name[j+1] = s;
            }
        }
    }
    for (int i = 0; i < 5; i++) {
        if (score > scores[i]) {
            n = i;
            break;
        }
    }
    file.close();
    return n;
}

void Tetris::save_leaderboard(int i) {
    fstream file;
    file.open("load/rank.txt");
    bool name_exist = false;
    for (int i=0 ; i<5 ; ++i){
        if (name[i] == inputText){
            name_exist = true;
            scores[i] = score;
            break;
        }
    }
    if (name_exist == true){
        for (int i=0 ; i<4 ; ++i){
            for (int j=0 ; j<4-i ; ++j){
                if (scores[j] < scores[j+1]){
                    int temp = scores[j];
                    scores[j] = scores[j+1];
                    scores[j+1] = temp;
                    string s = name[j];
                    name[j] = name[j+1];
                    name[j+1] = s;
                }
            }
        }
    } else {
        for (int j = 4; j > i; j--) {
            scores[j] = scores[j-1];
            name[j] = name[j - 1];
        }
        scores[i] = score;
        name[i] = inputText;
    }
    for (int i = 0; i < 5; i++) {
        file << name[i] << " " << scores[i] << "\n";
    }
    file.close();
}


LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}


#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor, SDL_Renderer* gRenderer)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}


	//Return success
	return mTexture != NULL;
	}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render( int x, int y, SDL_Renderer* gRenderer , SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

void handlegamestart()
{
    bool quit = false;

    SDL_Event e;
    SDL_Rect src = {0, 0, 1200, 500};
    SDL_Rect des = {250, 600, 200, 100};

    while( !quit )
    {
        while( SDL_PollEvent( &e ) != 0 )
        {
            int x, y;
            SDL_GetMouseState( &x, &y );

            //�P�_�ƹ��I���O�_�bSTART�ϰ줺��
            if( e.type == SDL_MOUSEBUTTONDOWN && x >= 250 && x <= 450 && y >= 600 && y <= 700)
            {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor( render, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( render );

        SDL_RenderCopy( render, gamestart, NULL, NULL );
        SDL_RenderCopy( render, start, &src, &des );
        for (int i=0 ; i<5 ; ++i){
            playername[i].render(250, 245 + i*65, render);
            playerscore[i].render(430, 245 + i*65, render);
        }

        SDL_RenderPresent( render );
    }
}

void handleScoreBoard()
{
    bool quit = false;

    SDL_Event e;

    while( !quit )
    {
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
        }

        SDL_RenderClear( render );

        SDL_RenderCopy( render, scoreboard, NULL, NULL );
        for (int i=0 ; i<5 ; ++i){
            playername[i].render(250, 245 + i*65, render);
            playerscore[i].render(430, 245 + i*65, render);
        }

        SDL_RenderPresent( render );
    }
}

void handleuserinput()
{
    bool quit = false;

    SDL_Event e;

    SDL_Color textColor = { 0, 0, 0, 0xFF };

    //Enable text input
    SDL_StartTextInput();

    while( !quit )
    {
        bool renderText = false;

        while( SDL_PollEvent( &e ) != 0 )
        {
            //Special key input
            if( e.type == SDL_KEYDOWN )
            {
                //Handle backspace
                if(e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
                {
                    //lop off character
                    inputText.pop_back();
                    renderText = true;
                } else if (e.key.keysym.sym == SDLK_RETURN){
                    quit = true;
                }
            }
            //Special text input event
            else if( e.type == SDL_TEXTINPUT )
            {
                //Append character
                inputText += e.text.text;
                renderText = true;
            }
        }

        //Rerender text if needed
        if( renderText )
        {
            //Text is not empty
            if( inputText != "" )
            {
                //Render new text
                gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor, render );
            }
            //Text is empty
            else
            {
                //Render space texture
                gInputTextTexture.loadFromRenderedText( " ", textColor, render );
            }
        }

        SDL_SetRenderDrawColor( render, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( render );

        gPromptTextTexture1.render( ( ScreenW - gPromptTextTexture1.getWidth() ) / 2, 200, render );
        gPromptTextTexture2.render( ( ScreenW - gPromptTextTexture1.getWidth() ) / 2 - 45 , 300, render );
        gInputTextTexture.render( ( ScreenW - gInputTextTexture.getWidth() ) / 2 - 15 , 400, render );

        SDL_RenderPresent( render );
    }

    //Disable text input
    SDL_StopTextInput();
}


