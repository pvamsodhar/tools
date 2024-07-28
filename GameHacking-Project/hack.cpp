#include <dlfcn.h>
#include <set>
#include <map>
#include <functional>
#include <string>
#include <cstring>
#include <vector>
#include<iostream>
#include "libGameLogic.h"

bool Player::CanJump() {
	return 1;
}

void Player::Chat(const char* msg) {
	if(strncmp("tp ", msg, 3) == 0){
		Vector3* new_pos = new Vector3();
		sscanf(msg+3, "%f %f %f", &(new_pos->x), &(new_pos->y), &(new_pos->z));
		this->SetPosition(*new_pos);
	} else if(strncmp("pos", msg, 3) == 0) {
		Vector3 new_pos = this->GetPosition();
		float x_coor = new_pos.x;
		float y_coor = new_pos.y;
		float z_coor = new_pos.z;
		printf("x:%f, y:%f, z:%f", x_coor, y_coor, z_coor);
		fflush(stdout);
	} else if(strncmp("tpz ", msg, 4) == 0){
		Vector3 curr_pos = this->GetPosition();
		Vector3* new_pos = new Vector3(curr_pos);
		sscanf(msg+4, "%f", &(new_pos->z));
		this->SetPosition(*new_pos);
	}

}

void World::Tick(float f) {
	ClientWorld* world = *(ClientWorld**)dlsym(RTLD_NEXT, "GameWorld");
	IPlayer* iplayer = world->m_activePlayer.m_object;
	Player* player = ((Player*)(iplayer));
	player->m_walkingSpeed = 99999;
	player->m_jumpSpeed = 999;
	player->m_jumpHoldTime = 99999;
}

int main(){

}
