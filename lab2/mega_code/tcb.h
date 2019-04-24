#ifndef HEADERSTUFF
#define HEADERSTUFF

    void Measure(void* dataPtr);
    void Compute(void* dataPtr);
    void Status(void* dataPtr);
    void WarningAlarm(void* dataPtr);
    void Display(void* dataPtr);

    // Declare TCB
    typedef struct
    {
        void (*myTask)(void*);
        void* taskDataPtr; 
    } TCB;

# endif