/**
 * 
 * 
 * 
 * 
 * 
 * 
 * */

enum BoxType{
  NONE,
  PAC,
  SEDEX,
  EXPRESS
};

typedef struct{
    unsigned short int timeLast;
    char type;
}Box;

enum Items{
   NONE,
   SHURICARD,
   MADDOG,
   TRAP,
   SPEED,
   SHIELD    
};

enum skin{
    JOSUE,
    JOSIAS,
    MATIAS
};

enum MsgIdentifier{
    POS,
    MONEY,
    HP,
    ITEM,
    BOX
};

typedef struct{

    unsigned char ID;
    unsigned char HP;
    unsigned char money;
    unsigned short posX, posY;
    Box boxArray[5];
    unsigned char itemArray[3];
    char skin;
    char ident;
}Player_Data; 


