/**
 * 
 * 
 * 
 * 
 * 
 * 
 * */

typedef enum{
  NONE,
  PAC,
  SEDEX,
  EXPRESS
}BoxType;

typedef struct{
    unsigned short int timeLast;
    BoxType type;
}Box;

typedef enum{
   NONE,
   SHURICARD,
   MADDOG,
   TRAP,
   SPEED,
   SHIELD    
}Items;

enum skin{
    JOSUE,
    JOSIAS,
    MATIAS
};

typedef struct{

    unsigned char ID;
    unsigned char HP;
    unsigned char money;
    unsigned short posX, posY;
    Box boxArray[5];
    unsigned char itemArray[3];
    char skin;
}Player_Data; 


