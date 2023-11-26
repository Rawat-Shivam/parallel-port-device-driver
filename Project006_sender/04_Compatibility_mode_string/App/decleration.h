// function decleration are included here
int init();

void* MainMenu(void*);
void* exitFunc(void*);
void* openFunc(void*);
void* releaseFunc(void*);
void* writeData(void*);

void* (*fptr[5])(void*);
