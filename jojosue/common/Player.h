enum BoxType{
  NO_BOX,
  PAC,
  SEDEX,
  EXPRESS
};

typedef struct{
    unsigned short int timeLast;
    char type;
}Box;

enum Items{
   NO_ITEM,
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

enum playerFace{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

typedef struct{

    unsigned char ID;
    unsigned char HP;
    unsigned char money;
    unsigned short posX, posY;
    Box boxArray[5];
    unsigned char itemArray[3];
    unsigned char skin;
    char ident;
    unsigned char face;
    char nome[20];
}Player_Data; 

typedef struct{
    unsigned char posX, posY;
    unsigned char face;
    unsigned char skin;
}Enemy_Data;