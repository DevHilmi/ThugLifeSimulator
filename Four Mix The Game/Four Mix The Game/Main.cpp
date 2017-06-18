#include "Main.h"


Main::Main()
{
}


Main::~Main()
{
	ImGui_ImplSdlGL3_Shutdown();
}

void Main::Init()
{
	ImGui_ImplSdlGL3_Init(this->window);
	BuildAttack();
	BuildSprite();
	BuildEnemy(0, 1);
	BuildEnemy(1, 1);
	BuildEnemy(2, 0);
	BuildEnemy(3, 1);
	BuildOpLogo();
}

void Main::Update(float deltaTime)
{
	UIManager();
	if (STATE == STATE_PLAY) {
		UpdateSprite(deltaTime, currentAnimMoveIndex);
		UpdateSpriteEnemy(deltaTime, currentAnimMoveIndexEnemy[0], programEnemy[0], 0);
		UpdateSpriteEnemy(deltaTime, currentAnimMoveIndexEnemy[1], programEnemy[1], 1);
		UpdateSpriteEnemy(deltaTime, currentAnimMoveIndexEnemy[2], programEnemy[2], 2);
		
		MoveSprite(deltaTime);
		MoveSpriteEnemy(deltaTime, 0, false);
		MoveSpriteEnemy(deltaTime, 1, false);
		MoveSpriteEnemy(deltaTime, 2, true);
		if (boss) {
			MoveSpriteEnemy(deltaTime, 3, false);
			UpdateSpriteEnemy(deltaTime, currentAnimMoveIndexEnemy[3], programEnemy[3], 3);
		}
		
		if (shoot[0]) {
			MoveProjectile(deltaTime, currentAnimMoveIndexAttack[indexBall], 0, ranged, true);
		}
		if (shoot[1]) {
			MoveProjectile(deltaTime, currentAnimMoveIndexAttack[indexBall], 1, ranged, true);
		}
		if (shoot[2]) {
			MoveProjectile(deltaTime, currentAnimMoveIndexEnemy[2], 2, true, false);
		}
		if (shoot[3]) {
			MoveProjectile(deltaTime, currentAnimMoveIndexEnemy[1], 3, false, false);
		}
		if (shoot[4]) {
			MoveProjectile(deltaTime, currentAnimMoveIndexEnemy[0], 4, false, false);
		}
		if (shoot[5]) {
			MoveProjectile(deltaTime, currentAnimMoveIndexEnemy[3], 5, false, false);
		}
		if (health[indexPilihan] < 0) {
			STATE = STATE_OVER;
			if (indexPilihan == 0) {
			health[indexPilihan] = 1000;
		}
			else {
				health[indexPilihan] = 1200;
			}
		}

	}
}

void Main::ControllerDownMapping(int key)
{
	if (key == SDL_CONTROLLER_BUTTON_DPAD_UP) {
		up = true;
	}
	else if (key == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
		right = true;
	}
	else if (key == SDL_CONTROLLER_BUTTON_DPAD_LEFT) {
		left = true;
	}
	else if (key == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
		down = true;
	}
	else if (key == SDL_CONTROLLER_BUTTON_A) {
		if (ranged) {
			indexBall = 1;
		}
		else {
			indexBall = 0;
		}
		if (!ballRender[indexBall]) {
			shooting = true;
			ballRender[indexBall] = true;
			currentAnimMoveIndexAttack[indexBall] = currentAnimMoveIndex;
			xPpos[indexBall] = xpos;
			yPpos[indexBall] = ypos;
			frame_idx = 0;
		}
	}
	else if (key == SDL_CONTROLLER_BUTTON_X) {

	}
	else if (up || right || left || down)
		move = true;
}

void Main::ControllerUpMapping(int key)
{
	if (key == SDL_CONTROLLER_BUTTON_DPAD_UP) {
		up = false;
	}
	else if (key == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
		right = false;
	}
	else if (key == SDL_CONTROLLER_BUTTON_DPAD_LEFT) {
		left = false;
	}
	else if (key == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
		down = false;
	}

	if (!up && !right && !left && !down);
	move = false;
}

void Main::KeyboardUpMapping(int key)
{
	if (key == SDL_Keycode('w')) {
		up = false;
	}
	else if (key == SDL_Keycode('d')) {
		right = false;
	}
	else if (key == SDL_Keycode('a')) {
		left = false;
	}
	else if (key == SDL_Keycode('s')) {
		down = false;
	}

	if (!up && !right && !left && !down);
	move = false;
}

