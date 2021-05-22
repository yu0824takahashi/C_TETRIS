#include<stdio.h>
#include"conio.h"
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>


//フィールドの高さと幅を定義
#define FIELD_WIDTH (10)
#define FIELD_HIGHT  (20)

#define SHAPE_WIDTH_MAX (4)
#define SHAPE_MAX_HIGHT  (4)

#define FPS (2)
#define INTERVAL (1000000 / FPS)

//プロトタイプ宣言
void Init(void);
void DrawScreen(void);
void InitMino(void);
bool MinoIntersectField(void);
//テトリミノ作成
enum{
    SHAPE_I ,
    SHAPE_O,
    SHAPE_S,
    SHAPE_Z,
    SHAPE_J,
    SHAPE_L,
    SHAPE_T,

    SHAPE_MAX

};

//テトリミノの形状の構造体
typedef struct{
    int width, height;
    int patern[SHAPE_MAX_HIGHT][SHAPE_WIDTH_MAX];

}SHAPE;


//テトリミノのデータ
typedef struct{
    //座標データ
    int x, y;

    //形状データ
    SHAPE shape;

}MINO;


SHAPE shapes[SHAPE_MAX] = {
    // SHAPE_I
    {
        // int width, height;
        4,4,
        // int patern[SHAPE_MAX_HIGHT][SHAPE_WIDTH_MAX]
        {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0}
        }
        
    },
    // SHAPE_O
    {
        // int width, height;
        2,2,
        // int patern[SHAPE_MAX_HIGHT][SHAPE_WIDTH_MAX]
        {
            {1,1},
            {1,1}
        }
        
    },
    // SHAPE_S
    {
        // int width, height;
        3,3,
        // int patern[SHAPE_MAX_HIGHT][SHAPE_WIDTH_MAX]
        {
            {0,1,1},
            {1,1,0},
            {0,0,0}
        }
        
    },
    // SHAPE_Z
    {
        // int width, height;
        3,3,
        // int patern[SHAPE_MAX_HIGHT][SHAPE_WIDTH_MAX]
        {
            {1,1,0},
            {0,1,1},
            {0,0,0}
        }
        
    },
    // SHAPE_J
    {
        // int width, height;
        3,3,
        // int patern[SHAPE_MAX_HIGHT][SHAPE_WIDTH_MAX]
        {
            {1,0,0},
            {1,1,1},
            {0,0,0}
        }
        
    },
    // SHAPE_L
    {
        // int width, height;
        3,3,
        // int patern[SHAPE_MAX_HIGHT][SHAPE_WIDTH_MAX]
        {
            {0,0,1},
            {1,1,1},
            {0,0,0}
        }
        
    },
    // SHAPE_T
    {
        // int width, height;
        3,3,
        // int patern[SHAPE_MAX_HIGHT][SHAPE_WIDTH_MAX]
        {
            {0,1,0},
            {1,1,1},
            {0,0,0}
        }
        
    }
};

//フィールドにブロックがあるかないかのデータ
int field[FIELD_HIGHT][FIELD_WIDTH];

//fieldとテトリミノを合成し描画するデータ
int screen[FIELD_HIGHT][FIELD_WIDTH];

//プレイヤーが操作するミノ
MINO mino;





