


#include "Game.h"
#include "Shader.h"

#define NUM_FRAMES 7
#define NUM_ANIM 16
#define FRAME_DUR 70

using namespace glm;
using namespace irrklang;
class Main :
	public Engine::Game, public Shader
{
public:
	Main();
	~Main();
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void ControllerDownMapping(int key);
	virtual void ControllerUpMapping(int key);
	virtual void KeyboardUpMapping(int key);
	virtual void KeyboardDownMapping(int key);
	virtual void Render();
private:
	int STATE = 0;
	ISoundEngine *SoundEngine = createIrrKlangDevice();
	const int STATE_START = 0, STATE_CHOOSE = 1, STATE_PLAY = 2, STATE_OVER = 3;
	const char* MP3NAME = "Opening Menu.wav";
	const char* MP3NAME2 = "In Game.wav";
	bool play1 = false;
	bool play2 = false;
	int plus = 0;
	int killCount = 0;
	int indexPilihan = 0;
	//UI
	bool show_start_window = true;
	bool show_choose_window = true;
	//Player
	GLuint VBOlogo, VAOlogo, EBOlogo, texturelogo, programlogo;

	//Player
	GLuint VBO[2], VAO[2], EBO[2], texture[2], program[2];
	int damage[2] = {0,0};
	int health[2] = {0,0};
	bool ranged = false;
	mat4 transformPlayer;
	bool shooting = false;
	int currentAnimMoveIndex = 0;
	const int MOVE_UP = 5, MOVE_UP_RIGHT = 4, MOVE_RIGHT = 3, MOVE_DOWN_RIGHT = 2, MOVE_DOWN = 1, MOVE_DOWN_LEFT = 0, MOVE_LEFT = 7, MOVE_UP_LEFT = 6;
	bool up = false, right = false, left = false, down = false, move = false;
	float frame_dur = 0, frame_width = 0, xpos = 0, ypos = 0, zpos = 0, pheight, pwidth;
	unsigned int frame_idx = 0;
	unsigned int frame_idy = 0;

	//AttackBox
	GLuint VBOrock[6], VAOrock[6], EBOrock[6], textureRock[6], programRock[6];
	bool shoot[6] = { false,false,false,false ,false,false };
	bool ballRender[6] = { false,false,false,false,false,false };
	int maxBall = 6;
	int indexBall = 0;
	int currentAnimMoveIndexAttack[6] = { false,false,false,false,false,false };
	bool shoots[6] = { false,false,false,false,false,false };
	float  xPpos[6] = { 0,0,0,0,0 ,0};
	float yPpos[6] = { 0,0,0,0,0,0 };
	float zPpos[6] = { 0,0,0,0,0,0 };


	//Enemy
	GLuint VBOenemy[4], VAOenemy[4], EBOenemy[4], textureEnemy[4], programEnemy[4];
	int damageEnemy[4] = { 10,10,10 ,100};
	int healthEnemy[4] = { 100,100,100,300 };
	bool boss = false;
	int deathCount = 0;
	int wave = 1;
	float frame_durEnemy[4] = { 0,0,0,0 };
	float frame_widthEnemy[4] = { 0,0,0,0 };
	float xposEnemy[4] = { 1,-1,-1,1 };
	float yposEnemy[4] = { 1,-1,1,-1 };
	float zposEnemy[4] = { 0,0,0,0 };
	float heightEnemy[4] = { 0,0,0,0 };
	float widthEnemy[4] = { 0,0,0,0 };
	int currentAnimMoveIndexEnemy[4] = { 99 ,99,99,99 };
	bool enemyShoot[4] = { false ,false,false,false };
	bool moveEnemy[4] = { false ,false,false,false};
	unsigned int frame_idxEnemy[4] = { 0 ,0,0,0 };
	unsigned int frame_idyEnemy[4] = { 0,0,0,0 };


	void UIManager();
	void BuildSprite();
	void BuildEnemy(int index, int type);
	void BuildOpLogo();
	void BuildAttack();
	void Draw();
	void Reset(int plus);
	void Shoot(int index);
	void UpdateSprite(float deltaTime, int indexAnim);
	void UpdateSpriteEnemy(float deltaTime, int indexAnim, GLuint program, int index);
	void MoveSprite(float deltaTime);
	void MoveSpriteEnemy(float deltaTime, int index, bool ranged);
	void MoveProjectile(float deltaTime, int directionState, int ballIndex, bool ranged, bool player);
	bool Main::IsCollided(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2);
};