void Main::KeyboardDownMapping(int key)
{
	if (key == SDL_Keycode('w')) {
		up = true;
	}
	else if (key == SDL_Keycode('d')) {
		right = true;
	}
	else if (key == SDL_Keycode('a')) {
		left = true;
	}
	else if (key == SDL_Keycode('s')) {
		down = true;
	}
	else if (key == SDL_Keycode(' ')) {
		if (ranged) {
			indexBall = 1;
		}
		else {
			indexBall = 0;
		}
		if (!ballRender[indexBall]) {
			shooting = true;
			ballRender[indexBall] = true;
			currentAnimMoveIndexAttack[indexBall] = currentAnimMoveIndex;
			xPpos[indexBall] = xpos;
			yPpos[indexBall] = ypos;
			frame_idx = 0;
		}
	}
	else if (key == SDL_CONTROLLER_BUTTON_X) {

	}
	else if (up || right || left || down)
		move = true;
}

void Main::Render()
{

	//Setting Viewport
	glViewport(0, 0, screenWidth, screenHeight);
	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Set the background color
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	Draw();

	ImGui::Render();
}

void Main::UIManager()
{
	ImGui_ImplSdlGL3_NewFrame(window);
	if (STATE == STATE_START) {
		if (!play1) {
			SoundEngine->removeAllSoundSources();
			SoundEngine->play2D(this->MP3NAME, GL_TRUE);
			play1 = true;
		}
		
		ImGui::Begin("Thug Life Simulator", &show_start_window);
		ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiSetCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Appearing);
		if (ImGui::Button("Play")) STATE = STATE_CHOOSE;
		if (ImGui::Button("Exit Game")) {
			SDL_Quit();
			exit(1);
		}
		ImGui::End();
	}
	else if (STATE == STATE_CHOOSE) {
		ImGui::Begin("Choose Character", &show_start_window);
		ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiSetCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Appearing);
		if (ImGui::Button("HILMI PRATAMA (RANGED)")) {
			STATE = STATE_PLAY;
			indexPilihan = 0;
			ranged = true;
		}
		if (ImGui::Button("BARIQ ALA (MEELE)")) {
			STATE = STATE_PLAY;
			indexPilihan = 1;
			ranged = false;

		}
		ImGui::End();
	}
	else if (STATE == STATE_PLAY) {
		if (!play2) {
			
			SoundEngine->removeAllSoundSources();
			SoundEngine->play2D(this->MP3NAME2, GL_TRUE);
			play2 = true;
		}
		ImGui::Begin("Thug Life Simulator", &show_start_window);
		ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiSetCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Appearing);
		ImGui::Text("Health :%d", health[indexPilihan]);
		ImGui::Text("Kill count :%d", killCount);
		ImGui::Text("Wave	:%d", wave);
		if (ImGui::Button("MAIN MENU")) {
			Reset(0);
			STATE = STATE_START;
		}
		if (ImGui::Button("EXIT")) {
			SDL_Quit();
			exit(1);
		}
		ImGui::End();
	}
	else if (STATE == STATE_OVER) {
		ImGui::Begin("GAME OVER", &show_start_window);
		ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiSetCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(1, 1), ImGuiSetCond_Appearing);
		ImGui::Text("Kill count :%d", killCount);
		ImGui::Text("SCORE WAVE	:%d", wave);
		if (ImGui::Button("MAIN MENU")) {
			Reset(0);
			STATE = STATE_START;
		}
		if (ImGui::Button("EXIT")) {
			SDL_Quit();
			exit(1);
		}
		ImGui::End();
	}


}


void Main::UpdateSprite(float deltaTime, int indexAnim)
{
	frame_dur += deltaTime;
	GLint location = glGetUniformLocation(this->program[indexPilihan], "frameIndex");
	GLint locationy = glGetUniformLocation(this->program[indexPilihan], "animIndex");
	UseShader(this->program[indexPilihan]);
	// Update animation
	if (indexAnim != 99) {
		if (!shooting && move) {
			frame_idy = currentAnimMoveIndex;
			glUniform1i(locationy, frame_idy);
		}
		else if (shooting) {
			frame_idy = currentAnimMoveIndex + 8;
			glUniform1i(locationy, frame_idy);
		}
	}
	else if (!shooting) {
		glUniform1i(location, 0);
	}

	if (frame_dur > FRAME_DUR) {
		if (move && !shooting) {
			frame_dur = 0;
			if (frame_idx == NUM_FRAMES - 1) frame_idx = 0;  else frame_idx++;
			// Pass frameIndex to shader
			glUniform1i(location, frame_idx);
		}
		else if (shooting) {
			frame_dur = 0;
			if (frame_idx == NUM_FRAMES - 1) {
				frame_idx = 0;	shooting = false;
			}
			else frame_idx++;
			if (frame_idx == 6) {
				shoot[indexBall] = true;

			}

			glUniform1i(location, frame_idx);
			// Pass frameIndex to shader
		}
		else if (!move && !shooting) {
			// Pass frameIndex to shader
			glUniform1i(location, 0);
		}
	}
}

