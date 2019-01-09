#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_query.hpp>

enum state { drone_halt, drone_attack, drone_random };

typedef struct drone_
{
	glm::vec3 pos;
	glm::vec3 tar;
	glm::vec3 a;
	glm::vec3 v;
	state s;
} drone;

drone createDrone();
glm::vec3 caculateNextPos(drone *drone_entity);
void changeState(drone *drone_entity, int s);
