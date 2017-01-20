#include <stdio.h>
#include <stdbool.h>

#define SIZE 8

typedef enum {WHT=-1, EMP=0, BLK=+1, OUT=+2} Piece;

typedef struct {
    Piece cell[SIZE][SIZE];
} Board;

typedef struct{
    int x;
    int y;
    Piece color;
} Hand;

Board brd_init(void);	// 盤面の初期化
void brd_output(Board brd);	// 盤面の出力
void player(Board *brd, Piece wh, bool *pass);	// プレイヤの処理
void hand_input(Hand *hand);	// 打ち手の入力
bool brd_turn(Board *brd, Hand hand);	// 盤面の更新
int brd_turn_dir(Board *brd, Hand hand, int dx, int dy);	// 指定方向への走査
bool end_check(Board brd);	// 終了の判定
Piece cell_val(Board brd, int x, int y);	// 指定位置のマスの状態
Piece winner_check(Board brd);	// 勝敗の判定
bool pass_check(Board brd, Piece wh);	// パスの判定


int main(void){
    //var
    Piece winner;
    Board brd;
    bool passBLK, passWHT;

    brd = brd_init();
    brd_output(brd);

    while(!end_check(brd)){
        printf("黒の番です。\n");
        player(&brd,BLK,&passBLK); // 先手(黒)の処理
		brd_output(brd);
        if(end_check(brd)) break;
        printf("白の番です。\n");
        player(&brd,WHT,&passWHT);	// 後手(白)の処理
		brd_output(brd);
        if ( passBLK && passWHT ) break;	// パスによる終了の判定
    }

    return 0;
}

Board brd_init(void) {
	//-- 宣言
	Board tmp = {{
		{EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP},
		{EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP},
		{EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP},
		{EMP, EMP, EMP, WHT, BLK, EMP, EMP, EMP},
		{EMP, EMP, EMP, BLK, WHT, EMP, EMP, EMP},
		{EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP},
		{EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP},
		{EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP}
	}};

	return tmp;
}

void brd_output(Board brd) {
	//-- 宣言
	int x, y;    // 反復変数

	//-- 出力処理
	printf("  ");
	for ( y = 0; y < SIZE; y++ ) { printf("Y%d",y);}
	for ( x = 0; x < SIZE; x++) {
		printf("\nX%d",x);
		for ( y = 0; y < SIZE; y++ ) {
			switch ( brd.cell[x][y] ) {
				case WHT: printf("Ｏ");   break;
				case EMP: printf("・");   break;
				case BLK: printf("Ｘ");   break;
				default : printf("？");
			}
		}
	}
	printf("\n\n");
}

void player(Board *brd, Piece wh, bool *pass) {
	//-- 宣言
	Hand hand;	// 打ち手格納用変数
	hand.color = wh;
	*pass = false;

	while ( 1 ) {
		//-- 打ち手入力
		hand_input(&hand);
		//-- 盤面更新
		brd->cell[hand.x][hand.y] = wh;
		// 盤面の更新と判定
		break;
	}
	printf("%d %d\n", hand.x, hand.y);
}

void hand_input(Hand *hand) {
	//-- 宣言
	int x, y;	// 入力格納用変数

	//--入力処理
	while ( 1 ) {
		printf("X > ");
		scanf("%d", &x);
		if ( x >= 0 && x < SIZE ) { break; }	// 正しい入力なら反復を脱出
		printf("不正な入力です。  再入力してください。\n");
	}

	while ( 1 ) {
		printf("Y > ");
		scanf("%d", &y);
		if ( y >= 0 && y < SIZE ) { break; }	// 正しい入力なら反復を脱出
		printf("不正な入力です。  再入力してください。\n");
	}

	//-- 格納
	hand->x = x;
	hand->y = y;
}

bool end_check(Board brd) {
	int x, y;
	for ( x = 0; x < SIZE; x++ ) {
		for ( y = 0; y < SIZE; y++ ) {
			if ( brd.cell[x][y] == EMP ) {   // brd.cell[x][y]がEMPか調べる
				return false;                // EMPならfalseを返す
			}
		}
	}
	return true;    // EMPがなければ終了
}