void Main::UpdateSpriteEnemy(float deltaTime, int indexAnim, GLuint program, int index)
{
	frame_durEnemy[index] += deltaTime;
	GLint location = glGetUniformLocation(program, "frameIndex");
	GLint locationy = glGetUniformLocation(program, "animIndex");
	UseShader(program);
	// Update animation
	if (indexAnim != 99) {
		if (!enemyShoot[index] && moveEnemy[index]) {
			frame_idyEnemy[index] = currentAnimMoveIndexEnemy[index];
			glUniform1i(locationy, frame_idyEnemy[index]);

		}
		else if (enemyShoot[index]) {
			frame_idyEnemy[index] = currentAnimMoveIndexEnemy[index] + 8;
			glUniform1i(locationy, frame_idyEnemy[index]);
		}
	}
	else if (!enemyShoot[index]) {
		glUniform1i(location, 0);
	}

	if (frame_durEnemy[index] > FRAME_DUR) {
		if (moveEnemy[index] && !enemyShoot[index]) {
			frame_durEnemy[index] = 0;
			if (frame_idxEnemy[index] == NUM_FRAMES - 1) frame_idxEnemy[index] = 0;  else frame_idxEnemy[index]++;
			// Pass frameIndex to shader
			glUniform1i(location, frame_idxEnemy[index]);
		}
		else if (enemyShoot[index]) {
			frame_durEnemy[index] = 0;
			if (frame_idxEnemy[index] == NUM_FRAMES - 1) {
				frame_idxEnemy[index] = 0;	enemyShoot[index] = false;
			}
			else frame_idxEnemy[index]++;

			if (frame_idxEnemy[index] == 5 && index == 2) {
				if (!ballRender[2]) {
					shoot[2] = true;
					ballRender[2] = true;
					currentAnimMoveIndexAttack[2] = currentAnimMoveIndexEnemy[2];
					xPpos[2] = xposEnemy[2];
					yPpos[2] = yposEnemy[2];
					frame_idxEnemy[2] = 0;
				}
			}
			else if (frame_idxEnemy[index] == 5 && index == 1) {
				if (!ballRender[3]) {
					shoot[3] = true;
					ballRender[3] = true;
					currentAnimMoveIndexAttack[3] = currentAnimMoveIndexEnemy[index];
					xPpos[3] = xposEnemy[index];
					yPpos[3] = yposEnemy[index];
					frame_idxEnemy[index] = 0;
				}
			}
			else if (frame_idxEnemy[index] == 5 && index == 0) {
				if (!ballRender[4]) {
					shoot[4] = true;
					ballRender[4] = true;
					currentAnimMoveIndexAttack[4] = currentAnimMoveIndexEnemy[index];
					xPpos[4] = xposEnemy[index];
					yPpos[4] = yposEnemy[index];
					frame_idxEnemy[index] = 0;
				}
			}
			else if (frame_idxEnemy[index] == 5 && index == 4) {
				if (!ballRender[5]) {
					shoot[5] = true;
					ballRender[5] = true;
					currentAnimMoveIndexAttack[5] = currentAnimMoveIndexEnemy[index];
					xPpos[5] = xposEnemy[index];
					yPpos[5] = yposEnemy[index];
					frame_idxEnemy[index] = 0;
				}
			}


			currentAnimMoveIndexEnemy[index] = 99;
			glUniform1i(location, frame_idxEnemy[index]);
			// Pass frameIndex to shader
		}
		else if (!moveEnemy[index] && !enemyShoot[index]) {
			// Pass frameIndex to shader
			glUniform1i(location, 0);
		}
	}
}

