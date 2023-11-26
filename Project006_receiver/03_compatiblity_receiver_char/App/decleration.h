// function decleration are included here
int init();

void* MainMenu(void*);
void* exitFunc(void*);
void* openFunc(void*);
void* releaseFunc(void*);
void* readData(void*);

void* (*fptr[5])(void*);