int main(){
    //乱数初期化
    srandom((unsigned int)time(NULL));
    Init();
    //前回の更新時刻
    clock_t lastClock = clock();  //ゲームが起動してから何ミリ秒経過したか
    while(1)
    {
        //現在の時刻更新
        clock_t nowClock = clock();
        //前回の時間からINETRVALミリ秒経過した場合
        if(nowClock >= lastClock + INTERVAL)
        {
            lastClock = nowClock;

            //当たり判定
            //移動する前のミノを記憶する
            MINO lastmino = mino;

            mino.y++;  //ミノ落下

            //ミノが当たった場合
            if(MinoIntersectField())
            {
                //移動をなかった事にする
                mino = lastmino;

                //ミノの形状のループ
                for(int y = 0; y < mino.shape.height; y++)
                {
                    for(int x = 0; x < mino.shape.width; x++)
                    {
                        //地面に到達した場合、地面とどうかする
                        if(mino.shape.patern[y][x])
                        {
                            field[mino.y + y][mino.x + x] |= 1;
                        }
                    }
                }

                //ラインを消す(フィールドの上から順に各行を見ていく)
                for(int y = 0; y < FIELD_HIGHT; y++)
                {
                    //揃ったフラグ
                    bool completed = true;
                    for(int x = 0; x < FIELD_WIDTH; x++ )
                    {
                        //その行にブロックが揃っていなければ
                        if(!field[y][x])
                        {
                            completed = false;
                            break;
                        }
                    }

                    //揃っていた場合
                    if(completed)
                    {
                        for(int x = 0; x < FIELD_WIDTH; x++)
                        {
                            //その行を消す
                            field[y][x] = 0;
                            
                        }
                        //上のブロックをしたにずらす
                        for(int y2= y; y2 >= 1; y2--)  //下から上にループする
                        {
                            for(int x = 0; x < FIELD_WIDTH; x++)
                            {
                                //上のミノをコピーする
                                field[y2][x] = field[y2 - 1][x];
                                //上のブロックは消す
                                field[y2 - 1][x] = 0;
                            }
                        }
                    }
                }
                //ミノ初期化
                InitMino();
            }

            
            //際描画
            DrawScreen();
        }
        if(kbhit())
        {
            //移動する前のミノを記憶する
            MINO lastmino = mino;
            switch(getchar())
            {
                case 'w':
                    break;

                case 's':
                    mino.y++;
                    break;

                case 'a':
                    mino.x--;
                    break;

                case 'd':
                    mino.x++;
                    break;

                default:
                    {
                        MINO newMino = mino;
                        //ミノの形状のループ
                        for(int y = 0; y < mino.shape.height; y++)
                        {
                            for(int x = 0; x < mino.shape.width; x++)
                            {
                                newMino.shape.patern[mino.shape.width -1 -x][y] = mino.shape.patern[y][x];
                            }
                        }
                        mino = newMino;
                    }
                    break;

                
            }
            //ミノが壁に当たった場合
            if(MinoIntersectField())
            {
                //移動をなかった事にする
                mino = lastmino;
            }
            DrawScreen();
        }
    }
    return 0;
}


void DrawScreen(void)
{
    //スクリーンバッファにフィールドをコピーする
    memcpy(screen, field, sizeof field);

    //プレイヤーのテトリミノを書き込む
    for(int y = 0; y < mino.shape.height; y++)
    {
        for(int x = 0; x < mino.shape.width; x++)
        {
            //もし、ミノのパターンの座標にブロックがあるなら書き込む
            if(mino.shape.patern[y][x])
            {
                screen[mino.y + y][mino.x + x] |= 1;   //1を上書き
            }
        }
    }

    //画面クリア
    system("clear");

    for(int y = 0; y < FIELD_HIGHT; y ++)
    {
        //左の壁表示
        printf("⬜︎");
        for(int x = 0; x < FIELD_WIDTH; x ++ )
        {
            //スクリーンの座標を参照し、そこにブロックがあるなら⬜︎そうでないなら空白
            printf("%s",screen[y][x]?"🟪":"  ");
        }
        //右の壁を表示
        printf("⬜︎");
        printf("\n");
    }

    //床の描画
    for(int x = 0; x < FIELD_WIDTH + 2; x++)
    {
        printf("⬜︎");
    }
    printf("\n");
    
    
}


//初期化関数
void Init(void)
{
    //画面クリア
    system("clear");

    //フィールドクリア
    memset(field, 0, sizeof field);

    InitMino();


    DrawScreen();
}


//テトリミノ生成関数
void InitMino(void)
{
    //ランダムにミノを生成
    mino.shape = shapes[random()%SHAPE_MAX];
    //mino.shape = shapes[2];
    //ミノの初期座標
    mino.x = (FIELD_WIDTH - mino.shape.width) / 2;
    mino.y = 0;
}

//テトリミノとフィールドの当たり判定判別関数
//戻り値 true or false
bool MinoIntersectField(void)
{
    for(int y = 0; y < mino.shape.height; y++)
    {
        for(int x = 0; x < mino.shape.width; x++)
        {
            //ミノがある場合
            if(mino.shape.patern[y][x])
            {
                //フィールドのミノの座標がフィールドの大きさ未満だった場合
                //フィールドとミノの当たり判定
                if((mino.y+y< 0 || mino.y + y >= FIELD_HIGHT)
                ||(mino.x + x < 0) || (mino.x + x >= FIELD_WIDTH))
                {
                    //当たったフラグを返す
                    return true;
                }

                //ミノ同士の当たり判定
                if(field[mino.y + y][mino.x + x])    //フィールド＋ミノの座標にミノがある場合
                {
                    return true;
                }

            }
        }
    }
    return false;
}