void Main::MoveSprite(float deltaTime)
{
	float oldXpos = xpos;
	float oldYpos = ypos;
	if (!shooting) {
		if (up) {
			move = true;
			if (right) {
				currentAnimMoveIndex = MOVE_UP_RIGHT;
				ypos += deltaTime * 0.00031f;
				xpos += deltaTime * 0.00031f;
			}
			else if (left) {
				currentAnimMoveIndex = MOVE_UP_LEFT;
				ypos += deltaTime * 0.00031f;
				xpos -= deltaTime * 0.00031f;
			}
			else {
				currentAnimMoveIndex = MOVE_UP;
				ypos += deltaTime * 0.0005f;
			}
		}
		else if (down) {
			move = true;
			if (right) {
				currentAnimMoveIndex = MOVE_DOWN_RIGHT;
				ypos -= deltaTime * 0.00031f;
				xpos += deltaTime * 0.00031f;
			}
			else if (left) {
				currentAnimMoveIndex = MOVE_DOWN_LEFT;
				ypos -= deltaTime * 0.00031f;
				xpos -= deltaTime * 0.00031f;
			}
			else {
				currentAnimMoveIndex = MOVE_DOWN;
				ypos -= deltaTime * 0.0005f;
			}
		}
		else if (right && !up && !down) {
			move = true;
			currentAnimMoveIndex = MOVE_RIGHT;
			xpos += deltaTime * 0.0005f;
		}
		else if (left && !up && !down) {
			move = true;
			currentAnimMoveIndex = MOVE_LEFT;
			xpos -= deltaTime * 0.0005f;
		}
		else {
			move = false;
		}
	}
	for (int i = 0; i < 4; i++) {
		if (IsCollided(xpos, ypos, pwidth, pheight, xposEnemy[i], yposEnemy[i], widthEnemy[i], heightEnemy[i])) {
			xpos = oldXpos;
			ypos = oldYpos;
		}
	}
	if (xpos > 1 || xpos < -1 || ypos>1 || ypos < -1) {
		xpos = oldXpos;
		ypos = oldYpos;
	}
	mat4 transform;
	transform = translate(transform, vec3(xpos, ypos, zpos)); //Membuat translate 
	GLint location = glGetUniformLocation(this->program[indexPilihan], "transform"); //Mengambil transform location dari shader
	UseShader(this->program[indexPilihan]); //Memakai shader sebelum mengedit isi shader
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(transform));//Masukin transform ke location shader
	transformPlayer = transform;


}

