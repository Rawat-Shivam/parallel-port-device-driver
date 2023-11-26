// function decleration are included here
int init();

void* MainMenu(void*);
void* exitFunc(void*);
void* openFunc(void*);
void* releaseFunc(void*);
void* writeData(void*);
void* readData(void*);

void* (*fptr[6])(void*);
