#define SHM_NAME "/myshared"
#define STRU_NAME_SIZE 20
#define STRU_VALUES_SIZE 5
typedef struct{
	char name[STRU_NAME_SIZE];
	float values[STRU_VALUES_SIZE];
} stru;