void Main::MoveSpriteEnemy(float deltaTime, int index, bool ranged)
{
	float oldXpos = xposEnemy[index];
	float oldYpos = yposEnemy[index];
	if (ypos > yposEnemy[index] + 0.1) {
		moveEnemy[index] = true;
		if (xpos > xposEnemy[index] + 0.1) {
			currentAnimMoveIndexEnemy[index] = MOVE_UP_RIGHT;
			yposEnemy[index] += deltaTime * 0.000131f;
			xposEnemy[index] += deltaTime * 0.000131f;

		}
		else if (xpos < xposEnemy[index] - 0.1) {
			currentAnimMoveIndexEnemy[index] = MOVE_UP_LEFT;
			yposEnemy[index] += deltaTime * 0.000131f;
			xposEnemy[index] -= deltaTime * 0.000131f;
		}
		else {
			currentAnimMoveIndexEnemy[index] = MOVE_UP;
			yposEnemy[index] += deltaTime * 0.00015f;
		}
	}
	else if (ypos < yposEnemy[index] - 0.1) {
		moveEnemy[index] = true;
		if (xpos > xposEnemy[index] + 0.1) {
			currentAnimMoveIndexEnemy[index] = MOVE_DOWN_RIGHT;
			yposEnemy[index] -= deltaTime * 0.000131f;
			xposEnemy[index] += deltaTime * 0.000131f;
		}
		else if (xpos < xposEnemy[index] - 0.1) {
			currentAnimMoveIndexEnemy[index] = MOVE_DOWN_LEFT;

			yposEnemy[index] -= deltaTime * 0.000131f;
			xposEnemy[index] -= deltaTime * 0.000131f;
		}
		else {
			currentAnimMoveIndexEnemy[index] = MOVE_DOWN;
			yposEnemy[index] -= deltaTime * 0.00015f;
		}
	}
	else if (ypos <= yposEnemy[index] + 0.1 && ypos >= yposEnemy[index] - 0.1 && xpos > xposEnemy[index]) {
		moveEnemy[index] = true;
		currentAnimMoveIndexEnemy[index] = MOVE_RIGHT;
		xposEnemy[index] += deltaTime * 0.00015f;
	}
	else if (ypos <= yposEnemy[index] && xpos < xposEnemy[index]) {
		moveEnemy[index] = true;
		currentAnimMoveIndexEnemy[index] = MOVE_LEFT;
		xposEnemy[index] -= deltaTime * 0.00015f;
	}
	else {
		moveEnemy[index] = false;
	}
	if (ranged && (glm::distance(glm::vec2(xpos, ypos), glm::vec2(xposEnemy[index], yposEnemy[index])) < 0.8)) {
		xposEnemy[index] = oldXpos;
		yposEnemy[index] = oldYpos;
		enemyShoot[index] = true;
		moveEnemy[index] = false;
	}
	else if (IsCollided(xpos, ypos, pwidth, pheight, xposEnemy[index], yposEnemy[index], widthEnemy[index], heightEnemy[index])) {

		xposEnemy[index] = oldXpos;
		yposEnemy[index] = oldYpos;
		enemyShoot[index] = true;
		moveEnemy[index] = false;

	}
	else {
		enemyShoot[index] = false;
		moveEnemy[index] = true;
	}
	int indexTemp = index;
	indexTemp++;
	if (indexTemp > 4) {
		indexTemp = 0;
	}
	if (IsCollided(xposEnemy[indexTemp], yposEnemy[indexTemp], widthEnemy[indexTemp], heightEnemy[indexTemp], xposEnemy[index], yposEnemy[index], widthEnemy[index], heightEnemy[index])) {
		xposEnemy[index] = oldXpos;
		yposEnemy[index] = oldYpos;
	}
	indexTemp++;
	if (indexTemp > 4) {
		indexTemp = 0;
	}
	if (IsCollided(xposEnemy[indexTemp], yposEnemy[indexTemp], widthEnemy[indexTemp], heightEnemy[indexTemp], xposEnemy[index], yposEnemy[index], widthEnemy[index], heightEnemy[index])) {
		xposEnemy[index] = oldXpos;
		yposEnemy[index] = oldYpos;
	}
	mat4 transform;
	transform = translate(transform, vec3(xposEnemy[index], yposEnemy[index], zpos)); //Membuat translate 
	GLint location = glGetUniformLocation(programEnemy[index], "transform"); //Mengambil transform location dari shader
	UseShader(programEnemy[index]); //Memakai shader sebelum mengedit isi shader
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(transform));//Masukin transform ke location shader

}

void Main::MoveProjectile(float deltaTime, int directionState, int ballIndex, bool ranged, bool player)
{
	float tempy = yPpos[ballIndex];
	float tempx = xPpos[ballIndex];
	if (directionState == MOVE_UP_RIGHT) {
		yPpos[ballIndex] += deltaTime * 0.00131f;
		xPpos[ballIndex] += deltaTime * 0.00131f;
	}
	else if (directionState == MOVE_UP_LEFT) {
		yPpos[ballIndex] += deltaTime * 0.00131f;
		xPpos[ballIndex] -= deltaTime * 0.00131f;
	}
	else if (directionState == MOVE_UP) {
		yPpos[ballIndex] += deltaTime * 0.0018f;
	}
	else if (directionState == MOVE_DOWN_RIGHT) {
		yPpos[ballIndex] -= deltaTime * 0.00131f;
		xPpos[ballIndex] += deltaTime * 0.00131f;
	}
	else if (directionState == MOVE_DOWN_LEFT) {
		yPpos[ballIndex] -= deltaTime * 0.00131f;
		xPpos[ballIndex] -= deltaTime * 0.00131f;
	}
	else if (directionState == MOVE_DOWN) {
		yPpos[ballIndex] -= deltaTime * 0.0018f;
	}
	else if (directionState == MOVE_RIGHT) {
		xPpos[ballIndex] += deltaTime * 0.0018f;
	}
	else if (directionState == MOVE_LEFT) {
		xPpos[ballIndex] -= deltaTime * 0.0018f;
	}
	if (xPpos[ballIndex] > 1 || xPpos[ballIndex] < -1 || yPpos[ballIndex]>1 || yPpos[ballIndex] < -1) {
		if (!shooting)
			ballRender[ballIndex] = false;
	}
	if (player) {
		for (int i = 0; i < 4; i++) {
			if (IsCollided(xPpos[ballIndex], yPpos[ballIndex], 0.03 * 2, 0.03 * 2, xposEnemy[i], yposEnemy[i], widthEnemy[i], heightEnemy[i])) {
				ballRender[ballIndex] = false;
				shoot[ballIndex] = false;
				healthEnemy[i] -= damage[indexPilihan];
			}
		}
		if (!ranged) {
			if (distance(vec2(xpos, ypos), vec2(xPpos[ballIndex], yPpos[ballIndex])) > 0.2) {
				shooting = false;
				if (!shooting) {
					yPpos[ballIndex] = tempy;
					xPpos[ballIndex] = tempx;
					shoot[ballIndex] = false;
					ballRender[ballIndex] = false;
				}
			}
		}
	}
	else {
		if (IsCollided(xPpos[ballIndex], yPpos[ballIndex], 0.03 * 2, 0.03 * 2, xpos, ypos, pwidth, pheight)) {
			ballRender[ballIndex] = false;
			shoot[ballIndex] = false;
			health[indexPilihan] -= damageEnemy[2];
		}if (!ranged) {
			if (distance(vec2(xposEnemy[1], yposEnemy[1]), vec2(xPpos[ballIndex], yPpos[ballIndex])) > 0.2) {
				shoot[ballIndex] = false;

				if (!shoot[ballIndex]) {
					yPpos[ballIndex] = tempy;
					xPpos[ballIndex] = tempx;
					shoot[ballIndex] = false;
					ballRender[ballIndex] = false;
					health[indexPilihan] -= damageEnemy[1];
				}
			}
			if (distance(vec2(xposEnemy[0], yposEnemy[0]), vec2(xPpos[ballIndex], yPpos[ballIndex])) > 0.2) {
				shoot[ballIndex] = false;

				if (!shoot[ballIndex]) {
					yPpos[ballIndex] = tempy;
					xPpos[ballIndex] = tempx;
					shoot[ballIndex] = false;
					ballRender[ballIndex] = false;
					health[indexPilihan] -= damageEnemy[0];
				}
			}
			if (boss) {
				if (distance(vec2(xposEnemy[3], yposEnemy[3]), vec2(xPpos[ballIndex], yPpos[ballIndex])) > 0.2) {
					cout << "masuksini" << endl;
					shoot[ballIndex] = false;
					if (!shoot[ballIndex]) {
						yPpos[ballIndex] = tempy;
						xPpos[ballIndex] = tempx;
						shoot[ballIndex] = false;
						ballRender[ballIndex] = false;
						health[indexPilihan] -= damageEnemy[3];
					}
				}
			}
			
		}
	}

	mat4 transform;
	transform = translate(transform, vec3(xPpos[ballIndex], yPpos[ballIndex], zPpos[ballIndex]));
	GLint location = glGetUniformLocation(this->programRock[ballIndex], "transform");
	UseShader(this->programRock[ballIndex]);
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(transform));

}


void Main::Draw() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (STATE == STATE_START) {
		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturelogo);
		// Activate shader
		UseShader(this->programlogo);
		// Draw sprite
		glBindVertexArray(VAOlogo);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	if (STATE == STATE_PLAY) {
		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[indexPilihan]);
		// Activate shader
		UseShader(this->program[indexPilihan]);
		// Draw sprite
		glBindVertexArray(VAO[indexPilihan]);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		if (boss) {
			if (healthEnemy[3] < 0) {
				boss = false;
			}
			else {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureEnemy[3]);
				// Activate shader
				UseShader(this->programEnemy[3]);
				// Draw sprite
				glBindVertexArray(VAOenemy[3]);
				glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}

		}
		// Bind Textures using texture units
		for (int i = 0; i < 3; i++) {
			if (healthEnemy[i] < 0 && healthEnemy[i] != -99) {
				killCount++;
				deathCount++;
				healthEnemy[i] = -99;
				xposEnemy[i] = 99;
				yposEnemy[i] = 99;
				if (wave % 3 != 0 && deathCount == 3) {
					wave++;
					plus += 25;
					Reset(plus);
					deathCount = 0;
				}
				else if (wave % 3 == 0) {
					healthEnemy[3] = 300 + plus;
					boss = true;
					wave++;
					plus += 25;
					Reset(plus);
					deathCount = 0;

				}
			}
			else {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureEnemy[i]);
				// Activate shader
				UseShader(this->programEnemy[i]);
				// Draw sprite
				glBindVertexArray(VAOenemy[i]);


				glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);

			}


		}
		for (int i = 0; i < 6; i++) {
			if (shoot[i] == true) {
				Shoot(i);
			}
		}



	}
	
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glDisable(GL_BLEND);
}

void Main::Reset(int plus)
{

	if (plus == 0) {
		play1 = false;
		play2 = false;
		killCount = 0;
		wave = 1;
	}
	else {
		health[indexPilihan] += 100;
	}

	float xposEnemyR[3] = { 1,-1,-1 };
	float yposEnemyR[3] = { 1,-1,1 };;
	int healthEnemyR[3] = { 100 + plus,100 + plus,100 + plus };


	for (int i = 0; i < 3; i++) {
		xposEnemy[i] = xposEnemyR[i];
		yposEnemy[i] = yposEnemyR[i];
		healthEnemy[i] = healthEnemyR[i];
		mat4 transform;
		transform = translate(transform, vec3(xposEnemy[i], yposEnemy[i], zpos)); //Membuat translate 
		GLint location = glGetUniformLocation(programEnemy[i], "transform"); //Mengambil transform location dari shader
		UseShader(programEnemy[i]); //Memakai shader sebelum mengedit isi shader
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(transform));//Masukin transform ke location shader
	}
}

void Main::Shoot(int index)
{
	if (index != 99) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureRock[index]);
		GLint location2 = glGetUniformLocation(this->programRock[index], "ourTexture");
		// Activate shader
		UseShader(this->programRock[index]);
		glUniform1i(location2, 1);

		// Draw projectile
		glBindVertexArray(VAOrock[index]);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

}


void Main::BuildSprite()
{
	for (int i = 0; i < 2; i++) {
		if (i == 0) {
			damage[i] = 25;
			health[i] = 1000;
		}
		else {
			damage[i] = 100;
			health[i] = 1200;
		}
		this->program[i] = BuildShader("sprite.vert", "sprite.frag");

		// Pass n to shader
		GLint locationx = glGetUniformLocation(this->program[i], "nx");
		GLint locationy = glGetUniformLocation(this->program[i], "ny");
		UseShader(this->program[i]);
		glUniform1f(locationx, 1.0f / NUM_FRAMES);
		glUniform1f(locationy, 1.0f / NUM_ANIM);
		// Load and create a texture 
		glGenTextures(1, &texture[i]);
		glBindTexture(GL_TEXTURE_2D, texture[i]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

											   // Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load, create texture 
		int width, height;
		if (i == 0) {
			unsigned char* image = SOIL_load_image("Sprite/hilmiPratama.png", &width, &height, 0, SOIL_LOAD_RGBA);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

											 // Set up vertex data (and buffer(s)) and attribute pointers
		}
		else {
			unsigned char* image = SOIL_load_image("Sprite/bariqAla.png", &width, &height, 0, SOIL_LOAD_RGBA);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

											 // Set up vertex data (and buffer(s)) and attribute pointers
		}


		frame_width = ((float)width) / NUM_FRAMES;
		float w = 0.3f;
		float x = frame_width * w / height * NUM_ANIM;
		float y = w;
		pheight = y*1.02;
		pwidth = x / 3.5;
		GLfloat vertices[] = {
			// Positions   // Colors           // Texture Coords
			x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
			x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
			-x, -y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
			-x,  y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left 
		};

		GLuint indices[] = {  // Note that we start from 0!
			0, 3, 2, 1
		};

		glGenVertexArrays(1, &VAO[i]);
		glGenBuffers(1, &VBO[i]);
		glGenBuffers(1, &EBO[i]);

		glBindVertexArray(VAO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO
	}


}
void Main::BuildEnemy(int index, int type)
{
	this->programEnemy[index] = BuildShader("sprite.vert", "sprite.frag");

	// Pass n to shader
	GLint locationx = glGetUniformLocation(this->programEnemy[index], "nx");
	GLint locationy = glGetUniformLocation(this->programEnemy[index], "ny");
	UseShader(this->programEnemy[index]);
	glUniform1f(locationx, 1.0f / NUM_FRAMES);
	glUniform1f(locationy, 1.0f / NUM_ANIM);
	// Load and create a texture 
	glGenTextures(1, &textureEnemy[index]);
	glBindTexture(GL_TEXTURE_2D, textureEnemy[index]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

										   // Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	if (type == 0) {
		unsigned char* image = SOIL_load_image("Sprite/enemy2.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	}
	else if (index == 3) {
		unsigned char* image = SOIL_load_image("Sprite/boss.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	}
	else {
		unsigned char* image = SOIL_load_image("Sprite/enemy1.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	}

	// Set up vertex data (and buffer(s)) and attribute pointers
	frame_width = ((float)width) / NUM_FRAMES;
	float w = 0.3f;
	float x = frame_width * w / height * NUM_ANIM;
	float y = w;
	heightEnemy[index] = y*1.02;
	widthEnemy[index] = x / 3.5;

	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
		x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
		-x, -y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-x,  y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &VAOenemy[index]);
	glGenBuffers(1, &VBOenemy[index]);
	glGenBuffers(1, &EBOenemy[index]);

	glBindVertexArray(VAOenemy[index]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOenemy[index]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOenemy[index]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
	mat4 transform;
	transform = translate(transform, vec3(xposEnemy[index], yposEnemy[index], zposEnemy[index]));
	GLint location = glGetUniformLocation(programEnemy[index], "transform");
	UseShader(programEnemy[index]);
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(transform));
}

void Main::BuildOpLogo()
{
	programlogo = BuildShader("projectile.vert", "projectile.frag");



	// Load, create texture 
	int width, height;
		// Load and create a texture 
		glGenTextures(1, &texturelogo);
		glBindTexture(GL_TEXTURE_2D, texturelogo); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

													  // Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		unsigned char* image = SOIL_load_image("Sprite/LOGO.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	// Unbind texture when done, so we won't accidentily mess up our texture.

	// Set up vertex data (and buffer(s)) and attribute pointers
		frame_width = ((float)width);
		float w = 0.3f;
		float x = frame_width * w / height;
		float y = w;
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
		x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
		-x, -y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-x,  y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &VAOlogo);
	glGenBuffers(1, &VBOlogo);
	glGenBuffers(1, &EBOlogo);

	glBindVertexArray(VAOlogo);

	glBindBuffer(GL_ARRAY_BUFFER, VBOlogo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOlogo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

	mat4 transform;
	transform = translate(transform, vec3(xpos, ypos, zpos));
	GLint location = glGetUniformLocation(programlogo, "transform");
	UseShader(programlogo);
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(transform));
}

void Main::BuildAttack()
{

	for (int i = 0; i < 6; i++) {
		programRock[i] = BuildShader("projectile.vert", "projectile.frag");



		// Load, create texture 
		int width, height;
		if (i == 0 || i == 3 || i == 4) {
			// Load and create a texture 
			glGenTextures(1, &textureRock[i]);
			glBindTexture(GL_TEXTURE_2D, textureRock[i]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

														  // Set texture filtering
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			unsigned char* image = SOIL_load_image("Sprite/meele.png", &width, &height, 0, SOIL_LOAD_RGBA);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else {
			// Load and create a texture 
			glGenTextures(1, &textureRock[i]);
			glBindTexture(GL_TEXTURE_2D, textureRock[i]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

														  // Set texture filtering
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			unsigned char* image = SOIL_load_image("Sprite/ball.png", &width, &height, 0, SOIL_LOAD_RGBA);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		// Unbind texture when done, so we won't accidentily mess up our texture.

										// Set up vertex data (and buffer(s)) and attribute pointers
		float w = 0.03f;
		float x = w;
		float y = w;
		GLfloat vertices[] = {
			// Positions   // Colors           // Texture Coords
			x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
			x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
			-x, -y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
			-x,  y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left 
		};

		GLuint indices[] = {  // Note that we start from 0!
			0, 3, 2, 1
		};

		glGenVertexArrays(1, &VAOrock[i]);
		glGenBuffers(1, &VBOrock[i]);
		glGenBuffers(1, &EBOrock[i]);

		glBindVertexArray(VAOrock[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBOrock[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOrock[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO

		mat4 transform;
		transform = translate(transform, vec3(xpos, ypos, zpos));
		GLint location = glGetUniformLocation(programRock[i], "transform");
		UseShader(programRock[i]);
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(transform));
	}

}


bool Main::IsCollided(float x1, float y1, float width1, float height1,
	float x2, float y2, float width2, float height2) {
	return (x1 < x2 + width2 && x1 + width1 > x2 && y1 < y2 + height2 && y1 + height1 > y2);
}

int main(int argc, char** argv) {

	Engine::Game &game = Main();
	game.Start("Four Mix The Game", 1280, 720, true, WindowFlag::WINDOWED, 0, 1);

	return 0;
}
